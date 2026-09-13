#pragma once

#include <Arduino.h>
#include "PurikSensorTypes.h"

namespace Purik {

struct SensorDescriptor {
  uint16_t sensorId;
  SensorClass sensorClass;
  const char* driverId;
  const char* displayName;
  const char* schemaName;
  uint8_t schemaVersion;
};

}  // namespace Purik
