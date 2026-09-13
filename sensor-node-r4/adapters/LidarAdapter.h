#pragma once

#include "../../common/PurikMessages.h"
#include "../drivers/RPLidarA1Driver.h"

class LidarAdapter {
 public:
  explicit LidarAdapter(RPLidarA1Driver& driver) : driver_(driver) {}

  bool available() const {
    return driver_.present() && driver_.initialized() && driver_.healthy() && driver_.hasSample();
  }

  bool build(Purik::LidarSamplePayloadV1& payload) {
    if (!available()) {
      return false;
    }

    const RPLidarA1Sample& sample = driver_.sample();
    payload.angleDeg = sample.angleDeg;
    payload.distanceM = sample.distanceM;
    payload.quality = sample.quality;

    driver_.clearSample();
    return true;
  }

 private:
  RPLidarA1Driver& driver_;
};
