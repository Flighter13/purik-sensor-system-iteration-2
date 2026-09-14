#include "SensorRegistry.h"

SensorRegistry::SensorRegistry() : drivers_{nullptr}, count_(0) {}

bool SensorRegistry::registerDriver(SensorDriver& driver) {
  if (count_ >= MAX_SENSORS) {
    return false;
  }

  for (size_t i = 0; i < count_; ++i) {
    if (drivers_[i] != nullptr && drivers_[i]->sensorId() == driver.sensorId()) {
      return false;
    }
  }

  drivers_[count_++] = &driver;
  return true;
}

void SensorRegistry::beginAll() {
  for (size_t i = 0; i < count_; ++i) {
    if (drivers_[i] != nullptr && drivers_[i]->present()) {
      drivers_[i]->begin();
    }
  }
}

void SensorRegistry::probeAll() {
  for (size_t i = 0; i < count_; ++i) {
    if (drivers_[i] != nullptr) {
      drivers_[i]->probe();
    }
  }
}

void SensorRegistry::updateAll() {
  for (size_t i = 0; i < count_; ++i) {
    if (drivers_[i] != nullptr && drivers_[i]->present() && drivers_[i]->initialized()) {
      drivers_[i]->update();
    }
  }
}

size_t SensorRegistry::count() const {
  return count_;
}

size_t SensorRegistry::presentCount() const {
  size_t present = 0;

  for (size_t i = 0; i < count_; ++i) {
    if (drivers_[i] != nullptr && drivers_[i]->present()) {
      ++present;
    }
  }

  return present;
}

SensorDriver* SensorRegistry::driverAt(size_t index) const {
  if (index >= count_) {
    return nullptr;
  }

  return drivers_[index];
}

SensorDriver* SensorRegistry::findById(uint16_t sensorId) const {
  for (size_t i = 0; i < count_; ++i) {
    if (drivers_[i] != nullptr && drivers_[i]->sensorId() == sensorId) {
      return drivers_[i];
    }
  }

  return nullptr;
}

SensorDriver* SensorRegistry::findByClass(Purik::SensorClass sensorClass) const {
  for (size_t i = 0; i < count_; ++i) {
    if (drivers_[i] != nullptr && drivers_[i]->sensorClass() == sensorClass) {
      return drivers_[i];
    }
  }

  return nullptr;
}

void SensorRegistry::printSummary(Stream& out) const {
  out.print("Registered drivers: ");
  out.println(count_);

  for (size_t i = 0; i < count_; ++i) {
    const SensorDriver* driver = drivers_[i];

    if (driver == nullptr) {
      continue;
    }

    out.print("  [");
    out.print(driver->sensorId());
    out.print("] ");
    out.print(driver->name());
    out.print(" present=");
    out.print(driver->present() ? "yes" : "no");
    out.print(" initialized=");
    out.print(driver->initialized() ? "yes" : "no");
    out.print(" healthy=");
    out.println(driver->healthy() ? "yes" : "no");
  }
}
