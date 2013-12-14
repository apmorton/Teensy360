/* Arduino SdSpi Library
 * Copyright (C) 2013 by William Greiman
 *
 * This file is part of the Arduino SdSpi Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SdSpi Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
 /**
 * \file
 * \brief SdSpi class for V2 SD/SDHC cards
 */
#ifndef SdSpi_h
#define SdSpi_h
#include <WProgram.h>


//------------------------------------------------------------------------------
// define default chip select pin
//
uint8_t const  SD_CHIP_SELECT_PIN = SS;


//------------------------------------------------------------------------------
/**
 * \class SdSpi
 * \brief SPI class for access to SD and SDHC flash memory cards.
 */
class SdSpi {
 public:
  /** Initialize the SPI bus */
  void begin();
  /** Set SPI options for access to SD/SDHC cards.
   * 
   * \param[in] spiDivisor SCK clock divider relative to the system clock.
   */
  void init(uint8_t spiDivisor);
  /** Receive a byte. 
   *
   * \return The byte.
   */
  uint8_t receive();
  /** Receive multiple bytes.
   *
   * \param[out] buf Buffer to receive the data.   
   * \param[in] n Number of bytes to receive.
   *
   * \return Zero for no error or nonzero error code.
   */   
  uint8_t receive(uint8_t* buf, size_t n);
  /** Send a byte.
   *
   * \param[in] data Byte to send
   */
  void send(uint8_t data);
   /** Send multiple bytes.
   *
   * \param[in] buf Buffer for data to be sent.   
   * \param[in] n Number of bytes to send.
   */   
  void send(const uint8_t* buf, size_t n);
};

#endif  // SdSpi_h

