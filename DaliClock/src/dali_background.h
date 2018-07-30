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

#ifndef _DALI_BACKGROUND_H_
#define _DALI_BACKGROUND_H_

class DaliGrid {
  public:
    static void fill_gradient(uint32_t top_color, uint32_t bottom_color);
    static void draw(uint8_t line_width, float motion);
};

class DaliShine {
  public:
    static void fill_gradient(uint32_t color);
    static void draw(float motion);  
};

class DaliStarburst {
  public:
    static void fill_gradient(uint32_t color);
    static void draw(uint32_t x, uint32_t y, float motion);  
};

#endif // _DALI_BACKGROUND_H_