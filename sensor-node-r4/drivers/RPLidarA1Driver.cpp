#include "RPLidarA1Driver.h"

RPLidarA1Driver::RPLidarA1Driver(uint16_t sensorId)
    : sensorId_(sensorId),
      present_(false),
      initialized_(false),
      healthy_(false),
      sampleReady_(false),
      lastUpdateMs_(0),
      descriptor_{sensorId,
                  Purik::SensorClass::Lidar,
                  "slamtec.rplidar_a1",
                  "Slamtec RPLIDAR A1M8",
                  "lidar.sample.v1",
                  1} {}

bool RPLidarA1Driver::probe() {
  present_ = false;
  initialized_ = false;
  healthy_ = false;
  return false;
}

bool RPLidarA1Driver::begin() { return false; }
void RPLidarA1Driver::update() {}

bool RPLidarA1Driver::healthy() const { return healthy_; }
bool RPLidarA1Driver::present() const { return present_; }
bool RPLidarA1Driver::initialized() const { return initialized_; }
uint32_t RPLidarA1Driver::lastUpdateMs() const { return lastUpdateMs_; }
uint16_t RPLidarA1Driver::sensorId() const { return sensorId_; }
Purik::SensorClass RPLidarA1Driver::sensorClass() const { return Purik::SensorClass::Lidar; }
const char* RPLidarA1Driver::name() const { return descriptor_.displayName; }
const Purik::SensorDescriptor& RPLidarA1Driver::descriptor() const { return descriptor_; }
bool RPLidarA1Driver::hasSample() const { return sampleReady_; }
const RPLidarA1Sample& RPLidarA1Driver::sample() const { return sample_; }
void RPLidarA1Driver::clearSample() { sampleReady_ = false; }
