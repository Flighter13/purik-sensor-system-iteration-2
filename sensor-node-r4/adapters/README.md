# Payload adapters

Adapters translate driver-native data into versioned Purik payload schemas. They normalize units and keep vendor-specific data structures from crossing the Purik interface boundary.

## Implemented adapters

- `GnssAdapter` — `MAXM10SReading` -> `GnssFixPayloadV1`
- `ImuAdapter` — `GY521Reading` -> `ImuSamplePayloadV1`
- `RadarAdapter` — `IWRL6432Target` -> `RadarTargetV1`
- `LidarAdapter` — `RPLidarA1Sample` -> `LidarSamplePayloadV1`

The radar and LiDAR adapters are ready, but their drivers remain interface scaffolds until the UNO R4 UART and electrical details are finalized.

## Design rule

Ground control should consume Purik schemas such as `gnss.fix.v1`, `imu.sample.v1`, `radar.target.v1`, and `lidar.sample.v1`, not vendor-native structures.
