#pragma once

#include <Arduino.h>

namespace Purik {

struct NodeHeartbeatPayloadV1 {
  uint32_t uptimeMs;
  uint8_t presentSensorCount;
  uint8_t registeredDriverCount;
};

struct DeviceAnnouncePayloadV1 {
  uint16_t sensorId;
  uint8_t sensorClass;
  uint8_t schemaVersion;
  uint8_t attached;
  uint8_t healthy;
  char driverId[24];
  char schemaName[24];
  char displayName[32];
};

struct GnssFixPayloadV1 {
  int32_t latitudeE7;
  int32_t longitudeE7;
  int32_t altitudeMm;
  uint32_t groundSpeedMmS;
  uint32_t headingE5;
  uint8_t satellites;
  uint8_t valid;
};

struct ImuSamplePayloadV1 {
  float accelXMps2;
  float accelYMps2;
  float accelZMps2;
  float gyroXDps;
  float gyroYDps;
  float gyroZDps;
  float temperatureC;
};

struct RadarTargetV1 {
  uint16_t targetId;
  float rangeM;
  float azimuthDeg;
  float elevationDeg;
  float radialVelocityMps;
};

struct LidarSamplePayloadV1 {
  float angleDeg;
  float distanceM;
  uint8_t quality;
};

struct DeviceHealthPayloadV1 {
  uint8_t online;
  uint8_t healthy;
  uint16_t errorCode;
  uint32_t lastUpdateMs;
};

}  // namespace Purik
