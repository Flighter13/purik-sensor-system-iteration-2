#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "../core/SensorDriver.h"

class GY521Driver : public SensorDriver {
 public:
  GY521Driver(TwoWire& wire, uint8_t address, uint16_t sensorId);

  bool begin() override;
  bool probe() override;
  void update() override;
  bool healthy() const override;
  bool present() const override;
  bool initialized() const override;
  uint32_t lastUpdateMs() const override;

  uint16_t sensorId() const override;
  Purik::SensorClass sensorClass() const override;
  const char* name() const override;
  const Purik::SensorDescriptor& descriptor() const override;

 private:
  bool readRegister(uint8_t reg, uint8_t& value);
  bool writeRegister(uint8_t reg, uint8_t value);

  TwoWire& wire_;
  uint8_t address_;
  uint16_t sensorId_;
  bool present_;
  bool initialized_;
  bool healthy_;
  uint32_t lastUpdateMs_;
  Purik::SensorDescriptor descriptor_;
};
