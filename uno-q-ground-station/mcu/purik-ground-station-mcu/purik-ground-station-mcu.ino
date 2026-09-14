#include <Arduino.h>

namespace {

constexpr uint32_t STATUS_INTERVAL_MS = 1000;
uint32_t lastStatusMs = 0;

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("Purik Ground Station MCU - Iteration 2");
  Serial.println("Receiver and Linux bridge pending");
}

void loop() {
  const uint32_t now = millis();

  if (now - lastStatusMs >= STATUS_INTERVAL_MS) {
    lastStatusMs = now;
    Serial.print("PURIK_UNO_Q_MCU_READY uptime_ms=");
    Serial.println(now);
  }

  delay(10);
}
