# Purik Sensor System — Iteration 2

Iteration 2 separates sensor-node drivers and adapters from ground-control processing and the user interface.

## Planned layout

- `common/` — shared protocol definitions
- `docs/` — architecture and interface documentation
- `sensor-node-r4/` — Arduino UNO R4 WiFi sensor-node firmware
- `ground-control-q/` — Arduino UNO Q ground-control software
- `tests/` — protocol and integration tests

## Initial hardware

Sensor node: Arduino UNO R4 WiFi with IWRL6432BOOST radar, RPLIDAR A1M8, MAX-M10S GNSS, GY-521 IMU, and nRF24L01 radio.

Ground control: Arduino UNO Q with nRF24L01 radio and touchscreen dashboard.

## Design rule

Drivers handle hardware-specific details. Adapters translate sensor output into a common Purik message format. Transports move those messages. Ground-control applications consume the common format.

## Current phase

The repository is being established as the clean Iteration 2 baseline. Hardware-specific drivers will be added after finalizing the UNO R4 UART allocation and verifying interface voltage/power requirements for the radar and LiDAR.
