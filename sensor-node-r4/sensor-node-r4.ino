#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "config/BoardConfig.h"
#include "config/NodeConfig.h"

uint32_t lastHeartbeatMs = 0;
uint32_t lastAnnounceMs = 0;
uint32_t lastProbeMs = 0;

static void announceNode() {
  Serial.print("NODE_ANNOUNCE id=");
  Serial.print(PurikNode::NODE_ID);
  Serial.print(" name=");
  Serial.print(PurikNode::NODE_NAME);
  Serial.print(" radar=");
  Serial.print(PurikNode::SUPPORT_RADAR);
  Serial.print(" lidar=");
  Serial.print(PurikNode::SUPPORT_LIDAR);
  Serial.print(" gnss=");
  Serial.print(PurikNode::SUPPORT_GNSS);
  Serial.print(" imu=");
  Serial.println(PurikNode::SUPPORT_IMU);
}

static void emitHeartbeat() {
  Serial.print("NODE_HEARTBEAT id=");
  Serial.print(PurikNode::NODE_ID);
  Serial.print(" uptime_ms=");
  Serial.println(millis());
}

static void probeSensors() {
  // Driver-backed discovery will replace these placeholders.
  // The important contract is that both R4 boards run identical logic and
  // report the devices actually attached to that particular node.
  Serial.print("SENSOR_PROBE node=");
  Serial.println(PurikNode::NODE_ID);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("Purik Sensor System - Iteration 2");
  Serial.print("Node ID: ");
  Serial.println(PurikNode::NODE_ID);
  Serial.print("Node name: ");
  Serial.println(PurikNode::NODE_NAME);
  Serial.println("Multi-node sensor framework starting...");

  Wire.begin();
  SPI.begin();

  announceNode();
}

void loop() {
  const uint32_t now = millis();

  if (now - lastHeartbeatMs >= PurikNode::NODE_HEARTBEAT_INTERVAL_MS) {
    lastHeartbeatMs = now;
    emitHeartbeat();
  }

  if (now - lastAnnounceMs >= PurikNode::NODE_ANNOUNCE_INTERVAL_MS) {
    lastAnnounceMs = now;
    announceNode();
  }

  if (PurikNode::AUTO_PROBE_SENSORS &&
      now - lastProbeMs >= PurikNode::SENSOR_PROBE_INTERVAL_MS) {
    lastProbeMs = now;
    probeSensors();
  }

  // Future flow:
  // 1. probe supported buses and register attached sensors
  // 2. service all active drivers
  // 3. convert native samples through payload adapters
  // 4. enqueue Purik messages containing this node's NODE_ID
  // 5. transmit over nRF24 and optionally Wi-Fi

  delay(10);
}
