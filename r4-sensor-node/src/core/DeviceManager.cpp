#include "DeviceManager.h"
#include "../config/NodeConfig.h"

DeviceManager::DeviceManager(SensorRegistry& registry)
    : registry_(registry), lastPresent_{false}, lastHealthy_{false}, lastProbeMs_(0) {}

void DeviceManager::begin() {
  forceProbe();
  reportTransitions();
}

void DeviceManager::update() {
  registry_.updateAll();

  const uint32_t now = millis();
  if (PurikNode::AUTO_PROBE_SENSORS &&
      now - lastProbeMs_ >= PurikNode::SENSOR_PROBE_INTERVAL_MS) {
    forceProbe();
  }

  reportTransitions();
}

void DeviceManager::forceProbe() {
  lastProbeMs_ = millis();
  registry_.probeAll();

  for (size_t i = 0; i < registry_.count(); ++i) {
    SensorDriver* driver = registry_.driverAt(i);
    if (driver != nullptr && driver->present() && !driver->initialized()) {
      driver->begin();
    }
  }
}

void DeviceManager::reportTransitions() {
  for (size_t i = 0; i < registry_.count(); ++i) {
    SensorDriver* driver = registry_.driverAt(i);
    if (driver == nullptr) {
      continue;
    }

    const bool present = driver->present();
    const bool healthy = driver->healthy();

    if (present != lastPresent_[i]) {
      Serial.print("DEVICE_");
      Serial.print(present ? "ATTACHED" : "DETACHED");
      Serial.print(" node=");
      Serial.print(PurikNode::NODE_ID);
      Serial.print(" sensor_id=");
      Serial.print(driver->sensorId());
      Serial.print(" driver=");
      Serial.print(driver->descriptor().driverId);
      Serial.print(" schema=");
      Serial.println(driver->descriptor().schemaName);
      lastPresent_[i] = present;
    }

    if (present && healthy != lastHealthy_[i]) {
      Serial.print("DEVICE_HEALTH node=");
      Serial.print(PurikNode::NODE_ID);
      Serial.print(" sensor_id=");
      Serial.print(driver->sensorId());
      Serial.print(" healthy=");
      Serial.println(healthy ? 1 : 0);
      lastHealthy_[i] = healthy;
    }

    if (!present) {
      lastHealthy_[i] = false;
    }
  }
}

void DeviceManager::printSummary(Stream& out) const {
  registry_.printSummary(out);
}
