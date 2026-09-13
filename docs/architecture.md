# Iteration 2 Architecture

## System split

### Sensor Nodes — Two Arduino UNO R4 WiFi boards

Iteration 2 supports two R4 sensor nodes simultaneously. Both boards run the same firmware architecture and differ only by node identity and attached hardware.

Responsibilities:

- own sensor-specific drivers
- own vendor/protocol-specific parsing
- translate measurements into common Purik payloads
- maintain local sensor health/status
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

### Ground Control — Arduino UNO Q

Responsibilities:

- receive Purik messages from both R4 nodes simultaneously
- key all state by `nodeId` and `sensorId`
- maintain node/device registry and online/offline state
- validate protocol versions and message types
- log telemetry
- perform analysis and future sensor fusion across nodes
- expose telemetry through a local API / WebSocket service
- drive the Corsair touchscreen dashboard

## Multi-node topology

```text
 R4 sensor-node-01                  R4 sensor-node-02
 +------------------+               +------------------+
 | swappable sensors|               | swappable sensors|
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
Payload adapter
    |
Purik common message (nodeId + sensorId + schema)
    |
Transport (nRF24 / future Wi-Fi)
    |
Ground-control receiver
    |
Purik decoder + multi-node registry
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

Phase 1 proves simultaneous operation of two known nodes. Later phases add runtime sensor discovery, manifests, capability advertisement, configuration negotiation, health monitoring, recording/replay, redundant transports, and more than two nodes without changing the message model.
