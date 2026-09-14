# Purik Sensor System — Iteration 2

Iteration 2 is organized as a monorepo containing independently buildable/deployable board packages plus shared protocol definitions.

## Repository layout

- `common/` — shared Purik protocol, message, version, sensor, and descriptor definitions
- `r4-sensor-node/` — Arduino UNO R4 WiFi PlatformIO sensor-node package
- `uno-q-ground-station/` — Arduino UNO Q ground-control package
  - `mcu/` — UNO Q microcontroller-side bridge/control code
  - `linux/` — Debian-side Python services, telemetry, API, and dashboard
- `docs/` — architecture and interface documentation
- `tests/` — protocol and integration tests as they are added

## Design rule

Each board/application package must be independently buildable. Shared protocol contracts live at repository level rather than being duplicated inside board packages.

Drivers handle hardware-specific details. Adapters translate sensor output into a common Purik message format. Transports move those messages. Ground-control applications consume the common format.

## R4 sensor node

From `r4-sensor-node/`:

```sh
pio run
pio run -t upload
pio device monitor
```

The same R4 package is intended to support multiple physical nodes, with node identity and enabled capabilities controlled by configuration.

## UNO Q ground station

The UNO Q package separates the MCU and Debian/Linux execution environments so each can be developed and deployed independently while remaining part of the same ground-control system.
