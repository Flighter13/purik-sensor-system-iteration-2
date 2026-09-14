# UNO Q MCU

This directory contains the UNO Q's independently buildable STM32U585
microcontroller firmware. UNO Q MCU sketches use the Arduino Zephyr core and are
built separately from the Debian/Linux application.

The package shape is:

```text
mcu/
└── purik-ground-station-mcu/
    ├── purik-ground-station-mcu.ino
    └── sketch.yaml
```

Build and upload it with the official Arduino tooling:

```sh
arduino-cli compile --profile uno_q purik-ground-station-mcu
arduino-cli upload --fqbn arduino:zephyr:unoq -p <port> purik-ground-station-mcu
```

The current sketch is a build/upload smoke-test that reports MCU readiness over
serial. It does not yet receive nRF24 frames or forward them to Linux. Those
features depend on the final radio wiring and MCU/Linux bridge contract. When
they are implemented, MCU-side code should consume the repository's shared
Purik protocol contract instead of defining a second wire format.
