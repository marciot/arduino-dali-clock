/*********************
 * ftdi_eve_panels.h *
 *********************/

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

#ifndef _FTDI_EVE_PANELS_H_
#define _FTDI_EVE_PANELS_H_

#include "ui_config.h"

  #if defined(LCD_VM800B35A)
    #define USE_FTDI_FT800
    #define LCD_320x240

    namespace FTDI {
      const uint16_t Hsize               =   320;
      const uint16_t Vsize               =   240;
      const uint16_t Vsync0              =     0;
      const uint16_t Vsync1              =     2;
      const uint16_t Voffset             =    13;
      const uint16_t Vcycle              =   263;
      const uint16_t Hsync0              =     0;
      const uint16_t Hsync1              =    10;
      const uint16_t Hoffset             =    70;
      const uint16_t Hcycle              =   408;
      const uint8_t Pclkpol              =     0;
      const uint8_t Swizzle              =     2;
      const uint8_t CSpread              =     1;
      const uint8_t Pclk                 =     8;

      const bool Use_Crystal             =  true; // 0 = use internal oscillator, 1 = module has a crystal populated
      const bool GPIO_1_Audio_Shutdown   =  true; /* 1 = does use GPIO01 for amplifier control, 0 = not in use for Audio */

      const uint16_t touch_threshold     =  1200; /* touch-sensitivity */
      const uint32_t default_transform_a =  0xffff7f55;
      const uint32_t default_transform_b =  0x00000000;
      const uint32_t default_transform_c =  0x01f0d673;
      const uint32_t default_transform_d =  0x00000084;
      const uint32_t default_transform_e =  0x000054b9;
      const uint32_t default_transform_f =  0xffe0f006;
    }

  /*
   * Settings for the Haoyu Electronics, 4.3" Graphical LCD Touchscreen,       480x272, SPI, FT800 (FT800CB-HY43B)
   *                  Haoyu Electronics,   5" Graphical LCD Touchscreen,       480x272, SPI, FT800 (FT800CB-HY50B)
   *
   *    http://www.hotmcu.com/43-graphical-lcd-touchscreen-480x272-spi-ft800-p-111.html?cPath=6_16
   *    http://www.hotmcu.com/5-graphical-lcd-touchscreen-480x272-spi-ft800-p-124.html?cPath=6_16
   *
   * Datasheet:
   *
   *    http://www.hantronix.com/files/data/1278363262430-3.pdf
   *    http://www.haoyuelectronics.com/Attachment/HY43-LCD/LCD%20DataSheet.pdf
   *    http://www.haoyuelectronics.com/Attachment/HY5-LCD-HD/KD50G21-40NT-A1.pdf
   *
   */

  #elif defined(LCD_FT800CB) || defined(LCD_FT810CB_480x272)

    #define LCD_480x272

    #if defined(LCD_FT800CB)
      #define USE_FTDI_FT800
    #else
      #define USE_FTDI_FT810
    #endif

    namespace FTDI {
      const uint16_t Hsize                =   480;
      const uint16_t Vsize                =   272;
      const uint16_t Vsync0               =     0;
      const uint16_t Vsync1               =    10;
      const uint16_t Voffset              =    12;
      const uint16_t Vcycle               =   292;
      const uint16_t Hsync0               =     0;
      const uint16_t Hsync1               =    41;
      const uint16_t Hoffset              =    43;
      const uint16_t Hcycle               =   548;
      const uint8_t Pclkpol               =     1;
      const uint8_t Swizzle               =     0;
      const uint8_t CSpread               =     1;
      const uint8_t Pclk                  =     5;

      const bool Use_Crystal              =  true; // 0 = use internal oscillator, 1 = module has a crystal populated
      const bool GPIO_1_Audio_Shutdown    = false;

      const uint16_t touch_threshold      =  2000; /* touch-sensitivity */
      const uint32_t default_transform_a  =  0x00008100;
      const uint32_t default_transform_b  =  0x00000000;
      const uint32_t default_transform_c  =  0xFFF18000;
      const uint32_t default_transform_d  =  0x00000000;
      const uint32_t default_transform_e  =  0xFFFFB100;
      const uint32_t default_transform_f  =  0x0120D000;
    }

  /*
   * Settings for the Haoyu Electronics, 5" Graphical LCD Touchscreen, 800x480, SPI, FT810
   *
   *    http://www.hotmcu.com/5-graphical-lcd-touchscreen-800x480-spi-ft810-p-286.html
   *
   * Datasheet:
   *
   *    http://www.haoyuelectronics.com/Attachment/HY5-LCD-HD/KD50G21-40NT-A1.pdf
   *
   */

  #elif defined(LCD_FT810CB)
    #define USE_FTDI_FT810
    #define LCD_800x480

    namespace FTDI {
      const uint16_t Hsize                =   800;
      const uint16_t Vsize                =   480;
      const uint16_t Vsync0               =     0;
      const uint16_t Vsync1               =     2;
      const uint16_t Voffset              =    13;
      const uint16_t Vcycle               =   525;
      const uint16_t Hsync0               =     0;
      const uint16_t Hsync1               =    20;
      const uint16_t Hoffset              =    64;
      const uint16_t Hcycle               =   952;
      const uint8_t Pclkpol               =     1;
      const uint8_t Swizzle               =     0;
      const uint8_t CSpread               =     1;
      const uint8_t Pclk                  =     2;

      const bool Use_Crystal              =  true; // 0 = use internal oscillator, 1 = module has a crystal populated
      const bool GPIO_1_Audio_Shutdown    = false;

      const uint16_t touch_threshold      =  2000; /* touch-sensitivity */
      const uint32_t default_transform_a  =  0x0000D8B9;
      const uint32_t default_transform_b  =  0x00000124;
      const uint32_t default_transform_c  =  0xFFE23926;
      const uint32_t default_transform_d  =  0xFFFFFF51;
      const uint32_t default_transform_e  =  0xFFFF7E4F;
      const uint32_t default_transform_f  =  0x01F0AF70;
    }

  /*
   * Settings for the 4D Systems,        4.3" Embedded SPI Display             480x272, SPI, FT800 (4DLCD-FT843)
   *
   *    http://www.4dsystems.com.au/product/4DLCD_FT843/
   *
   * Datasheet:
   *
   *    http://www.4dsystems.com.au/productpages/4DLCD-FT843/downloads/FT843-4.3-Display_datasheet_R_1_2.pdf
   *
   */

  #elif defined(LCD_4DLCD_FT843)
    #define USE_FTDI_FT800
    #define LCD_480x272

    namespace FTDI {
      const uint16_t Hsize                =   480;
      const uint16_t Vsize                =   272;
      const uint16_t Vsync0               =     0;
      const uint16_t Vsync1               =    10;
      const uint16_t Voffset              =    12;
      const uint16_t Vcycle               =   292;
      const uint16_t Hsync0               =     0;
      const uint16_t Hsync1               =    41;
      const uint16_t Hoffset              =    43;
      const uint16_t Hcycle               =   548;
      const uint8_t Pclkpol               =     1;
      const uint8_t Swizzle               =     0;
      const uint8_t CSpread               =     1;
      const uint8_t Pclk                  =     5;

      const bool Use_Crystal              =  true; // 0 = use internal oscillator, 1 = module has a crystal populated
      const bool GPIO_1_Audio_Shutdown    =  true;

      const uint16_t touch_threshold      =  1200; /* touch-sensitivity */
      const uint32_t default_transform_a  =  0x00008100;
      const uint32_t default_transform_b  =  0x00000000;
      const uint32_t default_transform_c  =  0xFFF18000;
      const uint32_t default_transform_d  =  0x00000000;
      const uint32_t default_transform_e  =  0xFFFFB100;
      const uint32_t default_transform_f  =  0x0120D000;
    }

  /*
   * Settings for the Aleph Objects Color LCD User Interface 5" (Prototype) 800x480, SPI, FT810
   *
   *    https://code.alephobjects.com/source/aotctl/
   *
   * Datasheet:
   *
   *    http://www.hantronix.com/files/data/s1501799605s500-gh7.pdf
   *
   */
  #elif defined(LCD_AO_CLCD)
    #define USE_FTDI_FT810
    #define LCD_800x480

    namespace FTDI {
      const uint16_t Hsize                =   800;
      const uint16_t Vsize                =   480;
      const uint16_t Vsync0               =     0;
      const uint16_t Vsync1               =     2;
      const uint16_t Voffset              =    13;
      const uint16_t Vcycle               =   525;
      const uint16_t Hsync0               =     0;
      const uint16_t Hsync1               =    20;
      const uint16_t Hoffset              =    64;
      const uint16_t Hcycle               =   952;
      const uint8_t Pclkpol               =     1;
      const uint8_t Swizzle               =     0;
      const uint8_t CSpread               =     1;
      const uint8_t Pclk                  =     2;

      const bool Use_Crystal              = false; // 0 = use internal oscillator, 1 = module has a crystal populated
      const bool GPIO_1_Audio_Shutdown    =  true;

      const uint16_t touch_threshold      =  2000; /* touch-sensitivity */
      const uint32_t default_transform_a  =  0x0000D8B9;
      const uint32_t default_transform_b  =  0x00000124;
      const uint32_t default_transform_c  =  0xFFE23926;
      const uint32_t default_transform_d  =  0xFFFFFF51;
      const uint32_t default_transform_e  =  0xFFFF7E4F;
      const uint32_t default_transform_f  =  0x01F0AF70;
    }

  #else
    #error Unknown or no panel specified. To add a new panel, modify "ftdi_eve_panels.h"
  #endif

#endif // _FTDI_EVE_PANELS_H_





