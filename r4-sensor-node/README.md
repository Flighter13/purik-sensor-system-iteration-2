# UNO R4 Sensor Node

This directory is an independent PlatformIO application for Arduino UNO R4 WiFi sensor nodes.

## Build

From this directory:

```sh
pio run
```

## Upload

```sh
pio run -t upload
```

## Serial monitor

```sh
pio device monitor
```

Shared protocol and message definitions remain in the repository-level `../common/` directory and are added to the compiler include path by `platformio.ini`.

The same firmware package is intended to support multiple physical R4 sensor nodes. Node identity and enabled capabilities belong in configuration rather than separate copies of the application.
