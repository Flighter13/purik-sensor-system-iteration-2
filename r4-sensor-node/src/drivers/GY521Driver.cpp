#include "GY521Driver.h"

namespace {
constexpr uint8_t REG_ACCEL_XOUT_H = 0x3B;
constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
constexpr uint8_t REG_WHO_AM_I = 0x75;
constexpr float ACCEL_SCALE = 9.80665f / 16384.0f;
constexpr float GYRO_SCALE = 1.0f / 131.0f;
}

GY521Driver::GY521Driver(TwoWire& wire, uint8_t address, uint16_t sensorId)
    : wire_(wire),
      address_(address),
      sensorId_(sensorId),
      present_(false),
      initialized_(false),
      healthy_(false),
      freshReading_(false),
      lastUpdateMs_(0),
      reading_{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},
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

bool GY521Driver::readBurst(uint8_t startReg, uint8_t* buffer, size_t length) {
  wire_.beginTransmission(address_);
  wire_.write(startReg);
  if (wire_.endTransmission(false) != 0) {
    return false;
  }

  const size_t received = wire_.requestFrom(static_cast<int>(address_), static_cast<int>(length));
  if (received != length) {
    return false;
  }

  for (size_t i = 0; i < length; ++i) {
    buffer[i] = wire_.read();
  }
  return true;
}

bool GY521Driver::probe() {
  uint8_t whoAmI = 0;
  const bool readable = readRegister(REG_WHO_AM_I, whoAmI);
  present_ = readable && (whoAmI == 0x68 || whoAmI == 0x69);

  if (!present_) {
    initialized_ = false;
    healthy_ = false;
    freshReading_ = false;
  }

  return present_;
}

bool GY521Driver::begin() {
  if (!probe()) {
    return false;
  }

  if (!writeRegister(REG_PWR_MGMT_1, 0x00)) {
    initialized_ = false;
    healthy_ = false;
    return false;
  }

  delay(10);
  initialized_ = true;
  healthy_ = true;
  lastUpdateMs_ = millis();
  freshReading_ = false;
  return true;
}

void GY521Driver::update() {
  if (!present_ || !initialized_) {
    return;
  }

  uint8_t raw[14];
  if (!readBurst(REG_ACCEL_XOUT_H, raw, sizeof(raw))) {
    healthy_ = false;
    return;
  }

  const int16_t ax = static_cast<int16_t>((raw[0] << 8) | raw[1]);
  const int16_t ay = static_cast<int16_t>((raw[2] << 8) | raw[3]);
  const int16_t az = static_cast<int16_t>((raw[4] << 8) | raw[5]);
  const int16_t temp = static_cast<int16_t>((raw[6] << 8) | raw[7]);
  const int16_t gx = static_cast<int16_t>((raw[8] << 8) | raw[9]);
  const int16_t gy = static_cast<int16_t>((raw[10] << 8) | raw[11]);
  const int16_t gz = static_cast<int16_t>((raw[12] << 8) | raw[13]);

  reading_.accelXMps2 = ax * ACCEL_SCALE;
  reading_.accelYMps2 = ay * ACCEL_SCALE;
  reading_.accelZMps2 = az * ACCEL_SCALE;
  reading_.gyroXDps = gx * GYRO_SCALE;
  reading_.gyroYDps = gy * GYRO_SCALE;
  reading_.gyroZDps = gz * GYRO_SCALE;
  reading_.temperatureC = (temp / 340.0f) + 36.53f;
  reading_.timestampMs = millis();

  healthy_ = true;
  lastUpdateMs_ = reading_.timestampMs;
  freshReading_ = true;
}

bool GY521Driver::healthy() const { return healthy_; }
bool GY521Driver::present() const { return present_; }
bool GY521Driver::initialized() const { return initialized_; }
uint32_t GY521Driver::lastUpdateMs() const { return lastUpdateMs_; }
uint16_t GY521Driver::sensorId() const { return sensorId_; }
Purik::SensorClass GY521Driver::sensorClass() const { return Purik::SensorClass::Imu; }
const char* GY521Driver::name() const { return descriptor_.displayName; }
const Purik::SensorDescriptor& GY521Driver::descriptor() const { return descriptor_; }
const GY521Reading& GY521Driver::reading() const { return reading_; }
bool GY521Driver::hasFreshReading() const { return freshReading_; }
void GY521Driver::clearFreshReading() { freshReading_ = false; }
