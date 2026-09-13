#pragma once

#include "../../common/PurikMessages.h"
#include "../drivers/GY521Driver.h"

class ImuAdapter {
 public:
  explicit ImuAdapter(GY521Driver& driver) : driver_(driver) {}

  bool available() const {
    return driver_.present() && driver_.initialized() && driver_.healthy() && driver_.hasFreshReading();
  }

  bool build(Purik::ImuSamplePayloadV1& payload) {
    if (!available()) {
      return false;
    }

    const GY521Reading& reading = driver_.reading();
    payload.accelXMps2 = reading.accelXMps2;
    payload.accelYMps2 = reading.accelYMps2;
    payload.accelZMps2 = reading.accelZMps2;
    payload.gyroXDps = reading.gyroXDps;
    payload.gyroYDps = reading.gyroYDps;
    payload.gyroZDps = reading.gyroZDps;
    payload.temperatureC = reading.temperatureC;

    driver_.clearFreshReading();
    return true;
  }

 private:
  GY521Driver& driver_;
};
