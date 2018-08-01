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

#ifndef _DALI_MEMORY_MAP_H_
#define _DALI_MEMORY_MAP_H_

//constexpr uint32_t  memory_end  = RAM_G + RAM_G_SIZE_800;
constexpr uint32_t memory_end = RAM_G + RAM_G_SIZE;

constexpr uint32_t clock_gradient_height = 255;
constexpr uint32_t clock_gradient_size   = clock_gradient_height*2;
constexpr uint32_t clock_gradient_addr   = memory_end - clock_gradient_size;

constexpr uint32_t grid_gradient_height = Vsize/2;
constexpr uint32_t grid_gradient_size   = grid_gradient_height*2;
constexpr uint32_t grid_gradient_addr   = clock_gradient_addr - grid_gradient_size;

constexpr uint32_t shine_gradient_height = 60;
constexpr uint32_t shine_gradient_size   = shine_gradient_height*2;
constexpr uint32_t shine_gradient_addr   = grid_gradient_addr - shine_gradient_size;

constexpr uint32_t starburst_gradient_width  = Hsize/2;
constexpr uint32_t starburst_gradient_height = 30;
constexpr uint32_t starburst_gradient_size   = starburst_gradient_height*starburst_gradient_width*2;
constexpr uint32_t starburst_gradient_addr   = shine_gradient_addr - starburst_gradient_size;

constexpr uint32_t digit_bits_linestride = DIGIT_INFO(eight).linestride;
constexpr uint32_t digit_bits_height     = DIGIT_INFO(eight).height;
constexpr uint32_t digit_bits_width      = DIGIT_INFO(eight).width;
constexpr uint32_t digit_bits_size       = digit_bits_linestride * digit_bits_height;

constexpr uint32_t digit6_bits_addr      = starburst_gradient_addr - digit_bits_size;
constexpr uint32_t digit5_bits_addr      = digit6_bits_addr - digit_bits_size;
constexpr uint32_t digit4_bits_addr      = digit5_bits_addr - digit_bits_size;
constexpr uint32_t digit3_bits_addr      = digit4_bits_addr - digit_bits_size;
constexpr uint32_t digit2_bits_addr      = digit3_bits_addr - digit_bits_size;
constexpr uint32_t digit1_bits_addr      = digit2_bits_addr - digit_bits_size;

constexpr uint32_t separator_bits_linestride = DIGIT_INFO(colon).linestride;
constexpr uint32_t separator_bits_height     = DIGIT_INFO(colon).height;
constexpr uint32_t separator_bits_width      = DIGIT_INFO(colon).width;
constexpr uint32_t separator_bits_size       = separator_bits_linestride * separator_bits_height;
constexpr uint32_t separator_bits_addr       = digit1_bits_addr - separator_bits_size;

#endif // _DALI_MEMORY_MAP_H_