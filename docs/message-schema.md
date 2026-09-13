# Purik Message Schema — Draft v1

## Envelope

Each message should carry enough metadata for routing, validation, ordering, health monitoring, and future replay.

Proposed fields:

- protocol version
- source node ID
- sensor/device ID
- sensor class
- message type
- payload schema version
- sequence number
- timestamp
- payload length
- flags

## Initial sensor classes

- `RADAR`
- `LIDAR`
- `GNSS`
- `IMU`
- `SYSTEM`

## Initial message types

- `RADAR_TARGET_LIST`
- `LIDAR_SCAN_SAMPLE`
- `GNSS_FIX`
- `IMU_SAMPLE`
- `DEVICE_ANNOUNCE`
- `DEVICE_HEALTH`
- `NODE_HEARTBEAT`

## Unit policy

Prefer explicit SI units in field names or schema documentation. Avoid vendor-specific scaling in messages crossing the Purik boundary.

Examples:

- position: meters
- velocity: meters/second
- angles: degrees unless otherwise documented
- acceleration: meters/second^2
- time: milliseconds or a future synchronized timestamp representation

## RF constraint

nRF24L01 payload size is limited, so larger logical Purik messages will require compact payloads and/or fragmentation/reassembly. The transport framing strategy remains intentionally open in this baseline.
