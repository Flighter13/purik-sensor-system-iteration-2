# Payload adapters

Adapters translate driver-native data into versioned Purik payload schemas.

Examples:

- TI radar targets -> `RADAR_TARGET_LIST`
- A1M8 samples -> `LIDAR_SCAN_SAMPLE`
- MAX-M10S navigation data -> `GNSS_FIX`
- MPU-6050 measurements -> `IMU_SAMPLE`

Adapters should normalize units and keep vendor-specific data structures from crossing the Purik interface boundary.
