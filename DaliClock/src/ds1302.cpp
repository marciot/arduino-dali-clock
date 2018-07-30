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

#define DS1302_CS   6
#define DS1302_SCLK 8
#define DS1302_DAT  7

#include "Arduino.h"

#include "avr_io.h"
#include "ds1302.h"

void DS1302::init() {
  SET_OUTPUT(DS1302_CS);
  WRITE(DS1302_CS, 0);
}

void DS1302::_cs() {
  SET_OUTPUT(DS1302_SCLK);
  SET_OUTPUT(DS1302_CS);
  SET_INPUT(DS1302_DAT);
  WRITE(DS1302_DAT, 0);

  WRITE(DS1302_SCLK, 0);
  delay(1);
  WRITE(DS1302_CS, 1);
  delay(1);
}

void DS1302::_dcs() {
  delay(1);
  WRITE(DS1302_CS,  0);
  SET_OUTPUT(DS1302_DAT); 
}

void DS1302::_write(uint8_t val) {
  SET_OUTPUT(DS1302_DAT);
  for(uint8_t bit = 0x01; bit != 0; bit <<= 1) {
    WRITE(DS1302_SCLK, 0);
    delay(1);
    WRITE(DS1302_DAT, (val & bit) ? 1 : 0);
    delay(1);
    WRITE(DS1302_SCLK, 1);
    delay(1);
  }
  WRITE(DS1302_DAT, 0);
  SET_INPUT(DS1302_DAT);
  WRITE(DS1302_SCLK, 0);
}

uint8_t DS1302::_read() {
  uint8_t val = 0;
  for(uint8_t bit = 0x01; bit != 0; bit <<= 1) {
    if(READ(DS1302_DAT)) val |= bit;
    delay(1);
    WRITE(DS1302_SCLK, 1);
    delay(1);
    WRITE(DS1302_SCLK, 0);
    delay(1);
  }
  return val;
}

uint8_t DS1302::read_reg(uint8_t addr) {
  uint8_t val;
  _cs();
  _write(addr);
  val = _read();
  _dcs();
  return val;
}

void DS1302::write_reg(uint8_t addr, uint8_t data) {
  _cs();
  _write(addr);
  _write(data);
  _dcs();
}

void DS1302::get(uint8_t &h, uint8_t &m, uint8_t &s, uint8_t &month, uint8_t &day, uint8_t &year, uint8_t &weekday) {
  DS1302::init();
  _cs();
  _write(0xBF);
  const uint8_t data[7] = {_read(),_read(),_read(),_read(),_read(),_read(),_read()};
  _dcs();
  s       = ((data[0] & 0b01110000) >> 4) * 10 + (data[0] & 0b00001111);
  m       = ((data[1] & 0b01110000) >> 4) * 10 + (data[1] & 0b00001111);
  h       = ((data[2] & 0b00110000) >> 4) * 10 + (data[2] & 0b00001111);
  day     = ((data[3] & 0b00110000) >> 4) * 10 + (data[3] & 0b00001111);
  month   = ((data[4] & 0b00010000) >> 4) * 10 + (data[4] & 0b00001111);
  weekday =                                      (data[5] & 0b00000111);
  year    = ((data[6] & 0b11110000) >> 4) * 10 + (data[6] & 0b00001111);
}

void DS1302::set(uint8_t h, uint8_t m, uint8_t s, uint8_t month, uint8_t day, uint8_t year, uint8_t weekday) {
  DS1302::init();
  write_reg(0x8E, 0x00); // Clear write protect
  write_reg(0x80, 0x00); // Clear clock hold
  _cs();
  _write(0xBE);
  _write((((    s/10) << 4) | (    s%10)) & 0b01111111);
  _write((((    m/10) << 4) | (    m%10)) & 0b01111111);
  _write((((    h/10) << 4) | (    h%10)) & 0b00111111);
  _write((((  day/10) << 4) | (  day%10)) & 0b00111111);
  _write((((month/10) << 4) | (month%10)) & 0b00011111);
  _write(                    weekday      & 0b00000111);
  _write(((( year/10) << 4) | ( year%10)) & 0b11111111);
  _write(0);
  _write(0);
  _write(0);
  _write(0);
  _write(0);
  _dcs();
}

void DS1302::dump_clock() {
  delay(1);
  
  DS1302::init();
  Serial.print("0x81: "); Serial.println(DS1302::read_reg(0x81), BIN);
  Serial.print("0x83: "); Serial.println(DS1302::read_reg(0x83), BIN);
  Serial.print("0x85: "); Serial.println(DS1302::read_reg(0x85), BIN);
  Serial.print("0x87: "); Serial.println(DS1302::read_reg(0x87), BIN);
  Serial.print("0x89: "); Serial.println(DS1302::read_reg(0x89), BIN);
  Serial.print("0x8B: "); Serial.println(DS1302::read_reg(0x8B), BIN);
  Serial.print("0x8D: "); Serial.println(DS1302::read_reg(0x8D), BIN);
  Serial.print("0x8F: "); Serial.println(DS1302::read_reg(0x8F), BIN);
  Serial.print("0x91: "); Serial.println(DS1302::read_reg(0x91), BIN);

  Serial.println("RAM Contents:");
  for(uint8_t addr = 0xC1; addr < 0xFF; addr += 2) {
    Serial.println(DS1302::read_reg(addr));
  }
  Serial.println();

  DS1302::write_reg(0x8E, 0x00); // Clear write protect
  DS1302::write_reg(0x80, 0x00); // Clear clock hold
  DS1302::write_reg(0x90, 0x00); // Disable charger

  // Clear the RAM
  for(uint8_t addr = 0xC0; addr < 0xFE; addr += 2) {
    DS1302::write_reg(addr, 0);
  }
  
  uint8_t addr = 0xC0;
  DS1302::write_reg(addr, 1); addr += 2;
  DS1302::write_reg(addr, 2); addr += 2;
  DS1302::write_reg(addr, 3); addr += 2;
  DS1302::write_reg(addr, 4); addr += 2;
  DS1302::write_reg(addr, 5); addr += 2;
  DS1302::write_reg(addr, 6); addr += 2;
}