/*********************
 * ftdi_eve_pins.h *
 *********************/

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

#ifndef _FTDI_EVE_PINS_H_
#define _FTDI_EVE_PINS_H_

#if defined(CR10_TFT)
    #ifndef USE_MARLIN_IO
        #error This display configuration cannot be used outside of Marlin.
    #endif

    #define CLCD_USE_SOFT_SPI
    #define CLCD_SOFT_SPI_SCLK  LCD_PINS_D4      // PORTA1               Pin 6
    #define CLCD_SOFT_SPI_MOSI  LCD_PINS_ENABLE  // PORTC1               Pin 8
    #define CLCD_SPI_CS         LCD_PINS_RS      // PORTA3               Pin 7
    #define CLCD_SOFT_SPI_MISO  16               // PORTC0   BTN_ENC     Pin 2
    #define CLCD_MOD_RESET      11               // PORTD3   BTN_EN1     Pin 3
    #define CLCD_AUX_0          10               // PORTD2   BTN_EN2     Pin 5
    #define CLCD_AUX_1          BEEPER_PIN       // PORTA4               Pin 1
//    #define CLCD_AUX_2          BEEPER_PIN
#endif

// The AlephObjects pinout for re-purposing the UltraLCD
// connector EXP1 for software SPI (rev B)

#if defined(AO_CLCD_PINOUT_REV_B_EXP1)
    #ifndef USE_MARLIN_IO
        #error This display configuration cannot be used outside of Marlin.
    #endif

    #define CLCD_MOD_RESET                 LCD_PINS_D4
    #define CLCD_SPI_CS                    LCD_PINS_D5

    #define CLCD_AUX_0                     LCD_PINS_ENABLE
    #define CLCD_AUX_1                     BTN_ENC
    #define CLCD_AUX_2                     BEEPER_PIN

    #define CLCD_USE_SOFT_SPI
    #define CLCD_SOFT_SPI_SCLK             LCD_PINS_D7
    #define CLCD_SOFT_SPI_MOSI             LCD_PINS_D6
    #define CLCD_SOFT_SPI_MISO             LCD_PINS_RS
#endif

// The AlephObjects pinout for re-purposing the UltraLCD
// connector EXP1 for software SPI (rev C)

#if defined(AO_CLCD_PINOUT_REV_C_EXP1)
    #ifndef USE_MARLIN_IO
        #error This display configuration cannot be used outside of Marlin.
    #endif

    #define CLCD_MOD_RESET                 LCD_PINS_ENABLE
    #define CLCD_SPI_CS                    LCD_PINS_D4

    #define CLCD_USE_SOFT_SPI
    #define CLCD_SOFT_SPI_SCLK             BTN_ENC
    #define CLCD_SOFT_SPI_MOSI             LCD_PINS_D5
    #define CLCD_SOFT_SPI_MISO             BEEPER_PIN
#endif

// The AlephObjects pinout for re-purposing the UltraLCD
// connector EXP2 for hardware SPI (rev C)

#if defined(AO_CLCD_PINOUT_REV_C_EXP2)
    #ifndef USE_MARLIN_IO
        #error This display configuration cannot be used outside of Marlin.
    #endif

    #define CLCD_SPI_CS                    BTN_EN1
    #define CLCD_MOD_RESET                 BTN_EN2
#endif

#endif // _FTDI_EVE_PINS_H_