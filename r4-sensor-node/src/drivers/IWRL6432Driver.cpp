#include "IWRL6432Driver.h"

IWRL6432Driver::IWRL6432Driver(uint16_t sensorId)
    : sensorId_(sensorId),
      present_(false),
      initialized_(false),
      healthy_(false),
      targetReady_(false),
      lastUpdateMs_(0),
      descriptor_{sensorId,
                  Purik::SensorClass::Radar,
                  "ti.iwrl6432",
                  "TI IWRL6432BOOST mmWave Radar",
                  "radar.target.v1",
                  1} {}

bool IWRL6432Driver::probe() {
  // TODO: implement after UART pins, voltage levels, and demo output mode
  // are finalized. Returning false keeps the unimplemented radar from being
  // advertised as attached.
  present_ = false;
  initialized_ = false;
  healthy_ = false;
  return false;
}

bool IWRL6432Driver::begin() {
  return false;
}

void IWRL6432Driver::update() {}

bool IWRL6432Driver::healthy() const { return healthy_; }
bool IWRL6432Driver::present() const { return present_; }
bool IWRL6432Driver::initialized() const { return initialized_; }
uint32_t IWRL6432Driver::lastUpdateMs() const { return lastUpdateMs_; }
uint16_t IWRL6432Driver::sensorId() const { return sensorId_; }
Purik::SensorClass IWRL6432Driver::sensorClass() const { return Purik::SensorClass::Radar; }
const char* IWRL6432Driver::name() const { return descriptor_.displayName; }
const Purik::SensorDescriptor& IWRL6432Driver::descriptor() const { return descriptor_; }
bool IWRL6432Driver::hasTarget() const { return targetReady_; }
const IWRL6432Target& IWRL6432Driver::target() const { return target_; }
void IWRL6432Driver::clearTarget() { targetReady_ = false; }
