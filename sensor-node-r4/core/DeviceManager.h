#pragma once

#include <Arduino.h>
#include "SensorRegistry.h"

class DeviceManager {
 public:
  explicit DeviceManager(SensorRegistry& registry);

  void begin();
  void update();
  void forceProbe();
  void printSummary(Stream& out) const;

 private:
  void probeAndInitialize();
  void reportTransitions();

  SensorRegistry& registry_;
  bool lastPresent_[SensorRegistry::MAX_SENSORS];
  bool lastHealthy_[SensorRegistry::MAX_SENSORS];
  uint32_t lastProbeMs_;
};
