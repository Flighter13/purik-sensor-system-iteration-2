#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "config/BoardConfig.h"
#include "config/NodeConfig.h"

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("Purik Sensor System - Iteration 2");
  Serial.print("Node: ");
  Serial.println(PurikNode::NODE_NAME);
  Serial.println("Baseline sensor-node framework starting...");

  Wire.begin();
  SPI.begin();

  // Drivers and transports will be registered here as they are implemented.
}

void loop() {
  // Future flow:
  // 1. service all registered sensor drivers
  // 2. convert new driver data through payload adapters
  // 3. enqueue Purik messages
  // 4. transmit through the selected transport(s)
  // 5. emit node/device health information

  delay(10);
}
