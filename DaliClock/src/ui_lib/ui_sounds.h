/*****************
 * ui_sounds.cpp *
 *****************/

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

#ifndef _UI_SOUNDS_H_
#define _UI_SOUNDS_H_

/******************* TINY INTERVAL CLASS ***********************/

/* Helpful Reference:
 *
 *  https://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover
 */

/* tiny_interval_t downsamples a 32-bit millis() value
   into a 8-bit value which can record periods of
   a few seconds with a rougly 1/16th of second
   resolution. This allows us to measure small
   intervals without needing to use four-byte counters.
 */
class tiny_time_t {
  private:
    friend class tiny_timer_t;
    uint8_t _duration;

    static uint8_t tiny_time(uint32_t ms) {return ceil(float(ms) / 64);};

  public:
    tiny_time_t()            : _duration(0) {}
    tiny_time_t(uint32_t ms) : _duration(tiny_time(ms)) {}
    tiny_time_t & operator=   (uint32_t ms) {_duration = tiny_time(ms); return *this;}
    bool          operator == (uint32_t ms) {return _duration == tiny_time(ms);}
};

class tiny_timer_t {
  private:
    uint8_t _start;

  public:
    void start();
    bool elapsed(tiny_time_t interval);
};

/******************* SOUND HELPER CLASS ************************/

namespace FTDI {
  typedef enum {
    PLAY_ASYNCHRONOUS,
    PLAY_SYNCHRONOUS
  } play_mode_t;

  class SoundPlayer {
    public:
      struct sound_t {
        effect_t  effect;      // The sound effect number
        note_t    note;        // The MIDI note value
        uint16_t  sixteenths;  // Duration of note, in sixteeths of a second, or zero to play to completion
      };

      const uint8_t WAIT = 0;

      static const PROGMEM sound_t silence[];

    private:
      const sound_t    *sequence;
      tiny_timer_t     timer;
      tiny_time_t      wait;

      note_t frequency_to_midi_note(const uint16_t frequency);

    public:
      static void set_volume(uint8_t volume);
      static uint8_t get_volume();

      static void play(effect_t effect, note_t note = NOTE_C4);
      static bool is_sound_playing();

      void play(const sound_t* seq, play_mode_t mode = PLAY_SYNCHRONOUS);
      void play_tone(const uint16_t frequency_hz, const uint16_t duration_ms);
      bool has_more_notes() {return sequence != 0;};

      void onIdle();
  };

  extern SoundPlayer sound;

  /* A sound sequence consists of an array of the following:

      struct sound_t {
        effect_t effect;      // The sound effect number
        note_t   note;        // The MIDI note value, or C4 if 0
        uint8_t  sixteenths;  // Note duration in 1/16th of a sec;
                              // If 0, play until sample is finished.
      };

     Constants are defined in "AO_FT810_Constants.h".

     Both note and sixteenths are optional. If omitted, the compiler
     will fill them in with 0 which will be interpreted by play C4
     for the duration of the sample.

     The sequence must be terminated by "{SILENCE, END_SONG, WAIT}", i.e
     all zeros.
   */

  const PROGMEM SoundPlayer::sound_t chimes[] = {
    {CHIMES,       NOTE_G3,  5},
    {CHIMES,       NOTE_E4,  5},
    {CHIMES,       NOTE_C4,  5},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t sad_trombone[] = {
    {TRUMPET,      NOTE_A3S, 10},
    {TRUMPET,      NOTE_A3 , 10},
    {TRUMPET,      NOTE_G3S, 10},
    {TRUMPET,      NOTE_G3,  20},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t twinkle[] = {
    {GLOCKENSPIEL, NOTE_C4,  1},
    {GLOCKENSPIEL, NOTE_E4,  1},
    {GLOCKENSPIEL, NOTE_G4,  16},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t start_print[] = {
    {TRUMPET,      NOTE_A3,  4},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_A3,  2},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_A3,  2},
    {SILENCE,      REST,     1},
    {TRUMPET,      NOTE_E4,  10},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t media_inserted[] = {
    {MUSIC_BOX,    NOTE_C4,  2},
    {MUSIC_BOX,    NOTE_E4,  2},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t media_removed[] = {
    {MUSIC_BOX,    NOTE_E4,  2},
    {MUSIC_BOX,    NOTE_C4,  2},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t js_bach_joy[] = {
    {PIANO,        NOTE_G3,  4},
    {PIANO,        NOTE_A3,  4},
    {PIANO,        NOTE_B3,  4},
    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_C4,  4},
    {PIANO,        NOTE_C4,  4},
    {PIANO,        NOTE_E4,  4},
    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_G4 , 4},
    {PIANO,        NOTE_F4S, 4},
    {PIANO,        NOTE_G4,  4},
    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_B3,  4},
    {PIANO,        NOTE_G3,  4},
    {PIANO,        NOTE_A3,  4},
    {PIANO,        NOTE_B3,  4},
    {PIANO,        NOTE_C4,  4},
    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_E4,  4},
    {PIANO,        NOTE_D4,  4},
    {PIANO,        NOTE_C4,  4},
    {PIANO,        NOTE_B3,  4},
    {PIANO,        NOTE_A3,  4},
    {PIANO,        NOTE_B3,  4},
    {PIANO,        NOTE_G3,  4},
    {PIANO,        NOTE_G3,  4},
    {SILENCE,      END_SONG, 0}
  };

  const PROGMEM SoundPlayer::sound_t all_instruments[] = {
    {HARP},
    {XYLOPHONE},
    {TUBA},
    {GLOCKENSPIEL},
    {ORGAN},
    {TRUMPET},
    {PIANO},
    {CHIMES},
    {MUSIC_BOX},
    {BELL},
    {CLICK},
    {SWITCH},
    {COWBELL},
    {NOTCH},
    {HIHAT},
    {KICKDRUM},
    {SWITCH},
    {POP},
    {CLACK},
    {CHACK},
    {SILENCE, END_SONG, 0}
  };
}

#endif // _UI_SOUNDS_H_
