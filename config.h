/* Copyright 2015-2023 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define RGBLIGHT_LIMIT_VAL 255
#define RGBLIGHT_EFFECT_CHRISTMAS
//#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_CHRISTMAS
#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT


//#define TAPPING_TERM 175
//#define TAPPING_TERM 300
//#define TAPPING_TERM_PER_KEY

#pragma once

#define TEMPO_DEFAULT 60
#define BTTF H__NOTE(_D4), HD_NOTE(_A4), Q__NOTE(_D5), HD_NOTE(_C5), E__NOTE(_B4), E__NOTE(_A4), QD_NOTE(_B4), QD_NOTE(_A4), Q__NOTE(_G4), W__NOTE(_A4), S__NOTE(_A4), E__NOTE(_A4),
#ifdef AUDIO_ENABLE
    //#define STARTUP_SONG SONG(BTTF)
    #define STARTUP_SONG SONG(QWERTY_SOUND)
#endif

// #define STARTUP_SONG SONG(NO_SOUND)
#    define DEFAULT_LAYER_SONGS \
        { SONG(QWERTY_SOUND), SONG(COLEMAK_SOUND), SONG(DVORAK_SOUND) }

/*
 * MIDI options
 */

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/

#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
// #define MIDI_ADVANCED
