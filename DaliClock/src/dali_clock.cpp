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

#include "ui_lib/ui_toolbox.h"

#include "dali_digit.h"
#include "dali_memory_map.h"
#include "dali_clock.h"

#include "ds1302.h"

// How often we synchronize with the RTC to prevent clock drift and
// to read the calendar date after a rollover.
constexpr uint32_t rtc_sync_interval = 60*60*1000; // Every hour

void DaliClock::sync_from_rtc() {
  uint8_t h, m, s, month, day, year, weekday;
  DS1302::get(h, m, s, month, day, year, weekday);
  set_time(h, m, s);
  set_date(month, day, year);
}

void DaliClock::sync_to_rtc() {
  uint8_t h, m, s, month, day, year;
  get_time(h, m, s);
  get_date(month, day, year);
  DS1302::set(h, m, s, month, day, year, 0);
}

void DaliClock::gradient_colors(uint32_t top_color, uint32_t bottom_color) {
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR(true,true,true))
     .gradient(0, 0, top_color, 0, clock_gradient_height, bottom_color)
     .cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .snapshot2(RGB565, clock_gradient_addr, 0, 0, 1, clock_gradient_height)
     .execute();

  DaliDigit::load_digit(digit1_bits_addr, 0);
  DaliDigit::load_digit(digit2_bits_addr, 0);
  DaliDigit::load_digit(digit3_bits_addr, 0);
  DaliDigit::load_digit(digit4_bits_addr, 0);
  DaliDigit::load_digit(digit5_bits_addr, 0);
  DaliDigit::load_digit(digit6_bits_addr, 0);
  DaliDigit::load_colon(separator_bits_addr);
}

void DaliClock::set_time(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  offset = hms_to_ms(hours, minutes, seconds) - millis();
}

void DaliClock::get_time(uint8_t &hours, uint8_t &minutes, float &seconds) const {
  ms_to_hms(millis() + offset, hours, minutes, seconds);
}

void DaliClock::get_time(uint8_t &hours, uint8_t &minutes, uint8_t &seconds) const {
  float f;
  ms_to_hms(millis() + offset, hours, minutes, f);
  seconds = f;
}

/*************************** Drawing and Display Functions ****************************/

void DaliClock::load_morphed_digit_bitmaps(const float digits[6], const uint8_t digit_limit[6]) {
  constexpr uint32_t addr[6] = {
    digit1_bits_addr, digit2_bits_addr, digit3_bits_addr,
    digit4_bits_addr, digit5_bits_addr, digit6_bits_addr
  };

  // It is expensive to load the bitmaps, so only
  // update them if they are actually changing.
  for(uint8_t i = 0; i < 6; i++) {
    if(last_digit[i] != digits[i]) {
      last_digit[i] = digits[i];
      DaliDigit::load_digit(addr[i], digits[i], digit_limit[i]);
    }
  }
}

void DaliClock::load_morphed_digit_bitmaps(const uint8_t d1[6], const uint8_t d2[6], uint8_t blend) {
  constexpr uint32_t addr[6] = {
    digit1_bits_addr, digit2_bits_addr, digit3_bits_addr,
    digit4_bits_addr, digit5_bits_addr, digit6_bits_addr
  };

  for(uint8_t i = 0; i < 6; i++) {
    DaliDigit::load_digit(addr[i], d1[i], d2[i], blend);
    last_digit[i] = INFINITY;
  }
}

void DaliClock::load_morphed_separator_bitmaps(uint8_t blend) {
  if(last_separator != blend) {
    last_separator = blend;
    DaliDigit::load_digit(separator_bits_addr, DaliDigit::COLON, DaliDigit::DASH, blend);
  }
}

void DaliClock::draw_digit_bitmaps(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  constexpr uint16_t digit_gap       = 6;
  constexpr uint16_t drop_shadow     = 4;
  
  constexpr uint16_t unscaled_width  = digit_bits_width*6 + separator_bits_width*2 + digit_gap*7 + drop_shadow;
  const float scale = float(w)/unscaled_width;
  const uint16_t s_d_height      = digit_bits_height     * scale;
  const uint16_t s_d_width       = digit_bits_width      * scale;
  const uint16_t s_s_width       = separator_bits_width  * scale;
  const uint16_t s16_drop_shadow = drop_shadow           * scale * 16;
  const uint16_t s_gap           = digit_gap             * scale;

  const uint16_t digit1_x = x;
  const uint16_t digit2_x = digit1_x + s_d_width + s_gap;
  const uint16_t colon1_x = digit2_x + s_d_width + s_gap;
  const uint16_t digit3_x = colon1_x + s_s_width + s_gap;
  const uint16_t digit4_x = digit3_x + s_d_width + s_gap;
  const uint16_t colon2_x = digit4_x + s_d_width + s_gap;
  const uint16_t digit5_x = colon2_x + s_s_width + s_gap;
  const uint16_t digit6_x = digit5_x + s_d_width + s_gap;

  CommandProcessor cmd;
  cmd.cmd(VERTEX_FORMAT(0))
  
     // Lower touch region  
     .cmd(SAVE_CONTEXT())
       .tag(9)
       .cmd(COLOR_MASK(0,0,0,0))
       .cmd(BEGIN(RECTS))
       .cmd(VERTEX2F(0, y + s_d_height/2))
       .cmd(VERTEX2F(Hsize, Vsize))
       .cmd(END())
     .cmd(RESTORE_CONTEXT())

     // Draw the digit drop shadows
     .cmd(SAVE_CONTEXT())
       .cmd(BITMAP_TRANSFORM_A(256.0f / scale))
       .cmd(BITMAP_TRANSFORM_E(256.0f / scale))

       .cmd(BLEND_FUNC(BLEND_FUNC_SRC_ALPHA,BLEND_FUNC_ONE_MINUS_SRC_ALPHA))
       .cmd(COLOR_RGB(shadow_color))
       .cmd(BEGIN(BITMAPS))

       // Draw the separators drop shadows
       .cmd(BITMAP_LAYOUT(L1, separator_bits_linestride, separator_bits_height))
       .cmd(BITMAP_SIZE(NEAREST, BORDER, BORDER, s_d_width, s_d_height))
       .cmd(VERTEX_TRANSLATE_X(s16_drop_shadow))
       .cmd(VERTEX_TRANSLATE_Y(s16_drop_shadow))
       .tag(7).cmd(BITMAP_SOURCE(separator_bits_addr )).cmd(VERTEX2F(colon1_x, y))
       .tag(8).cmd(BITMAP_SOURCE(separator_bits_addr )).cmd(VERTEX2F(colon2_x, y))

       // Draw the digits drop shadows
       .cmd(BITMAP_LAYOUT(L1, digit_bits_linestride, digit_bits_height))
       .cmd(BITMAP_SIZE(NEAREST, BORDER, BORDER, s_d_width, s_d_height))
       .tag(1).cmd(BITMAP_SOURCE(digit1_bits_addr)).cmd(VERTEX2F(digit1_x, y))
       .tag(2).cmd(BITMAP_SOURCE(digit2_bits_addr)).cmd(VERTEX2F(digit2_x, y))
       .tag(3).cmd(BITMAP_SOURCE(digit3_bits_addr)).cmd(VERTEX2F(digit3_x, y))
       .tag(4).cmd(BITMAP_SOURCE(digit4_bits_addr)).cmd(VERTEX2F(digit4_x, y))
       .tag(5).cmd(BITMAP_SOURCE(digit5_bits_addr)).cmd(VERTEX2F(digit5_x, y))
       .tag(6).cmd(BITMAP_SOURCE(digit6_bits_addr)).cmd(VERTEX2F(digit6_x, y))
     .cmd(RESTORE_CONTEXT())

     // Clear the alpha buffer
     
     .cmd(SAVE_CONTEXT())
       .cmd(TAG_MASK(false))
       .cmd(BLEND_FUNC(BLEND_FUNC_ONE,BLEND_FUNC_ZERO))
       .cmd(COLOR_A(0))
       .cmd(COLOR_MASK(0,0,0,1))
       .cmd(BEGIN(RECTS))
       .cmd(VERTEX2F(0, 0))
       .cmd(VERTEX2F(Hsize, Vsize))
       .cmd(END())
     .cmd(RESTORE_CONTEXT())

     // Draw the digits
     .cmd(SAVE_CONTEXT())
       .cmd(COLOR_RGB(0xFFFFFF))
       .cmd(BLEND_FUNC(BLEND_FUNC_SRC_ALPHA,BLEND_FUNC_ONE_MINUS_SRC_ALPHA))
       .cmd(BITMAP_TRANSFORM_A(256.0f / scale))
       .cmd(BITMAP_TRANSFORM_E(256.0f / scale))
       .cmd(BEGIN(BITMAPS))

       // Draw the separators
       .cmd(BITMAP_LAYOUT(L1, separator_bits_linestride, separator_bits_height))
       .cmd(BITMAP_SIZE(NEAREST, BORDER, BORDER, s_d_width, s_d_height))
       .tag(7).cmd(BITMAP_SOURCE(separator_bits_addr )).cmd(VERTEX2F(colon1_x, y))
       .tag(8).cmd(BITMAP_SOURCE(separator_bits_addr )).cmd(VERTEX2F(colon2_x, y))

       // Draw the digits
       .cmd(BITMAP_LAYOUT(L1, digit_bits_linestride, digit_bits_height))
       .cmd(BITMAP_SIZE(NEAREST, BORDER, BORDER, s_d_width, s_d_height))
       .tag(1).cmd(BITMAP_SOURCE(digit1_bits_addr)).cmd(VERTEX2F(digit1_x, y))
       .tag(2).cmd(BITMAP_SOURCE(digit2_bits_addr)).cmd(VERTEX2F(digit2_x, y))
       .tag(3).cmd(BITMAP_SOURCE(digit3_bits_addr)).cmd(VERTEX2F(digit3_x, y))
       .tag(4).cmd(BITMAP_SOURCE(digit4_bits_addr)).cmd(VERTEX2F(digit4_x, y))
       .tag(5).cmd(BITMAP_SOURCE(digit5_bits_addr)).cmd(VERTEX2F(digit5_x, y))
       .tag(6).cmd(BITMAP_SOURCE(digit6_bits_addr)).cmd(VERTEX2F(digit6_x, y))

       .cmd(END())
     .cmd(RESTORE_CONTEXT())

      // Overlay the gradient on the clock digits
     .cmd(SAVE_CONTEXT())
       .cmd(TAG_MASK(false))
       .cmd(BITMAP_TRANSFORM_A(256 * 1))
       .cmd(BITMAP_TRANSFORM_E(256.0f * clock_gradient_height / s_d_height))
       .cmd(BLEND_FUNC(BLEND_FUNC_DST_ALPHA,BLEND_FUNC_ONE_MINUS_DST_ALPHA))
       .cmd(BITMAP_SOURCE(clock_gradient_addr))
       .cmd(BITMAP_LAYOUT  (RGB565,2, clock_gradient_height))
       .cmd(BITMAP_SIZE(BILINEAR, REPEAT, BORDER, Hsize, s_d_height))
       .cmd(BEGIN(BITMAPS))
       .cmd(VERTEX2F(0, y))
     .cmd(RESTORE_CONTEXT())
     
     .cmd(VERTEX_FORMAT(4));
}

void DaliClock::draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  uint8_t month, day, year, hours, minutes;
  float   seconds;
  get_time(hours, minutes, seconds);

  animate_calendar_mode(seconds);
  
  static uint32_t last_sync = 0;
  if(millis() - last_sync > rtc_sync_interval) {
    sync_from_rtc();
    last_sync = millis();
  }

  if(calendar_blend == 0) {
    // Time display
    float   time[6];
    uint8_t time_limits[6];
    hms_to_morphed_digits(hours, minutes, seconds, time);
    get_clock_digit_limits(hours, time_limits);
    apply_manual_adjustment(time, time_limits);
    load_morphed_digit_bitmaps(time, time_limits);
    load_morphed_separator_bitmaps(0);
  } else {
    uint8_t month, day, year;
    get_date(month, day, year);

    if(calendar_blend == 255) {
      // Calendar display
      uint8_t date_limits[6];
      float date[6];
      mdy_to_digits(month, day, year, date);
      get_calendar_digit_limits(date_limits);
      apply_manual_adjustment(date, date_limits);
      load_morphed_digit_bitmaps(date, date_limits);
      load_morphed_separator_bitmaps(255);
    } else {
      // Transition between time and calendar display
      uint8_t time[6], date[6];
      mdy_to_digits(month, day, year, date);
      hms_to_digits(hours, minutes, seconds, time);
      load_morphed_digit_bitmaps(time, date, calendar_blend);
      load_morphed_separator_bitmaps(calendar_blend);
    }
  }

  draw_digit_bitmaps(x, y, w, h);
}

/*************************** Time Conversion Functions ****************************/

void DaliClock::get_clock_digit_limits(uint8_t hours, uint8_t digit_limit[6]) {
  digit_limit[0] = 2;
  digit_limit[1] = hours > 19 ? 4 : 9;
  digit_limit[2] = 5;
  digit_limit[3] = 9;
  digit_limit[4] = 5;
  digit_limit[5] = 9;
}

void DaliClock::ms_to_hms(uint32_t ms, uint8_t &hours, uint8_t &minutes, float &seconds) {
  seconds = fmod(ms, 60000)/1000;
  minutes = (ms /   60000) % 60;
  hours   = (ms / 3600000) % 24;
}

uint32_t DaliClock::hms_to_ms(uint8_t hours, uint8_t minutes, float seconds) {
  return seconds * 1000 + minutes * 60000 + hours * 3600000;
}

void DaliClock::ms_to_digits(uint32_t ms, uint8_t digits[6]) {
  uint8_t hours, minutes;
  float seconds;
  ms_to_hms(ms, hours, minutes, seconds);
  hms_to_digits(hours, minutes, seconds, digits);
}

void DaliClock::hms_to_digits(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t digits[6]) {
  digits[0] = hours / 10;
  digits[1] = hours % 10;
  digits[2] = minutes / 10;
  digits[3] = minutes % 10;
  digits[4] = seconds / 10;
  digits[5] = seconds % 10;
}

void DaliClock::digits_to_hms(const uint8_t digits[6], uint8_t &hours, uint8_t &minutes, uint8_t &seconds) {
  hours   = 10 * digits[0] + digits[1];
  minutes = 10 * digits[2] + digits[3];
  seconds = 10 * digits[4] + digits[5];
}

// This function returns floats corresponding to morphed digits, including the
// special case of when multiple digits are being morphed during a carry.
void DaliClock::hms_to_morphed_digits(uint8_t hours, uint8_t minutes, float seconds, float digits[6]) {
  const uint8_t hours_tens   = hours   / 10;
  const uint8_t hours_ones   = hours   % 10;
  const uint8_t minutes_tens = minutes / 10;
  const uint8_t minutes_ones = minutes % 10;
  const uint8_t seconds_tens = seconds / 10;
  const uint8_t seconds_ones = uint8_t(seconds) % 10;
  const float f              = seconds - floor(seconds);

  bool carry[6];                                             // 01 23 45
  carry[5] = seconds_ones == 9                            ;  // 00:00:0X
  carry[4] = seconds_tens == 5                 && carry[5];  // 00:00:X0
  carry[3] = minutes_ones == 9                 && carry[4];  // 00:0X:00
  carry[2] = minutes_tens == 5                 && carry[3];  // 00:X0:00
  carry[1] = (hours_ones  == 9 || hours == 24) && carry[2];  // 0X:00:00

  // Hours
  digits[0] = hours_tens   + (carry[1] ? f : 0);
  digits[1] = hours_ones   + (carry[2] ? f : 0);
  // Minutes
  digits[2] = minutes_tens + (carry[3] ? f : 0);
  digits[3] = minutes_ones + (carry[4] ? f : 0);
  // Seconds
  digits[4] = seconds_tens + (carry[5] ? f : 0);
  digits[5] = seconds_ones +             f     ;
}

uint32_t DaliClock::digits_to_ms(const uint8_t digits[6]) {
  uint8_t hours, minutes, seconds;
  hours   = digits[0] * 10 + digits[1];
  minutes = digits[2] * 10 + digits[3];
  seconds = digits[4] * 10 + digits[5];
  return hms_to_ms(hours, minutes, seconds);
}

/*************************** Calendar Display Mode ****************************/

void DaliClock::get_calendar_digit_limits(uint8_t digit_limit[6]) {
  digit_limit[0] = 1;
  digit_limit[1] = 9;
  digit_limit[2] = 3;
  digit_limit[3] = 9;
  digit_limit[4] = 9;
  digit_limit[5] = 9;
}

void DaliClock::set_date(uint8_t month, uint8_t day, uint8_t year) {
  calendar_m = month;
  calendar_d = day;
  calendar_y = year;
}

void DaliClock::get_date(uint8_t &month, uint8_t &day, uint8_t &year) const {
  month = calendar_m;
  day   = calendar_d;
  year  = calendar_y;
}

void DaliClock::mdy_to_digits(uint8_t month, uint8_t day, uint8_t year, uint8_t digits[6]) {
  digits[0] = month / 10;
  digits[1] = month % 10;
  digits[2] = day   / 10;
  digits[3] = day   % 10;
  digits[4] = year  / 10;
  digits[5] = year  % 10;
}

void DaliClock::mdy_to_digits(uint8_t month, uint8_t day, uint8_t year, float digits[6]) {
  digits[0] = month / 10;
  digits[1] = month % 10;
  digits[2] = day   / 10;
  digits[3] = day   % 10;
  digits[4] = year  / 10;
  digits[5] = year  % 10;
}

void DaliClock::digits_to_mdy(const uint8_t digits[6], uint8_t &month, uint8_t &day, uint8_t &year) {
  month   = 10 * digits[0] + digits[1];
  day     = 10 * digits[2] + digits[3];
  year    = 10 * digits[4] + digits[5];
}

void DaliClock::set_calender_mode(bool enable) {
  calendar_requested = enable;
}

void DaliClock::animate_calendar_mode(float seconds) {
  constexpr uint8_t blend_speed         = 64;
  constexpr uint32_t calendar_hold_time = 5000;

  if(calendar_requested) {
    if(calendar_blend == 0) {
      const float f = seconds - floor(seconds);
      if(f < 0.1) {
        calendar_blend += blend_speed;
        calendar_entry_ms = millis();
      }
    } else {
      if(calendar_blend < 255)
        calendar_blend = min(255, int16_t(calendar_blend) + blend_speed);
    }
  } else {
    if(calendar_blend == 255) {
      const float f = seconds - floor(seconds);
      if(f < 0.1 && (millis() - calendar_entry_ms) > calendar_hold_time)
        calendar_blend -= blend_speed;
    } else {
      if(calendar_blend > 0)
        calendar_blend = max(0, int16_t(calendar_blend) - blend_speed);
    }
  }
}

/****************************** Manual adjustment *****************************/

void DaliClock::set_digit(uint8_t digit, uint8_t value) {
  uint8_t digits[6];
  if(calendar_blend == 0) {
    // Time display
    const uint32_t base = millis();
    ms_to_digits(base + offset, digits);
    digits[digit-1] = value;
    offset = digits_to_ms(digits) - base;
  } else {
    // Calendar display
    mdy_to_digits(calendar_m, calendar_d, calendar_y, digits);
    digits[digit-1] = value;
    digits_to_mdy(digits, calendar_m, calendar_d, calendar_y);
  }
}

void DaliClock::apply_manual_adjustment(float digits[6], const uint8_t digit_limits[6]) {
  const uint8_t digit = abs(manually_adjusted_digit);

  if(manually_adjusted_digit != 0) {
    manually_adjusted_value += 0.1;
    manually_adjusted_value = fmod(manually_adjusted_value, digit_limits[digit-1] + 1);
    digits[digit-1] = manually_adjusted_value;

    // If gliding, stop when we have reached next value
    const float overshoot = manually_adjusted_value - floor(manually_adjusted_value);
    if(manually_adjusted_digit < 0 && overshoot < 0.11) {
      // Lock the adjusted value into place
      set_digit(digit, manually_adjusted_value);
      digits[digit-1] = floor(manually_adjusted_value);
      // Stop adjusting
      manually_adjusted_digit = 0;
      // Adjust the time chip
      sync_to_rtc();
    }
  }
}

void DaliClock::start_adjusting_digit(uint8_t tag) {
  uint8_t digits[6];
  if(calendar_blend == 0) {
    ms_to_digits(millis() + offset, digits);
  } else {
    mdy_to_digits(calendar_m, calendar_d, calendar_y, digits);
  }
  manually_adjusted_digit = tag;
  manually_adjusted_value = digits[tag-1];

  if(calendar_blend == 255) {
    // When we start adjusting a calendar digit,
    // reset the timeout
    calendar_entry_ms = millis();
  }
}

void DaliClock::stop_adjusting_digit() {
  // Negate to indicate digit will glide to next value and stop
  manually_adjusted_digit *= -1;
}