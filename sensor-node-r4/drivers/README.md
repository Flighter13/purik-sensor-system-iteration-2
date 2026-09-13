# Sensor drivers

Vendor- and device-specific hardware drivers live here.

Planned drivers:

- `IWRL6432Driver` — TI mmWave radar
- `RPLidarA1Driver` — Slamtec A1M8
- `MAXM10SDriver` — u-blox GNSS
- `GY521Driver` — MPU-6050 IMU

Drivers should expose the common `SensorDriver` lifecycle and keep vendor-specific protocol details out of the application layer.
