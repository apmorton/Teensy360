#ifndef PINS_H_
#define PINS_H_

#include <stdint.h>
#include <core_pins.h>

/*
 * +========+==========+=================+
 * | Teensy | Xbox     | Purpose         |
 * +========+==========+=================+
 * | 0      | J2B1.2   | Xbox TX         |
 * | 1      | J2B1.1   | Xbox RX         |
 * | 2      | J2B1.6   | South Bridge XX |
 * | 3      | J2B1.5   | South Bridge EJ |
 * | 4      | J2B1.11  | Xbox Kiosk      |
 * | 10     | J1D2.2   | SPI Select      |
 * | 11     | J1D2.1   | SPI Data Out    |
 * | 12     | J1D2.4   | SPI Data In     |
 * | 13     | J1D2.3   | SPI Clock       |
 * | 14     | FT6U1    | Post Bit 0      |
 * | 15     | FT6U7    | Post Bit 1      |
 * | 16     | FT6U6    | Post Bit 2      |
 * | 17     | FT6U5    | Post Bit 3      |
 * | 18     | FT6U4    | Post Bit 4      |
 * | 19     | FT6U3    | Post Bit 5      |
 * | 20     | FT6U2    | Post Bit 6      |
 * | 21     | FT6U8    | Post Bit 7      |
 * | Prgm   | R5V2     | Teensy Program  |
 * +--------+----------+-----------------+
 */

const static uint8_t POST_BIT0 = 14; // A0
const static uint8_t POST_BIT1 = 15; // A1
const static uint8_t POST_BIT2 = 16; // A2
const static uint8_t POST_BIT3 = 17; // A3
const static uint8_t POST_BIT4 = 18; // A4
const static uint8_t POST_BIT5 = 19; // A5
const static uint8_t POST_BIT6 = 20; // A6
const static uint8_t POST_BIT7 = 21; // A7

const static uint8_t XBSB_XX = 2;
const static uint8_t XBSB_EJ = 3;
const static uint8_t XBOX_KIOSK = 4;

#endif
