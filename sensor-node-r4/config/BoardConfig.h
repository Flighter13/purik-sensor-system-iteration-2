#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

namespace PurikNode {

// I2C devices
constexpr uint8_t MAX_M10S_I2C_ADDRESS = 0x42;
constexpr uint8_t GY521_I2C_ADDRESS = 0x68;

// NRF24L01+PA+LNA provisional wiring. Both R4 nodes use the same pinout.
constexpr uint8_t NRF24_CE_PIN = 7;
constexpr uint8_t NRF24_CSN_PIN = 10;

// Shared RF network settings. Every Purik node shares the same channel/network,
// while nodeId inside the Purik message header identifies the sender.
constexpr uint8_t NRF24_CHANNEL = 108;
constexpr uint8_t NRF24_DATA_RATE_MBPS = 1;
constexpr uint8_t NRF24_PA_LEVEL = 3;
constexpr uint8_t NRF24_GROUND_PIPE[6] = {'P', 'R', 'K', 'G', 'S', '\0'};

// UART allocation remains intentionally unresolved. The modular design lets a
// node host one UART sensor at a time until a validated multi-UART solution is selected.
constexpr uint32_t LIDAR_BAUDRATE = 115200;
constexpr uint32_t RADAR_BOOT_BAUDRATE = 115200;
constexpr uint32_t RADAR_HIGH_BAUDRATE = 1250000;

}  // namespace PurikNode
