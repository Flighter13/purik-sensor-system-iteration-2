#pragma once

#include <Arduino.h>
#include "SensorDriver.h"

class SensorRegistry {
 public:
  static constexpr size_t MAX_SENSORS = 8;

  SensorRegistry();

  bool registerDriver(SensorDriver& driver);
  void beginAll();
  void probeAll();
  void updateAll();

  size_t count() const;
  size_t presentCount() const;

  SensorDriver* driverAt(size_t index) const;
  SensorDriver* findById(uint16_t sensorId) const;
  SensorDriver* findByClass(Purik::SensorClass sensorClass) const;

  void printSummary(Stream& out) const;

 private:
  SensorDriver* drivers_[MAX_SENSORS];
  size_t count_;
};
