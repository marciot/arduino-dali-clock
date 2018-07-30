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

using namespace SPI;
using namespace SPI::least_significant_byte_first;

template<bulk_write_op byte_op>
const uint8_t *DaliDigit::unpack_rle_to_spi(const uint8_t *data, size_t len, size_t linestride) {
  const uint8_t *p = data;
  uint8_t mask = 0x80, byte = 0, n, out_n = 0;
  for(;;) {
    // Emit black pixels
    n = byte_op(p++);
    if(mask == 0x80)
      while(n > 7) {SPI::spi_write_8(0x00); out_n++; n -= 8;}
    while(n) {
      n--;
      if((mask >>= 1) == 0) {
        SPI::spi_write_8(byte); out_n++;
        mask = 0x80; byte = 0;
        while(n > 7) {SPI::spi_write_8(0x00); out_n++; n -= 8; }
      }
    }
    if(--len == 0) break;
    // Emit white pixels
    n = byte_op(p++);
    if(mask == 0x80)
      while(n > 7) {SPI::spi_write_8(0xFF); out_n++; n -= 8;}
    while(n) {
      n--; byte |= mask;
      if((mask >>= 1) == 0) {
        SPI::spi_write_8(byte); out_n++;
        mask = 0x80; byte = 0; 
        while(n > 7) {SPI::spi_write_8(0xFF); out_n++; n -= 8;}
      }
    }
    if(--len == 0) break;
  }
  if(mask != 0x80) {
    SPI::spi_write_8(byte); out_n++;
  }
  if(out_n < linestride) {
    for(n = linestride - out_n; n--;) {
      SPI::spi_write_8(0);
    }
  }
  return p;
}

void DaliDigit::rle_to_segment_endpoints(uint8_t rle[]) {
  if(rle[3] != 0) {
    rle[1] = rle[0] + rle[1];
    rle[2] = rle[1] + rle[2];
    rle[3] = rle[2] + rle[3];
  } else {
    rle[1] = rle[0] + rle[1];
    rle[2] = rle[0];
    rle[3] = rle[1];    
  }
}

void DaliDigit::segment_endpoints_to_rle(uint8_t rle[]) {
  if(rle[2] > rle[1]) {
    rle[3] -= rle[2];
    rle[2] -= rle[1];
    rle[1] -= rle[0];
  } else {
    rle[1]  = rle[3] - rle[0];
    rle[2]  = 0;
    rle[3]  = 0;
  }
}

void DaliDigit::blend_rle(uint8_t start[4], uint8_t final[], uint8_t blend) {
  rle_to_segment_endpoints(start);
  rle_to_segment_endpoints(final);
  for(uint8_t i = 0; i < 4; i++) {
    final[i] = ((256-blend) * start[i] + blend * final[i]) / 256;
  }
  segment_endpoints_to_rle(final);
}

void DaliDigit::load_digit(uint32_t addr, const dali_digit_t &d1, const dali_digit_t &d2, uint8_t blend) {
  const uint16_t height     = max(pgm_read_word(&d1.height),     pgm_read_word(&d2.height));
  const uint8_t linestride  = max(pgm_read_byte(&d1.linestride), pgm_read_byte(&d2.linestride));
  const uint8_t *p1         = pgm_read_word(&d1.packed_rle);
  const uint8_t *p2         = pgm_read_word(&d2.packed_rle);
  
  SPI::spi_ftdi_select();
  CLCD::spi_write_addr(addr);
  for(uint16_t y = 0; y < height; y++) {
    uint8_t start[4], final[4];
    for(uint8_t i = 0; i < 4; i++) {
      start[i] = pgm_read_byte(&p1[i]);
      final[i] = pgm_read_byte(&p2[i]);
    }
    blend_rle(start, final, blend);
    unpack_rle_to_spi<ram_write>(final, 4, linestride);
    p1 += 4;
    p2 += 4;
  }
  SPI::spi_ftdi_deselect();
}

void DaliDigit::load_digit(uint32_t addr, uint8_t d1, uint8_t d2, uint8_t blend) {
  constexpr dali_digit_t *digits[] = {
    &DIGIT_INFO(zero),
    &DIGIT_INFO(one),
    &DIGIT_INFO(two),
    &DIGIT_INFO(three),
    &DIGIT_INFO(four),
    &DIGIT_INFO(five),
    &DIGIT_INFO(six),
    &DIGIT_INFO(seven),
    &DIGIT_INFO(eight),
    &DIGIT_INFO(nine),
    &DIGIT_INFO(colon),
    &DIGIT_INFO(slash)
  };
  load_digit(addr, *digits[d1], *digits[d2], blend);
}

void DaliDigit::load_digit(uint32_t addr, float digit, uint8_t wrap) {
  const uint8_t f = floor(digit);
  const uint8_t c = ceil(digit);
  load_digit(addr, f, (f == wrap) ?  0 : c, min(255,(digit - f)*512));
}

void DaliDigit::load_colon(uint32_t addr) {
  load_digit(addr, COLON, DASH, 0);
}