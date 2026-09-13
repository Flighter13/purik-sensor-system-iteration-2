#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "config/BoardConfig.h"
#include "config/NodeConfig.h"
#include "core/SensorRegistry.h"
#include "core/DeviceManager.h"
#include "drivers/MAXM10SDriver.h"
#include "drivers/GY521Driver.h"

namespace {

// Stable node-local sensor IDs. The node ID in each Purik message keeps sensors
// on node 1 distinct from sensors with the same local ID on node 2.
constexpr uint16_t GNSS_SENSOR_ID = 0x0301;
constexpr uint16_t IMU_SENSOR_ID = 0x0401;

SensorRegistry sensorRegistry;
MAXM10SDriver gnssDriver(Wire, PurikNode::MAX_M10S_I2C_ADDRESS, GNSS_SENSOR_ID);
GY521Driver imuDriver(Wire, PurikNode::GY521_I2C_ADDRESS, IMU_SENSOR_ID);
DeviceManager deviceManager(sensorRegistry);

uint32_t lastHeartbeatMs = 0;
uint32_t lastAnnounceMs = 0;

void announceNode() {
  Serial.print("NODE_ANNOUNCE id=");
  Serial.print(PurikNode::NODE_ID);
  Serial.print(" name=");
  Serial.print(PurikNode::NODE_NAME);
  Serial.print(" present_sensors=");
  Serial.print(sensorRegistry.presentCount());
  Serial.print(" registered_drivers=");
  Serial.println(sensorRegistry.count());
}

void emitHeartbeat() {
  Serial.print("NODE_HEARTBEAT id=");
  Serial.print(PurikNode::NODE_ID);
  Serial.print(" uptime_ms=");
  Serial.print(millis());
  Serial.print(" present_sensors=");
  Serial.println(sensorRegistry.presentCount());
}

void registerSupportedDrivers() {
  if (PurikNode::SUPPORT_GNSS) {
    sensorRegistry.registerDriver(gnssDriver);
  }

  if (PurikNode::SUPPORT_IMU) {
    sensorRegistry.registerDriver(imuDriver);
  }

  // Radar and LiDAR drivers will be registered here after their UART
  // electrical/interface arrangements are finalized.
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("Purik Sensor System - Iteration 2");
  Serial.print("Node ID: ");
  Serial.println(PurikNode::NODE_ID);
  Serial.print("Node name: ");
  Serial.println(PurikNode::NODE_NAME);
  Serial.println("Multi-node sensor registry starting...");

  Wire.begin();
  Wire.setClock(400000);
  SPI.begin();

  registerSupportedDrivers();
  deviceManager.begin();
  deviceManager.printSummary(Serial);
  announceNode();
}

void loop() {
  const uint32_t now = millis();

  deviceManager.update();

  if (now - lastHeartbeatMs >= PurikNode::NODE_HEARTBEAT_INTERVAL_MS) {
    lastHeartbeatMs = now;
    emitHeartbeat();
  }

  if (now - lastAnnounceMs >= PurikNode::NODE_ANNOUNCE_INTERVAL_MS) {
    lastAnnounceMs = now;
    announceNode();
  }

  // Next milestone:
  // 1. payload adapters convert active driver data to Purik messages
  // 2. messages are queued for nRF24 transport
  // 3. UNO Q updates its multi-node registry from node/device announcements

  delay(10);
}
