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
#include "dali_background.h"

void DaliGrid::fill_gradient(uint32_t top_color, uint32_t bottom_color) {
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR(true,true,true))
     .gradient(0, 0, top_color, 0, grid_gradient_height, bottom_color)
     .cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .snapshot2(RGB565, grid_gradient_addr, 0, 0, 1, grid_gradient_height)
     .execute();
}

void DaliGrid::draw(uint8_t line_width, float motion) {
  CommandProcessor cmd;
  cmd.cmd(SAVE_CONTEXT())
     .cmd(COLOR_RGB(0xFFFFFF))
     .cmd(LINE_WIDTH(line_width*16))
     .cmd(BEGIN(LINES));
  
  for(uint16_t y = 2; ; y++) {
    uint16_t py = pow(2, motion + y);
    if(py > Vsize/2) break;
    cmd.cmd(VERTEX2F(0,       (Vsize/2 + py)*16))
       .cmd(VERTEX2F(Hsize*16,(Vsize/2 + py)*16));
  }
  
  for(int16_t x = -int16_t(Hsize); x < int16_t(Hsize); x += 200) {
    cmd.cmd(VERTEX2F((Hsize/2+x/2)*16, (Vsize/2+line_width)*16))
       .cmd(VERTEX2F((Hsize/2+x  )*16, (Vsize  )*16));
  }
  
  // Draw the gradient
  cmd.cmd(BLEND_FUNC(BLEND_FUNC_DST_ALPHA,BLEND_FUNC_ZERO))
     .cmd(BITMAP_SOURCE(grid_gradient_addr))
     .cmd(BITMAP_LAYOUT(RGB565,2, grid_gradient_height))
     .bitmap_size(NEAREST, REPEAT, BORDER, Hsize, grid_gradient_height)
     .cmd(BEGIN(BITMAPS))
     .cmd(VERTEX2II(0, Vsize/2, 0, 0))
     .cmd(RESTORE_CONTEXT());
}

void DaliShine::fill_gradient(uint32_t color) {
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR(true,true,true))
     .gradient(0,                       0,     0, 0, shine_gradient_height/2, color)
     .cmd(SCISSOR_XY(0, shine_gradient_height/2))
     .gradient(0, shine_gradient_height/2, color, 0, shine_gradient_height,   0)
     .cmd(SCISSOR_XY())
     .cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .snapshot2(RGB565, shine_gradient_addr, 0, 0, 1, shine_gradient_height)
     .execute();
}

void DaliShine::draw(float motion) {
  constexpr uint16_t shine_length = (Vsize + shine_gradient_height) * 1.4142;
  // Draw the gradient
  CommandProcessor cmd;
  cmd.cmd(BLEND_FUNC(BLEND_FUNC_DST_ALPHA, BLEND_FUNC_ONE))
     .loadidentity()
     .rotate(45ul * 0xFFFF / 360)
     .setmatrix()
     .cmd(BITMAP_SOURCE(shine_gradient_addr))
     .cmd(BITMAP_LAYOUT  (RGB565,2, shine_gradient_height))
     .cmd(BITMAP_SIZE(NEAREST, REPEAT, BORDER, Vsize+2*shine_gradient_height, Vsize+2*shine_gradient_height))
     .cmd(BEGIN(BITMAPS))
     .cmd(VERTEX2F(((Hsize+Vsize) * motion - Vsize)*16, -2*shine_gradient_height*16))
     .loadidentity()
     .setmatrix();
}

void DaliStarburst::fill_gradient(uint32_t color) {
  const float hw = starburst_gradient_width/2;
  const float hh = starburst_gradient_height/2;
  const float aa   = hw*hw + hh*hh;
  const uint8_t x  = hw*hh*hh/aa;
  const uint8_t y  = hw*hh*hw/aa;
  
  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR(true,true,true))
     // Top-left quadrant
     .cmd(SCISSOR_XY(0, 0))
     .cmd(SCISSOR_SIZE(Hsize/2, Vsize/2))
     .gradient(Hsize/2, Vsize/2, color, Hsize/2 - x, Vsize/2 - y, 0)
     // Top-right quadrant
     .cmd(SCISSOR_XY(Hsize/2, 0))
     .gradient(Hsize/2, Vsize/2, color, Hsize/2 + x, Vsize/2 - y, 0)
     //Bottom-right quadrant
     .cmd(SCISSOR_XY(Hsize/2, Vsize/2))
     .gradient(Hsize/2, Vsize/2, color, Hsize/2 + x, Vsize/2 + y, 0)
     //Bottom-Left quadrant
     .cmd(SCISSOR_XY(0, Vsize/2))
     .gradient(Hsize/2, Vsize/2, color, Hsize/2 - x, Vsize/2 + y, 0)
     .cmd(SCISSOR_XY())
     .cmd(SCISSOR_SIZE())
     .cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .snapshot2(RGB565, starburst_gradient_addr, Hsize/2 - hw, Vsize/2 - hh, starburst_gradient_width, starburst_gradient_height)
     .execute();
}

void DaliStarburst::draw(uint32_t x, uint32_t y, float motion) {
  constexpr struct {
    uint16_t a;
    uint8_t sx;
    uint8_t sy;
  } rays[] = {
    {0,   128,  128},
    {90,  128,  128},
    {45,   96,  96},
    {135,  96,  96}
  };
  
  const int32_t sbw = starburst_gradient_width;
  const int32_t sbh = starburst_gradient_height;
  
  const float s_inv = fabs(0.5 - motion);
  const float sh    = 1.0 - 2 * s_inv;
  const float sw    = s_inv * sh;
  
  CommandProcessor cmd;
  cmd.cmd(BLEND_FUNC(BLEND_FUNC_ONE, BLEND_FUNC_ONE))
     .cmd(BITMAP_LAYOUT(RGB565,sbw*2, sbh))
     .cmd(BITMAP_SIZE(NEAREST, BORDER, BORDER, sbw, sbw))
     .cmd(BEGIN(BITMAPS));
     
     for(uint8_t i = 0; i < sizeof(rays)/sizeof(rays[0]); i++) { 
       cmd.loadidentity()
          .translate(0xFFFFl *  sbw/2, 0xFFFFl *  sbw/2)
          .rotate((rays[i].a + motion * 360 * 2) * 0xFFFFl/360 )
          .scale (rays[i].sx * 0xFFFFl/255 * sh, rays[i].sy * 0xFFFFl/255 * sw)
          .translate(0xFFFFl * -sbw/2, 0xFFFFl * -sbh/2)
          .setmatrix()
          .cmd(VERTEX2F((x-sbw/2)*16,(y-sbw/2)*16));
     }
}