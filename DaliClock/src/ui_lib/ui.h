/********
 * ui.h *
 ********/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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

#ifndef _UI_H_
#define _UI_H_

//#include "../ui_api.h"
#include "ui_config.h"

#if defined(_MARLIN_CONFIG_H_)
    // If _MARLIN_CONFIG_H_ exists, then we are being
    // compiled inside Marlin.
    #define USE_MARLIN_IO
#else
    #include "Arduino.h"

    #ifndef pgm_read_word_far
    #define pgm_read_word_far pgm_read_word
    #endif

    #ifndef pgm_read_dword_far
    #define pgm_read_dword_far pgm_read_dword
    #endif

    // Load up compatibility routines
    #define EXTENSIBLE_UI
    #define _CAT(a, ...) a ## __VA_ARGS__
    #define SWITCH_ENABLED_      1
    #define ENABLED(b) _CAT(SWITCH_ENABLED_, b)
    #define DISABLED(b) !ENABLED(b)

    // Messages that are declared in Marlin
    #define WELCOME_MSG     "Printer Ready"
    #define MSG_SD_INSERTED "Media Inserted"
    #define MSG_SD_REMOVED  "Media Removed"

    #define SERIAL_ECHO_START()
    #define SERIAL_ECHOLNPGM(str)        Serial.println(F(str))
    #define SERIAL_ECHOPGM(str)          Serial.print(F(str))
    #define SERIAL_ECHOLNPAIR(str, val) {Serial.print(F(str)); Serial.println(value);}

    #define safe_delay delay
#endif

#endif // _UI_CONFIG_H_