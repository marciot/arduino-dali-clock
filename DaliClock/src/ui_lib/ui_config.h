/***************
 * ui_config.h *
 ***************/

/****************************************************************************
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

#ifndef _UI_CONFIG_H_
#define _UI_CONFIG_H_

// Define the display module used (see "ftdi_eve_panels.h" for definitions)

//#define LCD_VM800N35A       // FTDI 3.5" 320x240 with FT800
//#define LCD_FT800CB         // Haoyu 5" 480x272 with FT800
//#define LCD_4DLCD_FT843     // 4D Systems 4.3" 480x272 wuth FT800
//#define LCD_FT810CB_480x272 // Haoyu FT810CB fitted with a 480x272 panel (non-standard)
#define LCD_FT810CB           // Haoyu 5" 800x480 with FT810
//#define LCD_AO_CLCD           // Aleph Objects 800x480 Touch UI with FT810


// Define the printer interface or pins used (see "ftdi_eve_pins.h" for definitions):

//#define CR10_TFT
//#define AO_CLCD_PINOUT_REV_B_EXP1 // Uses UltraLCD EXP1 connector, old wiring
//#define AO_CLCD_PINOUT_REV_C_EXP1 // Uses UltraLCD EXP1 connector, new wiring
//#define AO_CLCD_PINOUT_REV_C_EXP2    // Uses UltraLCD EXP2 connector, new wiring
#define OTHER_PIN_LAYOUT

// Otherwise. Define all the pins manually:

#if defined(OTHER_PIN_LAYOUT)
    // When specifying pins:
    //   - If compiling Marlin, use Marlin pin numbers.
    //   - If compiling standalone sketch, use Arduino
    //     pin numbers or use USE_FAST_AVR_IO instead
    //    (see below for documentation).

    // The pins for CS and MOD_RESET (PD) must be chosen.
    #define CLCD_MOD_RESET                      9  // Arduino
    #define CLCD_SPI_CS                        10  // Arduino

    // If using software SPI, specify pins for SCLK, MOSI, MISO
    //#define CLCD_USE_SOFT_SPI
    #if defined(CLCD_USE_SOFT_SPI)
        //#define CLCD_SOFT_SPI_MOSI             11  // Arduino
        //#define CLCD_SOFT_SPI_MISO             12  // Arduino
        //#define CLCD_SOFT_SPI_SCLK             13  // Arduino
    #endif

    // If the following is defined, the pin definitions can be
    // given as a pairing of a port and bitmask, as opposed to
    // Arduino pin numbers, for faster sofware based I/O on
    // AVR chips, e.g:
    //
    //   #define CLCD_SPI_CS  G, 0b00001000 // PG3
    //
    //#define USE_FAST_AVR_IO
#endif

// Defines how to orient the display. An inverted (i.e. upside-down) display
// is supported on the FT800. The FT810 or better also support a portrait
// and mirrored orientation.
//#define USE_INVERTED_ORIENTATION
//#define USE_PORTRAIT_ORIENTATION
//#define USE_MIRRORED_ORIENTATION

#endif // _UI_CONFIG_H_