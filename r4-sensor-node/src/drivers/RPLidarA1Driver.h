#pragma once

#include <Arduino.h>
#include "../core/SensorDriver.h"

struct RPLidarA1Sample {
  float angleDeg = 0.0f;
  float distanceM = 0.0f;
  uint8_t quality = 0;
};

// Interface scaffold for the Slamtec A1M8 adapter boundary.
// The concrete UART implementation is deferred until the node UART allocation
// and logic-level wiring are finalized.
class RPLidarA1Driver : public SensorDriver {
 public:
  explicit RPLidarA1Driver(uint16_t sensorId);

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

  bool hasSample() const;
  const RPLidarA1Sample& sample() const;
  void clearSample();

 private:
  uint16_t sensorId_;
  bool present_;
  bool initialized_;
  bool healthy_;
  bool sampleReady_;
  uint32_t lastUpdateMs_;
  RPLidarA1Sample sample_;
  Purik::SensorDescriptor descriptor_;
};
