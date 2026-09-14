# UNO Q Linux Ground Control

This directory contains the Debian/Linux half of the UNO Q ground station. It is
kept separate from `../mcu/` because the two sides use different runtimes and
deployment tools.

## Run the current baseline

Python 3 is the only current requirement:

```sh
python main.py
```

The current program exercises the multi-node `NodeRegistry` with two simulated
R4 nodes. The receiver, Purik binary codec, persistence, API/WebSocket service,
and dashboard are still placeholders. Those pieces should depend on normalized
Purik schemas and never import sensor-vendor parsing code.

## Layout

- `receiver/` — transport ingestion from the UNO Q MCU and future transports
- `protocol/` — validation and decoding of versioned Purik frames
- `services/` — node/device registry, logging, analysis, and future fusion
- `api/` — local API and WebSocket boundary
- `dashboard/` — touchscreen application
