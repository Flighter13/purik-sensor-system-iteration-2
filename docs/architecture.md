# Iteration 2 Architecture

## System split

### Sensor Node — Arduino UNO R4 WiFi

Responsibilities:

- own sensor-specific drivers
- own vendor/protocol-specific parsing
- translate measurements into common Purik payloads
- maintain local sensor health/status
- timestamp and sequence outgoing messages
- transmit Purik messages over nRF24L01
- optionally support Wi-Fi as a secondary transport later

Planned sensors:

- TI IWRL6432BOOST mmWave radar
- Slamtec RPLIDAR A1M8
- u-blox MAX-M10S GNSS
- GY-521 / MPU-6050 IMU

### Ground Control — Arduino UNO Q

Responsibilities:

- receive Purik messages from one or more nodes
- maintain node/device registry
- validate protocol versions and message types
- log telemetry
- perform analysis and future sensor fusion
- expose telemetry through a local API / WebSocket service
- drive the Corsair touchscreen dashboard

## Layering

```text
Sensor hardware
    |
Vendor-specific driver
    |
Payload adapter
    |
Purik common message
    |
Transport (nRF24 / future Wi-Fi)
    |
Ground-control receiver
    |
Purik decoder + device registry
    |
Analysis / logging / fusion
    |
API / WebSocket
    |
Dashboard
```

## Design principle

The stable integration boundary is the Purik message contract, not a vendor command set. A new sensor requires a driver and adapter on the node, while the ground station should remain unchanged when the resulting capability/message schema already exists.

## Planned evolution

Phase 1 uses known compile-time devices while preserving clean driver interfaces. Later phases add runtime discovery, manifests, capability advertisement, configuration negotiation, health monitoring, replay, and multiple transport options.
