# Hardware Connections — Baseline

This file records the intended bus assignments without locking in unresolved UART details.

## UNO R4 WiFi sensor node

| Device | Interface | Status |
|---|---|---|
| MAX-M10S GNSS | I2C | Planned |
| GY-521 / MPU-6050 | I2C | Planned |
| nRF24L01+PA+LNA | SPI | Planned |
| RPLIDAR A1M8 | UART | UART allocation TBD |
| IWRL6432BOOST | UART | UART allocation TBD |

## Important unresolved item

The sensor node needs two reliable serial interfaces for the A1M8 and IWRL6432BOOST. Do not assume SoftwareSerial for the radar, especially at high baud rates. Final UART pin/peripheral mapping must be confirmed before implementing those two drivers.

## NRF24 baseline

Initial proposed R4 mapping:

- CE: D7
- CSN: D10
- SCK/MOSI/MISO: hardware SPI

These pins remain provisional until physical wiring is reviewed.

## Power notes

- nRF24L01+PA+LNA should use a stable regulated 3.3 V rail with local bulk and ceramic decoupling.
- Logic levels for the IWRL6432BOOST and A1M8 UART connections must be verified before direct connection to the 5 V UNO R4 logic domain.
- Sensor power and interface-level compatibility are separate concerns and should both be documented.
