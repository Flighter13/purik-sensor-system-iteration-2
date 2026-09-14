# UNO Q Ground Station

This directory is the UNO Q ground-control package. The board has two execution
environments, so the package deliberately keeps them separate:

- `mcu/` contains the STM32U585/Zephyr Arduino sketch and its build/upload
  instructions.
- `linux/` contains Debian-side Python services, telemetry handling, API code,
  and dashboard components.

These are independently built and deployed; neither side is compiled as part of
the UNO R4 PlatformIO project. The MCU will consume the repository-level C++
protocol contract when bridge/receiver code is added. The Linux side should use
a matching Python codec rather than including Arduino headers directly.

At present this is a structural baseline, not a complete ground station. See the
subpackage READMEs for runnable pieces and known gaps.
