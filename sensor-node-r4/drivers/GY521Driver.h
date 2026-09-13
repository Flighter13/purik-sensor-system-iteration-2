#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "../core/SensorDriver.h"

struct GY521Reading {
  float accelXMps2;
  float accelYMps2;
  float accelZMps2;
  float gyroXDps;
  float gyroYDps;
  float gyroZDps;
  float temperatureC;
  uint32_t timestampMs;
};

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

  const GY521Reading& reading() const;
  bool hasFreshReading() const;
  void clearFreshReading();

 private:
  bool readRegister(uint8_t reg, uint8_t& value);
  bool writeRegister(uint8_t reg, uint8_t value);
  bool readBurst(uint8_t startReg, uint8_t* buffer, size_t length);

  TwoWire& wire_;
  uint8_t address_;
  uint16_t sensorId_;
  bool present_;
  bool initialized_;
  bool healthy_;
  bool freshReading_;
  uint32_t lastUpdateMs_;
  GY521Reading reading_;
  Purik::SensorDescriptor descriptor_;
};
