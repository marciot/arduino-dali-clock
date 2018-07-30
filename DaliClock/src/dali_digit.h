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

#ifndef _DALI_DIGIT_H_
#define _DALI_DIGIT_H_

struct dali_digit_t {
  uint16_t height;
  uint16_t width;
  uint8_t linestride;
  uint8_t *packed_rle;
};

//#include "dali_font_B2.h"
#include "dali_font_C3.h"

using namespace SPI;
using namespace SPI::least_significant_byte_first;

class DaliDigit {
  private:
    template<bulk_write_op byte_op>
    static const uint8_t *unpack_rle_to_spi(const uint8_t *data, size_t len, size_t linestride);

    static void rle_to_segment_endpoints(uint8_t rle[]);
    static void segment_endpoints_to_rle(uint8_t rle[]);
    static void blend_rle(uint8_t start[4], uint8_t final[], uint8_t blend);
  public:
    enum {
      COLON = 10,
      DASH  = 11
    };
    static void load_digit(uint32_t addr, const dali_digit_t &d1, const dali_digit_t &d2, uint8_t blend);
    static void load_digit(uint32_t addr, uint8_t d1, uint8_t d2, uint8_t blend);
    static void load_digit(uint32_t addr, float digit, uint8_t wrap = 9);
    static void load_colon(uint32_t addr);
};

#endif // _DALI_DIGIT_H_