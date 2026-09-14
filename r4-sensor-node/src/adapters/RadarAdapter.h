#pragma once

#include "../../common/PurikMessages.h"
#include "../drivers/IWRL6432Driver.h"

class RadarAdapter {
 public:
  explicit RadarAdapter(IWRL6432Driver& driver) : driver_(driver) {}

  bool available() const {
    return driver_.present() && driver_.initialized() && driver_.healthy() && driver_.hasTarget();
  }

  bool build(Purik::RadarTargetV1& payload) {
    if (!available()) {
      return false;
    }

    const IWRL6432Target& target = driver_.target();
    payload.targetId = target.targetId;
    payload.rangeM = target.rangeM;
    payload.azimuthDeg = target.azimuthDeg;
    payload.elevationDeg = target.elevationDeg;
    payload.radialVelocityMps = target.radialVelocityMps;

    driver_.clearTarget();
    return true;
  }

 private:
  IWRL6432Driver& driver_;
};
