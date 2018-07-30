/********************
 * ftdi_eve_spi.cpp *
 ********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "ui.h"

#if ENABLED(EXTENSIBLE_UI)

#include "ftdi_eve_panels.h"
#include "ftdi_eve_pins.h"
#include "ftdi_eve_constants.h"
#include "ftdi_eve_functions.h"
#include "ftdi_eve_spi.h"

/*************************** I/O COMPATIBILITY ******************************/

// The following functions allow this code to be used outside of Marlin (i.e.
// in Arduino sketches)

#if defined(USE_FAST_AVR_IO)
  // If the following is defined, the pin definitions can be
  // given as a pairing of port and bitmask, as opposed to
  // Arduino pin numbers, for faster I/O on AVR chips.
  //
  //    #define CLCD_SPI_CS   G, 0b00001000 // PG3 P1 Pin-3

  #define _PIN_HIGH( port, bit)       PORT##port = (PORT##port |   bit);
  #define _PIN_LOW(  port, bit)       PORT##port = (PORT##port & (~bit));

  #define _SET_INPUT(   port, bit)    DDR##port  = (DDR##port  & (~bit));
  #define _SET_OUTPUT(  port, bit)    DDR##port  = (DDR##port  |   bit);
  #define _READ(  port, bit)          (PIN##port & bit)
  #define _WRITE(  port, bit, value)  {if(value) {_PIN_HIGH(port, bit)} else {_PIN_LOW(port, bit)}}

  // This level of indirection is needed to unpack the "pin" into two arguments
  #define SET_INPUT(pin)               _SET_INPUT(pin)
  #define SET_INPUT_PULLUP(pin)        _SET_INPUT(pin); _PIN_HIGH(pin);
  #define SET_OUTPUT(pin)              _SET_OUTPUT(pin)
  #define READ(pin)                    _READ(pin)
  #define WRITE(pin, value)            _WRITE(pin, value)

#elif !defined(USE_MARLIN_IO)
  // Use standard Arduino Wire library
  #define SET_OUTPUT(p)             pinMode(p, OUTPUT);
  #define SET_INPUT_PULLUP(p)       pinMode(p, INPUT_PULLUP);
  #define SET_INPUT(p)              pinMode(p, INPUT);
  #define WRITE(p,v)                digitalWrite(p, v ? HIGH : LOW);
  #define READ(p)                   digitalRead(p)
#endif

/********************************* SPI Functions *********************************/

namespace FTDI {
  void SPI::spi_init (void) {
    SET_OUTPUT(CLCD_MOD_RESET); // Module Reset (a.k.a. PD, not SPI)
    WRITE(CLCD_MOD_RESET, 0); // start with module in power-down

    SET_OUTPUT(CLCD_SPI_CS);
    WRITE(CLCD_SPI_CS, 1);

    #if defined(SPI_FLASH_SS)
      SET_OUTPUT(SPI_FLASH_SS);
      WRITE(SPI_FLASH_SS, 1);
    #endif

    #if defined(CLCD_USE_SOFT_SPI)
      SET_OUTPUT(CLCD_SOFT_SPI_MOSI);
      WRITE(CLCD_SOFT_SPI_MOSI, 1);

      SET_OUTPUT(CLCD_SOFT_SPI_SCLK);
      WRITE(CLCD_SOFT_SPI_SCLK, 0);

      SET_INPUT_PULLUP(CLCD_SOFT_SPI_MISO);
    #elif defined(USE_MARLIN_IO)
      spiBegin();
      spiInit(SPI_SPEED);
    #endif
  }

  #if defined(CLCD_USE_SOFT_SPI)
    uint8_t SPI::_soft_spi_xfer (uint8_t spiOutByte) {
      uint8_t spiIndex  = 0x80;
      uint8_t spiInByte = 0;
      uint8_t k;

      for(k = 0; k <8; k++) {  // Output and Read each bit of spiOutByte and spiInByte
        if(spiOutByte & spiIndex) {   // Output MOSI Bit
          WRITE(CLCD_SOFT_SPI_MOSI, 1);
        }
        else {
          WRITE(CLCD_SOFT_SPI_MOSI, 0);
        }
        WRITE(CLCD_SOFT_SPI_SCLK, 1);   // Pulse Clock
        WRITE(CLCD_SOFT_SPI_SCLK, 0);

        if(READ(CLCD_SOFT_SPI_MISO)) {
          spiInByte |= spiIndex;
        }

        spiIndex >>= 1;
      }
      return spiInByte;
    }
  #endif

  #if defined(CLCD_USE_SOFT_SPI)
    void SPI::_soft_spi_send (uint8_t spiOutByte) {
      uint8_t spiIndex  = 0x80;
      uint8_t k;

      for(k = 0; k <8; k++) {         // Output each bit of spiOutByte
        if(spiOutByte & spiIndex) {   // Output MOSI Bit
          WRITE(CLCD_SOFT_SPI_MOSI, 1);
        }
        else {
          WRITE(CLCD_SOFT_SPI_MOSI, 0);
        }
        WRITE(CLCD_SOFT_SPI_SCLK, 1);   // Pulse Clock
        WRITE(CLCD_SOFT_SPI_SCLK, 0);

        spiIndex >>= 1;
      }
    }
  #endif

  void SPI::spi_read_bulk (void *data, uint16_t len) {
    uint8_t* p = (uint8_t *)data;
    #if defined(USE_MARLIN_IO) && !defined(CLCD_USE_SOFT_SPI)
      spiRead(p, len);
    #else
      while(len--) *p++ = spi_recv();
    #endif
  }

  bool SPI::spi_verify_bulk (const void *data, uint16_t len) {
    const uint8_t* p = (const uint8_t *)data;
    while(len--) if(*p++ != spi_recv()) return false;
    return true;
  }

  // CLCD SPI - Chip Select
  void SPI::spi_ftdi_select (void) {
    #if !defined(CLCD_USE_SOFT_SPI) && !defined(USE_MARLIN_IO)
      ::SPI.begin();
      ::SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    #endif
    WRITE(CLCD_SPI_CS, 0);
    delayMicroseconds(1);
  }

  // CLCD SPI - Chip Deselect
  void SPI::spi_ftdi_deselect (void) {
    WRITE(CLCD_SPI_CS, 1);
    #if !defined(CLCD_USE_SOFT_SPI) && !defined(USE_MARLIN_IO)
      ::SPI.endTransaction();
      ::SPI.end();
    #endif
  }

  #if defined(SPI_FLASH_SS)
  // Serial SPI Flash SPI - Chip Select
  void SPI::spi_flash_select () {
    WRITE(SPI_FLASH_SS, 0);
    delayMicroseconds(1);
  }

  // Serial SPI Flash SPI - Chip Deselect
  void SPI::spi_flash_deselect () {
    WRITE(SPI_FLASH_SS, 1);
  }
  #endif

  // Not really a SPI signal...
  void SPI::ftdi_reset (void) {
    WRITE(CLCD_MOD_RESET, 0);
    delay(6); /* minimum time for power-down is 5ms */
    WRITE(CLCD_MOD_RESET, 1);
    delay(21); /* minimum time to allow from rising PD_N to first access is 20ms */
  }

  // Not really a SPI signal...
  void SPI::test_pulse(void)
  {
    #if defined(CLCD_AUX_0)
      WRITE(CLCD_AUX_0, 1);
      delayMicroseconds(10);
      WRITE(CLCD_AUX_0, 0);
    #endif
  }
}
#endif // EXTENSIBLE_UI