# Sensor drivers

Vendor- and device-specific hardware drivers live here. All drivers implement the common `SensorDriver` lifecycle and keep vendor-specific protocol details out of the application layer.

## Implemented

- `MAXM10SDriver` — probes the u-blox MAX-M10S over I2C, initializes the SparkFun u-blox GNSS v3 library, and publishes position, altitude, speed, heading, satellites, fix type, and fix validity.
- `GY521Driver` — probes the MPU-6050 with `WHO_AM_I`, wakes it, reads the 14-byte accelerometer/temperature/gyro block, and converts native counts into m/s^2, deg/s, and degrees C.

## Interface scaffolds

- `IWRL6432Driver` — defines the normalized radar target boundary. The concrete UART/TLV implementation remains disabled until the UNO R4 UART pins and radar logic-level interface are finalized.
- `RPLidarA1Driver` — defines the normalized LiDAR scan-sample boundary. The concrete UART implementation remains disabled until the node UART allocation and electrical wiring are finalized.

The placeholder radar and LiDAR implementations deliberately report `present = false`; this prevents unimplemented hardware from being advertised as attached.

## Arduino library dependency

GNSS acquisition requires the **SparkFun u-blox GNSS v3** Arduino library (`SparkFun_u-blox_GNSS_v3.h`).
