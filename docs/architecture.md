# Iteration 2 Architecture

## Repository and build boundaries

The repository is a monorepo, but it is not one firmware image:

- `r4-sensor-node/` is a PlatformIO project for either UNO R4 WiFi node. Its
  Arduino entry point is `src/purik-sensor-node.ino`; all other firmware remains
  in modular drivers, adapters, registry/core, configuration, and transport
  sources.
- `uno-q-ground-station/mcu/` is the STM32U585 Arduino/Zephyr package, built with
  the UNO Q Arduino CLI platform. It currently contains a build/upload smoke
  test; the receiver and bridge are pending.
- `uno-q-ground-station/linux/` is the separately run Debian/Python application.
- `common/` is the single source of truth for the embedded Purik protocol,
  message, version, sensor, and descriptor types.

The R4 firmware, UNO Q MCU smoke test, and UNO Q Linux registry baseline are
executable today. The UNO Q receiver, radio wiring, and MCU/Linux bridge contract
remain to be implemented.

## System split

### Sensor Nodes — Two Arduino UNO R4 WiFi boards

Iteration 2 supports two R4 sensor nodes simultaneously. Both boards run the same firmware architecture and differ only by node identity and attached hardware.

Responsibilities:

- own sensor-specific drivers
- probe supported interfaces for attached devices
- register detected devices at runtime
- own vendor/protocol-specific parsing
- translate measurements into common Purik payloads
- maintain local sensor presence, initialization, and health
- timestamp and sequence outgoing messages
- advertise node identity and attached sensor capabilities
- transmit Purik messages over nRF24L01
- optionally support Wi-Fi as a secondary transport

Initial nodes:

- `sensor-node-01` (`nodeId = 1`)
- `sensor-node-02` (`nodeId = 2`)

Supported sensor families:

- TI IWRL6432BOOST mmWave radar
- Slamtec RPLIDAR A1M8
- u-blox MAX-M10S GNSS
- GY-521 / MPU-6050 IMU

Sensors are not permanently assigned to a particular R4. Runtime discovery is intended to let either node host any supported sensor set that its available interfaces can electrically and physically support.

## Runtime sensor registry

Each R4 contains a `SensorRegistry` of driver instances. Drivers implement the common `SensorDriver` interface and expose a `SensorDescriptor` containing:

- node-local sensor ID
- sensor class
- driver ID
- display name
- output schema name
- schema version

The `DeviceManager` periodically probes supported devices and performs lifecycle transitions:

```text
supported driver
    |
probe interface
    |
+---+-------------------+
|                       |
not present          present
                        |
                     begin()
                        |
                    initialized
                        |
                     update()
                        |
             health / data / detach
```

A sensor can therefore be connected or disconnected without changing ground-control application code. The node can report attachment, detachment, health, and capability information upstream.

### First implemented discovery drivers

- MAX-M10S: detected by I2C address `0x42`
- GY-521 / MPU-6050: detected through the `WHO_AM_I` register at I2C address `0x68`/`0x69`

Radar and LiDAR drivers remain intentionally pending until the UNO R4 UART and electrical-interface arrangements are finalized.

### Ground Control — Arduino UNO Q

Responsibilities:

- receive Purik messages from both R4 nodes simultaneously
- key all state by `nodeId` and `sensorId`
- maintain node/device registry and online/offline state
- process attach/detach and health transitions
- validate protocol versions and message types
- log telemetry
- perform analysis and future sensor fusion across nodes
- expose telemetry through a local API / WebSocket service
- drive the Corsair touchscreen dashboard

## Multi-node topology

```text
 R4 sensor-node-01                  R4 sensor-node-02
 +------------------+               +------------------+
 | SensorRegistry   |               | SensorRegistry   |
 | DeviceManager    |               | DeviceManager    |
 | drivers/adapters |               | drivers/adapters |
 +---------+--------+               +---------+--------+
           |                                  |
        nRF24                               nRF24
           \                                  /
            \                                /
             +--------- Purik RF ------------+
                          |
                          v
                     Arduino UNO Q
                     Ground Control
                          |
                    NodeRegistry
                          |
               registry / fusion / log
                          |
                     web dashboard
                          |
                  Corsair touchscreen
```

## Layering

```text
Sensor hardware
    |
Vendor-specific driver
    |
SensorRegistry / DeviceManager
    |
Payload adapter
    |
Purik common message (nodeId + sensorId + schema)
    |
Transport (nRF24 / future Wi-Fi)
    |
Ground-control receiver
    |
Purik decoder + multi-node NodeRegistry
    |
Analysis / logging / fusion
    |
API / WebSocket
    |
Dashboard
```

## Node identity rule

Node identity belongs to the R4 board, not to a sensor. Sensor identity belongs to the attached logical device. A message therefore carries both identities:

```text
nodeId = 1
sensorId = 4
sensorClass = Radar
messageType = RadarTargetList
```

If that radar is removed and a LiDAR is attached, the node remains `nodeId = 1`; only the registered sensor/capability set changes.

## Design principle

The stable integration boundary is the Purik message contract, not a vendor command set. A new sensor requires a driver and adapter on the node, while the ground station should remain unchanged when the resulting capability/message schema already exists.

## Planned evolution

1. prove runtime I2C discovery and lifecycle management
2. add GNSS and IMU measurement payload adapters
3. add nRF24 message transport and ground-control decoding
4. verify dual-node simultaneous operation
5. add LiDAR and radar drivers after UART/interface validation
6. add richer manifests, configuration negotiation, logging/replay, health monitoring, and redundant transports
