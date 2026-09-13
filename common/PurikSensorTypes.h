#pragma once

#include <Arduino.h>

namespace Purik {

enum class SensorClass : uint8_t {
  Unknown = 0,
  Radar = 1,
  Lidar = 2,
  Gnss = 3,
  Imu = 4,
  System = 5
};

enum class MessageType : uint8_t {
  Unknown = 0,
  DeviceAnnounce = 1,
  DeviceHealth = 2,
  NodeHeartbeat = 3,
  RadarTargetList = 10,
  LidarScanSample = 20,
  GnssFix = 30,
  ImuSample = 40
};

}  // namespace Purik
