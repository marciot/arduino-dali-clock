/*******************
 * ui_event_loop.h *
 *******************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
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

#ifndef _UI_EVENT_LOOP_
#define _UI_EVENT_LOOP_

#define STATUS_UPDATE_INTERVAL     1000
#define TOUCH_UPDATE_INTERVAL        50
#define TOUCH_REPEATS_PER_SECOND      4
#define DEBOUNCE_PERIOD             150

typedef struct {
  bool touch_start_sound  : 1;
  bool touch_end_sound    : 1;
  bool touch_repeat_sound : 1;
  bool touch_debouncing   : 1;
  bool ignore_unpress     : 1;
} event_flags_t;

extern event_flags_t event_flags;

void enable_touch_sounds(bool enabled);
bool touch_sounds_enabled();
uint8_t get_pressed_tag();
bool is_touch_held();

#endif // _UI_EVENT_LOOP_