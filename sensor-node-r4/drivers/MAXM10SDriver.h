#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_v3.h>
#include "../core/SensorDriver.h"

struct MAXM10SReading {
  int32_t latitudeE7 = 0;
  int32_t longitudeE7 = 0;
  int32_t altitudeMm = 0;
  uint32_t groundSpeedMmS = 0;
  uint32_t headingE5 = 0;
  uint8_t satellites = 0;
  uint8_t fixType = 0;
  bool valid = false;
  uint32_t timestampMs = 0;
};

class MAXM10SDriver : public SensorDriver {
 public:
  MAXM10SDriver(TwoWire& wire, uint8_t address, uint16_t sensorId);

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

  const MAXM10SReading& reading() const;
  bool hasFreshReading() const;
  void clearFreshReading();

 private:
  TwoWire& wire_;
  uint8_t address_;
  uint16_t sensorId_;
  bool present_;
  bool initialized_;
  bool healthy_;
  bool freshReading_;
  uint32_t lastUpdateMs_;
  SFE_UBLOX_GNSS gnss_;
  MAXM10SReading reading_;
  Purik::SensorDescriptor descriptor_;
};
