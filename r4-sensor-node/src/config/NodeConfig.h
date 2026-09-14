#pragma once

#include <Arduino.h>

// Build the same firmware for either UNO R4 sensor node by changing PURIK_NODE_SLOT.
// Node 1: -DPURIK_NODE_SLOT=1 (default)
// Node 2: -DPURIK_NODE_SLOT=2
#ifndef PURIK_NODE_SLOT
#define PURIK_NODE_SLOT 1
#endif

namespace PurikNode {

static_assert(PURIK_NODE_SLOT == 1 || PURIK_NODE_SLOT == 2,
              "PURIK_NODE_SLOT must be 1 or 2");

#if PURIK_NODE_SLOT == 1
constexpr uint16_t NODE_ID = 1;
constexpr const char* NODE_NAME = "sensor-node-01";
#elif PURIK_NODE_SLOT == 2
constexpr uint16_t NODE_ID = 2;
constexpr const char* NODE_NAME = "sensor-node-02";
#endif

// Capability switches describe which drivers are available in this firmware image.
// Runtime discovery will later determine which supported devices are actually attached.
constexpr bool SUPPORT_RADAR = true;
constexpr bool SUPPORT_LIDAR = true;
constexpr bool SUPPORT_GNSS = true;
constexpr bool SUPPORT_IMU = true;
constexpr bool SUPPORT_NRF24 = true;
constexpr bool SUPPORT_WIFI = true;

// Start conservatively while the hardware interfaces are brought up.
constexpr bool AUTO_PROBE_SENSORS = true;
constexpr uint32_t NODE_ANNOUNCE_INTERVAL_MS = 2000;
constexpr uint32_t NODE_HEARTBEAT_INTERVAL_MS = 1000;
constexpr uint32_t SENSOR_PROBE_INTERVAL_MS = 3000;

}  // namespace PurikNode
