#include "GY521Driver.h"

namespace {
constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
constexpr uint8_t REG_WHO_AM_I = 0x75;
}

GY521Driver::GY521Driver(TwoWire& wire, uint8_t address, uint16_t sensorId)
    : wire_(wire),
      address_(address),
      sensorId_(sensorId),
      present_(false),
      initialized_(false),
      healthy_(false),
      lastUpdateMs_(0),
      descriptor_{sensorId,
                  Purik::SensorClass::Imu,
                  "invensense.mpu6050",
                  "GY-521 / MPU-6050 IMU",
                  "imu.sample.v1",
                  1} {}

bool GY521Driver::readRegister(uint8_t reg, uint8_t& value) {
  wire_.beginTransmission(address_);
  wire_.write(reg);

  if (wire_.endTransmission(false) != 0) {
    return false;
  }

  if (wire_.requestFrom(static_cast<int>(address_), 1) != 1) {
    return false;
  }

  value = wire_.read();
  return true;
}

bool GY521Driver::writeRegister(uint8_t reg, uint8_t value) {
  wire_.beginTransmission(address_);
  wire_.write(reg);
  wire_.write(value);
  return wire_.endTransmission() == 0;
}

bool GY521Driver::probe() {
  uint8_t whoAmI = 0;
  const bool readable = readRegister(REG_WHO_AM_I, whoAmI);

  present_ = readable && (whoAmI == 0x68 || whoAmI == 0x69);

  if (!present_) {
    initialized_ = false;
    healthy_ = false;
  }

  return present_;
}

bool GY521Driver::begin() {
  if (!probe()) {
    return false;
  }

  // Wake MPU-6050 from sleep mode.
  if (!writeRegister(REG_PWR_MGMT_1, 0x00)) {
    initialized_ = false;
    healthy_ = false;
    return false;
  }

  delay(10);
  initialized_ = true;
  healthy_ = true;
  lastUpdateMs_ = millis();
  return true;
}

void GY521Driver::update() {
  if (!present_ || !initialized_) {
    return;
  }

  uint8_t whoAmI = 0;
  healthy_ = readRegister(REG_WHO_AM_I, whoAmI) &&
             (whoAmI == 0x68 || whoAmI == 0x69);

  if (healthy_) {
    lastUpdateMs_ = millis();
  }
}

bool GY521Driver::healthy() const { return healthy_; }
bool GY521Driver::present() const { return present_; }
bool GY521Driver::initialized() const { return initialized_; }
uint32_t GY521Driver::lastUpdateMs() const { return lastUpdateMs_; }
uint16_t GY521Driver::sensorId() const { return sensorId_; }
Purik::SensorClass GY521Driver::sensorClass() const { return Purik::SensorClass::Imu; }
const char* GY521Driver::name() const { return descriptor_.displayName; }
const Purik::SensorDescriptor& GY521Driver::descriptor() const { return descriptor_; }
