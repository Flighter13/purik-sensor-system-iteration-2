# UNO Q Ground Station

This directory is the independently deployable UNO Q ground-control package.

- `mcu/` contains microcontroller-side bridge/control code for the UNO Q.
- `linux/` contains Debian-side Python services, telemetry handling, API code, and dashboard components.

The package consumes repository-level protocol definitions from `../common/` conceptually, while the Linux side should implement matching schemas in Python rather than including Arduino headers directly.
