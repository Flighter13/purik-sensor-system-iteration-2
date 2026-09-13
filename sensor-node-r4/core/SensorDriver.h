#pragma once

#include <Arduino.h>
#include "../../common/PurikSensorTypes.h"
#include "../../common/SensorDescriptor.h"

class SensorDriver {
 public:
  virtual ~SensorDriver() = default;

  virtual bool begin() = 0;
  virtual bool probe() = 0;
  virtual void update() = 0;
  virtual bool healthy() const = 0;

  virtual bool present() const = 0;
  virtual bool initialized() const = 0;
  virtual uint32_t lastUpdateMs() const = 0;

  virtual uint16_t sensorId() const = 0;
  virtual Purik::SensorClass sensorClass() const = 0;
  virtual const char* name() const = 0;
  virtual const Purik::SensorDescriptor& descriptor() const = 0;
};
