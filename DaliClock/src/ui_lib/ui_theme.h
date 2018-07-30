/**************
 * ui_theme.h *
 **************/

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

#ifndef _UI_THEME_
#define _UI_THEME_

/************************************ MENU THEME ********************************/

namespace Theme {
  const uint32_t theme_darkest = 0x2A2F0A;
  const uint32_t theme_dark    = 0x495212;
  const uint32_t theme_light   = 0x8C9D22;

  const uint32_t background    = theme_dark;
  const uint32_t back_btn      = theme_light;

  const uint32_t x_axis        = 0x500000;
  const uint32_t y_axis        = 0x005000;
  const uint32_t z_axis        = 0x000050;
  const uint32_t e_axis        = 0x000000;
  const uint32_t feedrate      = 0x000000;
  const uint32_t other         = 0x000000;

  const uint32_t toggle_on     = theme_light;
  const uint32_t toggle_off    = theme_darkest;

  // Files screens

  const uint32_t files_selected = theme_light;

  // Adjustment Screens

  const uint32_t incr_btn      = theme_darkest;

  // Status screen
  const uint32_t status_bg     = 0x707070;
  const uint32_t status_dark   = 0x404040;
  const uint32_t stop_btn      = 0xF02020;
  const uint32_t prnt_btn      = 0x20D020;
  const uint32_t progress      = 0x404040;
  const uint32_t status_msg    = 0x404040;
  const uint32_t fan_speed     = 0x6060FF;
  const uint32_t temp          = 0xFF0000;
  const uint32_t axis_label    = 0x404040;

  // Calibration Registers Screen
  const uint32_t transformA    = 0x3010D0;
  const uint32_t transformB    = 0x4010D0;
  const uint32_t transformC    = 0x5010D0;
  const uint32_t transformD    = 0x6010D0;
  const uint32_t transformE    = 0x7010D0;
  const uint32_t transformF    = 0x8010D0;
  const uint32_t transformVal  = 0x104010;

  // Disabled vs enabled buttons
  struct default_btn {
    static const uint32_t rgb_enabled  = 0xFFFFFF;
    static const uint32_t fg_enabled   = theme_darkest;
    static const uint32_t rgb_disabled = theme_dark;
    static const uint32_t fg_disabled  = theme_dark;
  };

  struct light_btn {
    static const uint32_t rgb_enabled  = 0xFFFFFF;
    static const uint32_t fg_enabled   = theme_light;
    static const uint32_t rgb_disabled = theme_dark;
    static const uint32_t fg_disabled  = theme_dark;
  };

  struct red_btn {
    static const uint32_t rgb_enabled  = 0xFFFFFF;
    static const uint32_t fg_enabled   = 0xF02020;
    static const uint32_t rgb_disabled = status_bg;
    static const uint32_t fg_disabled  = status_bg;
  };

#if defined(LCD_800x480)
  #if defined(USE_PORTRAIT_ORIENTATION)
    const int16_t  font_small    = 29;
    const int16_t  font_medium   = 30;
    const int16_t  font_large    = 30;
    const int16_t  font_xlarge   = 31;
  #else
    const int16_t  font_small    = 30;
    const int16_t  font_medium   = 30;
    const int16_t  font_large    = 31;
    const int16_t  font_xlarge   = 31;
  #endif
  const float      icon_scale    = 1.0;
#elif defined(LCD_480x272)
  #if defined(USE_PORTRAIT_ORIENTATION)
  const int16_t  font_small    = 26;
  const int16_t  font_medium   = 27;
  const int16_t  font_large    = 28;
  const int16_t  font_xlarge   = 29;
  const float    icon_scale    = 0.7;
  #else
  const int16_t  font_small    = 27;
  const int16_t  font_medium   = 28;
  const int16_t  font_large    = 30;
  const int16_t  font_xlarge   = 31;
  const float    icon_scale    = 0.6;
  #endif
#elif defined(LCD_320x240)
  #if defined(USE_PORTRAIT_ORIENTATION)
  const int16_t  font_small    = 26;
  const int16_t  font_medium   = 27;
  const int16_t  font_large    = 27;
  const int16_t  font_xlarge   = 28;
  const float    icon_scale    = 0.6;
  #else
  const int16_t  font_small    = 26;
  const int16_t  font_medium   = 27;
  const int16_t  font_large    = 29;
  const int16_t  font_xlarge   = 30;
  const float    icon_scale    = 0.5;
  #endif
#endif

  const FTDI::effect_t press_sound   = FTDI::CHACK;
  const FTDI::effect_t repeat_sound  = FTDI::CHACK;
  const FTDI::effect_t unpress_sound = FTDI::POP;

/*************************** COLOR SCALES **********************************/

  const FTDI::rgb_t cool_rgb (  0,   0,   0);
  const FTDI::rgb_t low_rgb  (128,   0,   0);
  const FTDI::rgb_t med_rgb  (255, 128,   0);
  const FTDI::rgb_t high_rgb (255, 255, 128);

  uint32_t getWarmColor(uint16_t temp, uint16_t cool = 40, uint16_t low = 100, uint16_t high = 140);

};
#endif // _UI_THEME_