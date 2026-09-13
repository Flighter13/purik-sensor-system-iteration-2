#pragma once

#include <Arduino.h>
#include "PurikSensorTypes.h"
#include "PurikVersion.h"

namespace Purik {

struct MessageHeader {
  uint8_t protocolMajor;
  uint8_t protocolMinor;
  uint16_t nodeId;
  uint16_t sensorId;
  SensorClass sensorClass;
  MessageType messageType;
  uint8_t schemaVersion;
  uint8_t flags;
  uint32_t sequence;
  uint32_t timestampMs;
  uint16_t payloadLength;
};

inline MessageHeader makeHeader(
    uint16_t nodeId,
    uint16_t sensorId,
    SensorClass sensorClass,
    MessageType messageType,
    uint8_t schemaVersion,
    uint32_t sequence,
    uint32_t timestampMs,
    uint16_t payloadLength) {
  MessageHeader header{};
  header.protocolMajor = PROTOCOL_VERSION_MAJOR;
  header.protocolMinor = PROTOCOL_VERSION_MINOR;
  header.nodeId = nodeId;
  header.sensorId = sensorId;
  header.sensorClass = sensorClass;
  header.messageType = messageType;
  header.schemaVersion = schemaVersion;
  header.flags = 0;
  header.sequence = sequence;
  header.timestampMs = timestampMs;
  header.payloadLength = payloadLength;
  return header;
}

}  // namespace Purik
