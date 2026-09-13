#pragma once

#include <Arduino.h>
#include "../../common/PurikSensorTypes.h"

class SensorDriver {
 public:
  virtual ~SensorDriver() = default;

  virtual bool begin() = 0;
  virtual bool probe() = 0;
  virtual void update() = 0;
  virtual bool healthy() const = 0;

  virtual uint16_t sensorId() const = 0;
  virtual Purik::SensorClass sensorClass() const = 0;
  virtual const char* name() const = 0;
};
