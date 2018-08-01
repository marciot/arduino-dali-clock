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

#ifndef _DALI_CLOCK_H_
#define _DALI_CLOCK_H_

class DaliClock {
  private:
    static const uint32_t shadow_color;
    
    bool     calendar_requested;
    uint8_t  calendar_blend;
    uint32_t calendar_entry_ms;
    uint8_t  calendar_m, calendar_d, calendar_y;
    
    uint32_t offset;
    int8_t   manually_adjusted_digit;
    float    manually_adjusted_value;
    float    last_digit[6], last_separator;
    
    static void get_clock_digit_limits(uint8_t hours, uint8_t digit_limit[6]);
    static void get_calendar_digit_limits(uint8_t digit_limit[6]);
    
    static void draw_digit_bitmaps(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    static void hms_to_morphed_digits(uint8_t hours, uint8_t minutes, float seconds, float digits[6]);
    
    void load_morphed_digit_bitmaps(const float digits[6], const uint8_t digit_limit[6]);
    void load_morphed_digit_bitmaps(const uint8_t d1[6], const uint8_t d2[6], uint8_t blend);
    void load_morphed_separator_bitmaps(uint8_t blend);
    
    void apply_manual_adjustment(float digits[6], const uint8_t digit_limits[6]);
    
    void animate_calendar_mode(float seconds);
  public:
    static void gradient_colors(uint32_t top_color, uint32_t bottom_color);
    
    static void     ms_to_hms(uint32_t ms, uint8_t &hours, uint8_t &minutes, float &seconds);
    static uint32_t hms_to_ms(uint8_t hours, uint8_t minutes, float seconds);
    static void     ms_to_digits(uint32_t ms, uint8_t digits[6]);
    static void     hms_to_digits(uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t digits[6]);
    static uint32_t digits_to_ms(const uint8_t digits[6]);
    static void     digits_to_hms(const uint8_t digits[6], uint8_t &hours, uint8_t &minutes, uint8_t &seconds);
    
    static void mdy_to_digits(uint8_t month, uint8_t day, uint8_t year, uint8_t digits[6]);
    static void mdy_to_digits(uint8_t month, uint8_t day, uint8_t year, float digits[6]);
    static void digits_to_mdy(const uint8_t digits[6], uint8_t &month, uint8_t &day, uint8_t &year);
    
    void sync_from_rtc();
    void sync_to_rtc();
    
    void draw(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void set_time (uint8_t hours, uint8_t minutes, uint8_t seconds);
    void get_time(uint8_t &hours, uint8_t &minutes, float &seconds) const;
    void get_time(uint8_t &hours, uint8_t &minutes, uint8_t &seconds) const;
    void set_date(uint8_t month, uint8_t day, uint8_t year);
    void get_date(uint8_t &month, uint8_t &day, uint8_t &year) const;
    void set_digit(uint8_t digit, uint8_t value);
    
    void start_adjusting_digit(uint8_t tag);
    void stop_adjusting_digit(uint8_t &hours, uint8_t &minutes, uint8_t &seconds);
    void stop_adjusting_digit();
    void set_calender_mode(bool enable);
};

#endif // _DALI_CLOCK_H_