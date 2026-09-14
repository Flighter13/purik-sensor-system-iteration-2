#include "MAXM10SDriver.h"

MAXM10SDriver::MAXM10SDriver(TwoWire& wire, uint8_t address, uint16_t sensorId)
    : wire_(wire),
      address_(address),
      sensorId_(sensorId),
      present_(false),
      initialized_(false),
      healthy_(false),
      freshReading_(false),
      lastUpdateMs_(0),
      descriptor_{sensorId,
                  Purik::SensorClass::Gnss,
                  "ublox.max_m10s",
                  "u-blox MAX-M10S GNSS",
                  "gnss.fix.v1",
                  1} {}

bool MAXM10SDriver::probe() {
  wire_.beginTransmission(address_);
  const uint8_t result = wire_.endTransmission();
  present_ = (result == 0);

  if (!present_) {
    initialized_ = false;
    healthy_ = false;
    freshReading_ = false;
  }

  return present_;
}

bool MAXM10SDriver::begin() {
  if (!probe()) {
    return false;
  }

  if (!gnss_.begin(wire_)) {
    initialized_ = false;
    healthy_ = false;
    return false;
  }

  gnss_.setNavigationFrequency(5);

  initialized_ = true;
  healthy_ = true;
  lastUpdateMs_ = millis();
  freshReading_ = false;
  return true;
}

void MAXM10SDriver::update() {
  if (!present_ || !initialized_) {
    return;
  }

  if (!gnss_.getPVT()) {
    return;
  }

  reading_.latitudeE7 = gnss_.getLatitude();
  reading_.longitudeE7 = gnss_.getLongitude();
  reading_.altitudeMm = gnss_.getAltitude();
  reading_.groundSpeedMmS = gnss_.getGroundSpeed();
  reading_.headingE5 = gnss_.getHeading();
  reading_.satellites = gnss_.getSIV();
  reading_.fixType = gnss_.getFixType();
  reading_.valid = gnss_.getGnssFixOk() && reading_.fixType >= 2;
  reading_.timestampMs = millis();

  healthy_ = true;
  lastUpdateMs_ = reading_.timestampMs;
  freshReading_ = true;
}

bool MAXM10SDriver::healthy() const { return healthy_; }
bool MAXM10SDriver::present() const { return present_; }
bool MAXM10SDriver::initialized() const { return initialized_; }
uint32_t MAXM10SDriver::lastUpdateMs() const { return lastUpdateMs_; }
uint16_t MAXM10SDriver::sensorId() const { return sensorId_; }
Purik::SensorClass MAXM10SDriver::sensorClass() const { return Purik::SensorClass::Gnss; }
const char* MAXM10SDriver::name() const { return descriptor_.displayName; }
const Purik::SensorDescriptor& MAXM10SDriver::descriptor() const { return descriptor_; }
const MAXM10SReading& MAXM10SDriver::reading() const { return reading_; }
bool MAXM10SDriver::hasFreshReading() const { return freshReading_; }
void MAXM10SDriver::clearFreshReading() { freshReading_ = false; }
