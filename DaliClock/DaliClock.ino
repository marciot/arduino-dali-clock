/****************************************************************************
 *   DaliClock by (c) 2018 Marcio Teixeira                               *
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

#include "src/ui_lib/ui_toolbox.h"

#include "src/dali_clock.h"
#include "src/dali_background.h"

const uint32_t DaliClock::shadow_color = 0x0044FF;

DaliGrid       grid;
DaliClock      clock;
DaliShine      shine;

/****************************** SCREEN DECLARATIONS *****************************/

class ClockScreen : public InterfaceScreen {
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t what);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};

SCREEN_TABLE {
  DECL_SCREEN(ClockScreen)
};
SCREEN_TABLE_POST

/***************************** CLOCK SCREEN *****************************/

void ClockScreen::onEntry() {
  // Draw and capture gradient bitmaps
  clock.gradient_colors(0xFF0000, 0x0000FF);
  grid.gradient_colors(0x000000, 0xFF8800);
  shine.gradient_colors(0x7F7F7F);

  // Clear the screen before turning on backlight to avoid flickering.
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(0))
     .cmd(CLEAR(true,true,true))
     .cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .execute();
  delay(100);
  
  // Load the time from the real-time clock
  clock.sync_from_rtc();
  
  // Turn on the backlight
  cmd.memwrite(REG_PWM_DUTY, 128).execute();
  
  UIScreen::onEntry();
}

void ClockScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(0x000000))
     .cmd(CLEAR(true,true,true));

  const uint32_t ms = millis();
  const float period_1s   = float(ms %  1000)  / 1000;
  const float period_3s   = float(ms %   3000) / 1000;

  grid.draw (0.008 * Vsize, period_1s);

  constexpr uint16_t margin = float(Hsize)*0.0125;
  constexpr uint16_t    top = float(Vsize)*0.333;

  clock.draw(margin, top, Hsize - margin*2, Vsize - top);

  if(period_3s < 1) shine.draw(period_3s);
}

bool ClockScreen::onTouchStart(uint8_t tag) {
  switch(tag) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      clock.start_adjusting_digit(tag);
      return true;
    case 9:
      clock.set_calender_mode(true);
      return true;
  }
}

bool ClockScreen::onTouchEnd(uint8_t tag) {
  switch(tag) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      clock.stop_adjusting_digit();
      return true;
    case 9:
      clock.set_calender_mode(false);
      return true;
  }
}

void ClockScreen::onIdle() {
  onRefresh();
}

/***************************** MAIN PROGRAM *****************************/

void setup() {
  Serial.begin(9600);
  onStartup();
}

void loop() {
  onUpdate();
}
