# Purik Iteration 2 Interface Control Document — Draft

## Purpose

Define stable interfaces between sensor hardware, node-side drivers/adapters, transports, and the UNO Q ground-control system.

## Interface boundaries

### Sensor driver interface

Each sensor driver should eventually expose a common lifecycle:

- `begin()` — initialize the physical device
- `probe()` — verify presence/identity
- `update()` — service the device without blocking the whole node
- `healthy()` — report current health
- `capabilities()` — report supported outputs

Driver implementations may use vendor-specific buses and protocols internally.

### Payload adapter interface

Adapters convert driver-native data into Purik-standard message payloads. The adapter owns unit normalization and schema version selection.

Examples:

- TI target output -> `RADAR_TARGET_LIST`
- A1M8 scan sample -> `LIDAR_SCAN_SAMPLE` / future scan packet
- MAX-M10S navigation data -> `GNSS_FIX`
- MPU-6050 measurements -> `IMU_SAMPLE`

### Transport interface

Transport code moves serialized Purik frames without interpreting sensor-specific payload contents.

Initial transport: nRF24L01.

Future transports may include Wi-Fi without changing the logical Purik message schema.

## Ground-control interface

The ground station consumes Purik messages, not vendor-specific sensor protocols. Unknown message types should be rejectable or retainable for logging without crashing the receiver.

## Versioning

Every Purik frame includes a protocol version. Payload schemas should be explicitly versioned when incompatible changes occur.

## Open items

- final nRF24 packet/framing strategy
- fragmentation/reassembly for payloads larger than one RF packet
- timestamp authority and synchronization
- exact UART allocation on UNO R4
- final payload structures for radar and LiDAR
