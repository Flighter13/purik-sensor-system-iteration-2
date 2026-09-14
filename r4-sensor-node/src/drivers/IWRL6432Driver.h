#pragma once

#include <Arduino.h>
#include "../core/SensorDriver.h"

struct IWRL6432Target {
  uint16_t targetId = 0;
  float rangeM = 0.0f;
  float azimuthDeg = 0.0f;
  float elevationDeg = 0.0f;
  float radialVelocityMps = 0.0f;
};

// Interface scaffold for the TI radar adapter boundary.
// The concrete UART/TLV implementation is intentionally deferred until the
// UNO R4 radar UART pins and electrical interface are finalized.
class IWRL6432Driver : public SensorDriver {
 public:
  IWRL6432Driver(uint16_t sensorId);

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

  bool hasTarget() const;
  const IWRL6432Target& target() const;
  void clearTarget();

 private:
  uint16_t sensorId_;
  bool present_;
  bool initialized_;
  bool healthy_;
  bool targetReady_;
  uint32_t lastUpdateMs_;
  IWRL6432Target target_;
  Purik::SensorDescriptor descriptor_;
};
