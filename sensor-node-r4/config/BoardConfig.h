#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

namespace PurikNode {

// Node identity
constexpr uint16_t NODE_ID = 1;

// I2C devices
constexpr uint8_t MAX_M10S_I2C_ADDRESS = 0x42;
constexpr uint8_t GY521_I2C_ADDRESS = 0x68;

// NRF24L01+PA+LNA provisional wiring
constexpr uint8_t NRF24_CE_PIN = 7;
constexpr uint8_t NRF24_CSN_PIN = 10;

// UART allocation is intentionally unresolved until the R4 serial plan is confirmed.
constexpr uint32_t LIDAR_BAUDRATE = 115200;
constexpr uint32_t RADAR_BOOT_BAUDRATE = 115200;
constexpr uint32_t RADAR_HIGH_BAUDRATE = 1250000;

}  // namespace PurikNode
