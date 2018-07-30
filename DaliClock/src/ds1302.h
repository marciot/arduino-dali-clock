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

#ifndef DS1302_H
#define DS1302_H

class DS1302 {
  private:
    static void    _cs();
    static void    _dcs();
    static void    _write(uint8_t val);
    static uint8_t _read();
    
    static uint8_t read_reg(uint8_t addr);
    static void write_reg(uint8_t addr, uint8_t data);
  public:
    static void init();
    static void get(uint8_t &h, uint8_t &m, uint8_t &s, uint8_t &month, uint8_t &day, uint8_t &year, uint8_t &weekday);
    static void set(uint8_t h, uint8_t m, uint8_t s, uint8_t month, uint8_t day, uint8_t year, uint8_t weekday);
    static void dump_clock();
};
#endif // DS1302_H