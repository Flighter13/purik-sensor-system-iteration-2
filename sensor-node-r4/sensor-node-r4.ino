#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "config/BoardConfig.h"
#include "config/NodeConfig.h"
#include "core/SensorRegistry.h"
#include "core/DeviceManager.h"
#include "drivers/MAXM10SDriver.h"
#include "drivers/GY521Driver.h"
#include "adapters/GnssAdapter.h"
#include "adapters/ImuAdapter.h"

namespace {

constexpr uint16_t GNSS_SENSOR_ID = 0x0301;
constexpr uint16_t IMU_SENSOR_ID = 0x0401;

SensorRegistry sensorRegistry;
MAXM10SDriver gnssDriver(Wire, PurikNode::MAX_M10S_I2C_ADDRESS, GNSS_SENSOR_ID);
GY521Driver imuDriver(Wire, PurikNode::GY521_I2C_ADDRESS, IMU_SENSOR_ID);
DeviceManager deviceManager(sensorRegistry);

GnssAdapter gnssAdapter(gnssDriver);
ImuAdapter imuAdapter(imuDriver);

uint32_t lastHeartbeatMs = 0;
uint32_t lastAnnounceMs = 0;
uint32_t lastGnssPublishMs = 0;
uint32_t lastImuPublishMs = 0;
uint32_t messageSequence = 0;

constexpr uint32_t GNSS_PUBLISH_INTERVAL_MS = 200;
constexpr uint32_t IMU_PUBLISH_INTERVAL_MS = 50;

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

  // Radar and LiDAR will use this same registration path once their UART
  // interfaces are finalized.
}

void publishGnssIfReady(uint32_t now) {
  if (now - lastGnssPublishMs < GNSS_PUBLISH_INTERVAL_MS) {
    return;
  }

  Purik::GnssFixPayloadV1 payload{};
  if (!gnssAdapter.build(payload)) {
    return;
  }

  lastGnssPublishMs = now;
  ++messageSequence;

  Serial.print("PURIK_DATA node=");
  Serial.print(PurikNode::NODE_ID);
  Serial.print(" seq=");
  Serial.print(messageSequence);
  Serial.print(" schema=gnss.fix.v1 sensor_id=");
  Serial.print(GNSS_SENSOR_ID);
  Serial.print(" valid=");
  Serial.print(payload.valid);
  Serial.print(" sats=");
  Serial.print(payload.satellites);
  Serial.print(" lat_e7=");
  Serial.print(payload.latitudeE7);
  Serial.print(" lon_e7=");
  Serial.print(payload.longitudeE7);
  Serial.print(" alt_mm=");
  Serial.println(payload.altitudeMm);
}

void publishImuIfReady(uint32_t now) {
  if (now - lastImuPublishMs < IMU_PUBLISH_INTERVAL_MS) {
    return;
  }

  Purik::ImuSamplePayloadV1 payload{};
  if (!imuAdapter.build(payload)) {
    return;
  }

  lastImuPublishMs = now;
  ++messageSequence;

  Serial.print("PURIK_DATA node=");
  Serial.print(PurikNode::NODE_ID);
  Serial.print(" seq=");
  Serial.print(messageSequence);
  Serial.print(" schema=imu.sample.v1 sensor_id=");
  Serial.print(IMU_SENSOR_ID);
  Serial.print(" ax=");
  Serial.print(payload.accelXMps2, 3);
  Serial.print(" ay=");
  Serial.print(payload.accelYMps2, 3);
  Serial.print(" az=");
  Serial.print(payload.accelZMps2, 3);
  Serial.print(" gx=");
  Serial.print(payload.gyroXDps, 3);
  Serial.print(" gy=");
  Serial.print(payload.gyroYDps, 3);
  Serial.print(" gz=");
  Serial.println(payload.gyroZDps, 3);
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
  publishGnssIfReady(now);
  publishImuIfReady(now);

  if (now - lastHeartbeatMs >= PurikNode::NODE_HEARTBEAT_INTERVAL_MS) {
    lastHeartbeatMs = now;
    emitHeartbeat();
  }

  if (now - lastAnnounceMs >= PurikNode::NODE_ANNOUNCE_INTERVAL_MS) {
    lastAnnounceMs = now;
    announceNode();
  }

  delay(10);
}
