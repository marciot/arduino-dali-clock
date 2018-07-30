/****************************************************************************
 *   DaliFontEncoder by (c) 2018 Marcio Teixeira                               *
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

//#define FIRST_HALF

#ifdef FIRST_HALF
#include "src/dali_font_C3/zeroC3.h"
#include "src/dali_font_C3/oneC3.h"
#include "src/dali_font_C3/twoC3.h"
#include "src/dali_font_C3/threeC3.h"
#include "src/dali_font_C3/fourC3.h"
#include "src/dali_font_C3/fiveC3.h"
#else
#include "src/dali_font_C3/sixC3.h"
#include "src/dali_font_C3/sevenC3.h"
#include "src/dali_font_C3/eightC3.h"
#include "src/dali_font_C3/nineC3.h"
#include "src/dali_font_C3/colonC3.h"
#include "src/dali_font_C3/slashC3.h"
#endif

size_t convert_line_to_rle(const uint8_t *data, size_t linestride, uint16_t line, uint8_t *rle_data, size_t rle_size) {
  const uint8_t* p = data + linestride * line;
  uint8_t bit_mask  = 0b00000001;
  uint8_t run_length = 0;
  uint8_t rle_bytes = 0;
  bool last_bit = false;
  while(linestride && rle_bytes < rle_size) {
    const bool bit = pgm_read_byte_near(p) & bit_mask;
    if(bit != last_bit) {
      if(rle_bytes < rle_size) {
        *rle_data++ = run_length;
      }
      rle_bytes++;
      run_length = 1;
      last_bit = bit;
    } else {
      run_length++;
    }
    bit_mask <<= 1;
    if(bit_mask == 0) {
      p++;
      bit_mask = 0b00000001;
      linestride--;
    }
  }
  
  // Finish up
  if(rle_bytes < rle_size) {
    *rle_data++ = run_length;
  }
  rle_bytes++;
      
  return rle_bytes;
}

void convert_xbm_to_packed_rle(char *fname, const uint8_t *data, uint16_t width, uint16_t height) {
  const uint8_t linestride = ceil(float(width)/8);
  
  Serial.print(F("const PROGMEM uint8_t "));
  Serial.print(fname);
  Serial.print(F("_rle[] = {"));
  for(uint16_t y = 0; y < height; y++) {
    if(y % 4 == 0) {
      Serial.println();
      Serial.print(F("    "));
    }
    uint8_t rle_data[4] = {0,0,0,0};
    convert_line_to_rle(data, linestride, y, rle_data, 4);
    Serial.print(rle_data[0]);
    Serial.print(F(", "));
    Serial.print(rle_data[1]);
    Serial.print(F(", "));
    Serial.print(rle_data[2]);
    Serial.print(F(", "));
    Serial.print(rle_data[3]);
    if(y + 1 < height) {
      Serial.print(F(",     "));
    }
  }
  Serial.println();
  Serial.println(F("};"));
  Serial.println();
  Serial.print(F("constexpr PROGMEM dali_digit_t "));
  Serial.print(fname);
  Serial.println(F("_info = {"));
  Serial.print(F("  .height = "));
  Serial.print(height);
  Serial.println(F(","));
  Serial.print(F("  .width = "));
  Serial.print(width);
  Serial.println(F(","));
  Serial.print(F("  .linestride = "));
  Serial.print(linestride);
  Serial.println(F(","));
  Serial.print(F("  .packed_rle = "));
  Serial.print(fname);
  Serial.println(F("_rle"));
  Serial.println(F("};"));
  Serial.println();
}

#define CONVERT_FONT(font) convert_xbm_to_packed_rle(#font, font##_bits, font##_width, font##_height)

void setup() {
  Serial.begin(9600);
  #ifdef FIRST_HALF
  CONVERT_FONT(zeroC3);
  CONVERT_FONT(oneC3);
  CONVERT_FONT(twoC3);
  CONVERT_FONT(threeC3);
  CONVERT_FONT(fourC3);
  CONVERT_FONT(fiveC3);
  #else
  CONVERT_FONT(sixC3);
  CONVERT_FONT(sevenC3);
  CONVERT_FONT(eightC3);
  CONVERT_FONT(nineC3);
  CONVERT_FONT(colonC3);
  CONVERT_FONT(slashC3);
  #endif
}

void loop() {
}
