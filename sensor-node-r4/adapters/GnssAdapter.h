#pragma once

#include "../../common/PurikMessages.h"
#include "../drivers/MAXM10SDriver.h"

class GnssAdapter {
 public:
  explicit GnssAdapter(MAXM10SDriver& driver) : driver_(driver) {}

  bool available() const {
    return driver_.present() && driver_.initialized() && driver_.healthy() && driver_.hasFreshReading();
  }

  bool build(Purik::GnssFixPayloadV1& payload) {
    if (!available()) {
      return false;
    }

    const MAXM10SReading& reading = driver_.reading();
    payload.latitudeE7 = reading.latitudeE7;
    payload.longitudeE7 = reading.longitudeE7;
    payload.altitudeMm = reading.altitudeMm;
    payload.groundSpeedMmS = reading.groundSpeedMmS;
    payload.headingE5 = reading.headingE5;
    payload.satellites = reading.satellites;
    payload.valid = reading.valid ? 1 : 0;

    driver_.clearFreshReading();
    return true;
  }

 private:
  MAXM10SDriver& driver_;
};
