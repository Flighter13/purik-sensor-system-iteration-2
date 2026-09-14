# UNO R4 Sensor Node

This directory is the independently buildable and uploadable firmware package for
the Arduino UNO R4 WiFi sensor nodes. It keeps an Arduino sketch entry point at
`src/purik-sensor-node.ino` while PlatformIO compiles the sketch and the modular
C++ sources below `src/`.

## Layout

- `src/purik-sensor-node.ino` owns the single `setup()` and `loop()` pair.
- `src/drivers/` contains sensor-specific acquisition and parsing.
- `src/adapters/` translates driver readings into shared Purik payloads.
- `src/core/` contains discovery, lifecycle, and registry code.
- `src/config/` contains board wiring and per-node configuration.
- `src/transport/` is reserved for nRF24 and later transport implementations.
- `../common/` contains the shared protocol and message headers. PlatformIO adds
  this directory to the compiler include path; code should include headers by
  name, for example `#include "PurikMessages.h"`.

## Build

From this directory:

```sh
pio run -e sensor_node_1
pio run -e sensor_node_2
```

`sensor_node_1` is the default environment, so `pio run` builds node 1.

## Upload

```sh
pio run -e sensor_node_1 -t upload
pio run -e sensor_node_2 -t upload
```

## Serial monitor

```sh
pio device monitor -b 115200
```

Open `src/purik-sensor-node.ino` when you want an Arduino-style sketch entry
point. Do not add a second `main.cpp` or another sketch that defines `setup()` or
`loop()`; PlatformIO would compile both and fail with duplicate definitions.

The same firmware package supports both physical R4 nodes. The two PlatformIO
environments set `PURIK_NODE_SLOT`; there are no copied node-specific sources.

The GNSS and IMU drivers are implemented. Radar, LiDAR, nRF24 transport, and
Wi-Fi transport remain deliberate hardware/integration gaps described in the
repository documentation.
