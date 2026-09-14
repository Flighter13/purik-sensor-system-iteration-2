# Purik Sensor System — Iteration 2

Iteration 2 is organized as a monorepo containing independently buildable/deployable board packages plus shared protocol definitions.

## Repository layout

- `common/` — shared Purik protocol, message, version, sensor, and descriptor definitions
- `r4-sensor-node/` — Arduino UNO R4 WiFi sensor-node package with an Arduino
  `.ino` entry point and PlatformIO build/upload configuration
- `uno-q-ground-station/` — Arduino UNO Q ground-control package
  - `mcu/` — UNO Q microcontroller-side bridge/control code
  - `linux/` — Debian-side Python services, telemetry, API, and dashboard
- `docs/` — architecture and interface documentation
- `tests/` — protocol and integration tests as they are added

## Design rule

Each completed board/application package must be independently buildable. Shared
protocol contracts live at repository level rather than being duplicated inside
board packages. Packages consume those contracts through their build tooling.

Drivers handle hardware-specific details. Adapters translate sensor output into a common Purik message format. Transports move those messages. Ground-control applications consume the common format.

## R4 sensor node

From `r4-sensor-node/`:

```sh
pio run -e sensor_node_1
pio run -e sensor_node_2
pio run -e sensor_node_1 -t upload
pio device monitor -b 115200
```

The same R4 package supports multiple physical nodes. PlatformIO selects the
node identity through `sensor_node_1` and `sensor_node_2`; the Arduino entry point
is `r4-sensor-node/src/purik-sensor-node.ino`, and it is the only file defining
`setup()` and `loop()`.

## UNO Q ground station

The UNO Q package separates its STM32U585 MCU and Debian/Linux execution
environments so each can be developed and deployed independently while remaining
part of the same ground-control system. The Linux registry baseline is runnable,
and the MCU contains an independently buildable/uploadable smoke-test sketch;
the actual receiver/bridge remains a documented implementation gap.

## Current implementation status

- R4: PlatformIO package, Arduino sketch entry point, modular registry, drivers,
  adapters, and shared message types.
- UNO Q Linux: Python multi-node registry baseline; receiver, codec, API, and
  dashboard are pending.
- UNO Q MCU: Arduino/Zephyr sketch package and official toolchain profile;
  nRF24 receiver and Linux bridge are pending.
- Hardware/runtime validation: radar and LiDAR UART/electrical interfaces,
  nRF24 transport, dual-node RF operation, and on-device sensor testing remain.
