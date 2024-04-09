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

#include QMK_KEYBOARD_H

enum planck_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _MOV,
  _NUM,
  _ADJUST,
  _MOUSE
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  ADJUST,
  MOUSE,
  NUM
};

// MONGO defines
#define TABNUM LT(_NUM, KC_TAB) // NUM layer while held, tab when tap
#define QUONUM LT(_NUM, KC_QUOT) // NUM layer while held, simple quote when tap
#define TABMOV LT(_MOV, KC_TAB) // MOV layer while held, tab when tap
#define SPCMOV LT(_MOV, KC_SPC) // MOV layer while held, space when tap
#define SPCNUM LT(_NUM, KC_SPC) // NUM layer while held, space when tap
#define BSNUM LT(_NUM, KC_BSPC) // NUM layer while held, BS when tap
#define SHIFTLOCK LT(KC_LSFT, _NUM) // layer while held, RSFT when tap
#define FMOV LT(_MOV, KC_F) // MOV layer while held, f when tap

#define RAISE OSL(_RAISE)
#define LOWER LT(_LOWER, KC_NO)
//#define LOWER OSL(_LOWER)

#ifdef AUDIO_ENABLE
float plover_song[][2]    = SONG(PLOVER_SOUND);
float plover_gb_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
float qwerty_song[][2] = SONG(QWERTY_SOUND);
float bttf_song[][2] = SONG(BTTF);
float colemak_song[][2] = SONG(COLEMAK_SOUND);
float dvorak_song[][2] = SONG(DVORAK_SOUND);
#endif

/* Combo for kj to ESC
#undef COMBO_COUNT
#define COMBO_COUNT 1
*/
#undef COMBO_TERM //stock 50
#define COMBO_TERM 40
#undef COMBO_TERM_PER_COMBO
#define COMBO_TERM_PER_COMBO 40 //stock 50

#undef TAPPING_TERM //stock 200
#define TAPPING_TERM 40
//#define TAPPING_TERM_PER_KEY
#define RETRO_TAPPING


//# ESTOS SON LOS COMBOS
const uint16_t PROGMEM combo_esc[] = {KC_J, KC_K, COMBO_END};
//const uint16_t PROGMEM combo_enter[] = {KC_SCLN, KC_ENTER, COMBO_END};
const uint16_t PROGMEM combo_num[] = {KC_K, KC_L, COMBO_END};
const uint16_t PROGMEM combo_email[] = {KC_W, KC_A, COMBO_END};

enum combo_events {
  EM_EMAIL,
  CMBESC,
//  CMBENT,
  CMBNUM,
};

combo_t key_combos[] = {
    [EM_EMAIL] = COMBO_ACTION(combo_email),
    [CMBESC] = COMBO(combo_esc, KC_ESC),
//    [CMBENT] = COMBO(combo_enter, KC_ENTER),
    [CMBNUM] = COMBO_ACTION(combo_num),
};


void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case EM_EMAIL:
      if (pressed) {
#ifdef AUDIO_ENABLE
        stop_all_notes();
        //PLAY_SONG(bttf_song);
#endif
        SEND_STRING("andmarti@gmail.com");
      }
      break;
    case CMBESC:
      if (pressed) {
        tap_code16(KC_ESC);
      }
      break;
 /*   case CMBENT:
      if (pressed) {
        tap_code16(KC_ENTER);
      }
      break;
 */
    case CMBNUM:
      if (pressed) {
#ifdef AUDIO_ENABLE
        stop_all_notes();
        PLAY_SONG(colemak_song);
#endif
        layer_on(_NUM);
      }
      break;
    }
}



/* TAP DANCE */
// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    QUOT_LAYR, // ESC, NUM, doble NUM
    MEH_MOV, // MEH, MOV
    TAB_LCTL,
    QUOT_DQUOT,
};
// Declare the functions to be used with your tap dance key(s)

// Function associated with all tap dances
td_state_t cur_dance(tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(tap_dance_state_t *state, void *user_data);
void ql_reset(tap_dance_state_t *state, void *user_data);
















/* clang-format off */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


/* Qwerty MONGO
  * ,----------------------------------------------------------------------------------------------.
 * | ESC NUM  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | BS          |
 * |----------+------+------+------+------+------+------+------+------+------+------+-------------|
 * | TAB CTRL |   A  |   S  |   D  | F MOV|   G  |   H  |   J  |   K  |   L  |   ;  |  ENTER      |
 * |----------+------+------+------+------+------+------+------+------+------+------+-------------|
 * | Shift    |   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |  '   NUM    |
 * |----------+------+------+------+------+------+------+------+------+------+------+-------------|
 * |tmux C-spa| ALT  | WIN  |  LOW | MEH  |    Space    | Raise|      | DEL  | RCTL | HYP         |
 * |          |      |      |      |      |    Mov      |      | AltGR|      |      |             |
 *
 * `----------------------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_grid(
    TD( QUOT_LAYR),       KC_Q,    KC_W,    KC_E,          KC_R,    KC_T,            KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    TD( TAB_LCTL ),       KC_A,    KC_S,    KC_D,          FMOV,    KC_G,            KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENTER,
    KC_LSFT,              KC_Z,    KC_X,    KC_C,          KC_V,    KC_B,            KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, TD(QUOT_DQUOT),
    LCTL(KC_SPACE),       KC_LALT, KC_LGUI, LOWER,         KC_MEH,  SPCMOV,          SPCMOV,  RAISE,   KC_ALGR, KC_DEL,  KC_RCTL, KC_HYPR
),



/* Adjust
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |qwerty| Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|PRINT |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |reboot|MUSmod|Aud on|Audoff|      | NUM  | MOUSE|      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | CAPSL|Voice-|Voice+|Mus on|Musoff|      | PLAY | MUTE | VOLD | VOLU |BRID  |BRIU  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 */
[_ADJUST] = LAYOUT_planck_grid(
    QWERTY, QK_BOOT, DB_TOGG, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_PSCR,
    XXXXXXX, QK_RBT,  MU_NEXT, AU_ON,   AU_OFF,  RGB_M_B, NUM, MOUSE  , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_CAPS, AU_PREV, AU_NEXT, MU_ON,   MU_OFF,  _______, KC_MPLY, KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID, KC_BRIU,
    XXXXXXX, EE_CLR,  XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, _______, _______, _______, _______, _______
),

/* MOUSE
 * ,-----------------------------------------------------------------------------------.
 * |QWERTY|ACL0  |ACL1  |ACL2  |      |      |      |      |      |BUT1  |BUT2  |BUT3  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |MOUSEL|MOUDWN|MOUSUP|MOURIG|      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |W LEFT|W DWN |W UP  |W RGHT|      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 */
[_MOUSE] = LAYOUT_planck_grid(
    QWERTY, KC_ACL0, KC_ACL1, KC_ACL2, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BTN1, KC_BTN2, KC_BTN3,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, XXXXXXX, XXXXXXX,
    XXXXXXX, QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

 /* Lower MONGO
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |  DEL |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |******|             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_grid(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
    QWERTY , _______, _______, _______, _______, _______, _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
    _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),


/* Raise MONGO
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | DEL  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |   _  |   +  |   {  |   }  |  |   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |******|      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_grid(
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
    QWERTY , _______, _______, _______, _______, _______, _______, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
    _______, _______, _______, _______, _______, _______, _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),


/* MOV MONGO
 * TODO: LCTL when held, QWERTY when tapped
 * ,-----------------------------------------------------------------------------------.
 * |QWERTY|      |  w   |      |      | PgUp |      |      |      |      |      | BS   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | LCTL | Home | End  | Shift|      |PgDown| Left | Down |  Up  | Right| DEL  | ENTER|
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |  x   |   c  |      |   b  |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |   *******   |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_MOV] = LAYOUT_planck_grid(
    QWERTY  , XXXXXXX , LCTL(KC_RIGHT), XXXXXXX , XXXXXXX , KC_PGUP , XXXXXXX   , XXXXXXX , XXXXXXX , XXXXXXX  , XXXXXXX   , KC_BSPC,
    KC_LCTL , KC_HOME , KC_END  , KC_LSFT , XXXXXXX , KC_PGDN , KC_LEFT   , KC_DOWN , KC_UP   , KC_RIGHT , KC_DEL,  KC_ENT,
    XXXXXXX , XXXXXXX , KC_DEL  , XXXXXXX , XXXXXXX , LCTL(KC_LEFT), XXXXXXX   , XXXXXXX , XXXXXXX , XXXXXXX  , XXXXXXX   , XXXXXXX,
    _______ , _______ , _______ , _______ , XXXXXXX , _______ , _______   , _______ , _______ , _______  , _______   , _______
 ),




/* NUM MONGO
 * ,-----------------------------------------------------------------------------------.
 * | **** |  F1  |  F2  |  F3  |  F4  |  F5  |      |   7  |   8  |   9  |   *  | BS   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |  F6  |  F7  |  F8  |  F9  |  F10 | DEL  |   4  |   5  |   6  |   +  |QWERTY|
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |  F11 |  F12 | Enter|   1  |   2  |   3  |   -  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |   0  |   ,  |   .  |   /  |      |
 * `-----------------------------------------------------------------------------------'
 */
[_NUM] = LAYOUT_planck_grid(
    QWERTY  , KC_F1   , KC_F2   , KC_F3   , KC_F4   , KC_F5   , XXXXXXX , KC_7 , KC_8    , KC_9    , KC_PAST , KC_BSPC ,
    _______ , KC_F6   , KC_F7   , KC_F8   , KC_F9   , KC_F10  , KC_DEL  , KC_4 , KC_5    , KC_6    , KC_PPLS , QWERTY  ,
    XXXXXXX , XXXXXXX , XXXXXXX , XXXXXXX , KC_F11  , KC_F12  , KC_ENT  , KC_1 , KC_2    , KC_3    , KC_PMNS , XXXXXXX ,
    XXXXXXX , XXXXXXX , XXXXXXX , _______ , XXXXXXX , KC_SPACE, KC_SPACE, KC_0 , KC_COMM , KC_DOT  , KC_SLSH , XXXXXXX
)

};




/* Adjust (Lower + Raise)
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|  Del |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
[_ADJUST] = LAYOUT_planck_grid(
     QWERTY, QK_BOOT, DB_TOGG, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_DEL ,
    _______, EE_CLR,  MU_NEXT, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  XXXXXXX, XXXXXXX, XXXXXXX, _______,
    _______, AU_PREV, AU_NEXT, MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)

};
 */
/* clang-format on */


void rgb_matrix_indicators_user(void) {
    switch (get_highest_layer(layer_state)) {
        case _MOUSE:
            rgblight_sethsv_noeeprom(HSV_RED);
            break;
        case _QWERTY:
            rgblight_sethsv_noeeprom(HSV_RED);
            break;
    }
}


layer_state_t layer_state_set_user(layer_state_t state) {
  switch (get_highest_layer(state)) {
    case _ADJUST:
        //rgblight_setrgb (0x00,  0xFF, 0xFF);
        //rgblight_sethsv_noeeprom(HSV_BLUE);
        //rgblight_Mode(RGBLIGHT_MODE_CHRISTMAS);
        rgblight_sethsv_noeeprom(HSV_WHITE);
        break;
    case _RAISE:
        //rojo ok
        //rgblight_setrgb (0xFF,  0x00, 0x00);
        //rgblight_sethsv_noeeprom(HSV_RED);
        rgblight_sethsv_noeeprom(HSV_GOLDENROD);
        break;
    case _LOWER:
        //verde ok
        //rgblight_setrgb (0x00,  0xFF, 0x00);
        rgblight_sethsv_noeeprom(HSV_CHARTREUSE);
        break;
    case _MOV:
        //rgblight_setrgb (0xFF,  0xFF, 0xFF);
        rgblight_sethsv_noeeprom(HSV_WHITE);
        break;
    case _NUM:
        //rgblight_setrgb (0xFF,  0xFF, 0xFF);
        rgblight_sethsv_noeeprom(HSV_WHITE);
        //rgblight_sethsv_noeeprom(HSV_PURPLE);
        break;
    case _MOUSE:
        //rgblight_setrgb (0xFF,  0xFF, 0xFF);
        rgblight_sethsv_noeeprom(HSV_GOLDENROD);
        break;
    default: //  for any other layers, or the default layer
        //rgblight_setrgb (0x00,  0x00, 0x00);
        rgblight_sethsv_noeeprom(HSV_BLACK);
        break;
    }
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                PLAY_SONG(qwerty_song);
#endif
                layer_clear();
                layer_on(_QWERTY);
                //set_single_persistent_default_layer(_QWERTY);
            }
            return false;
            break;
        case ADJUST:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                //PLAY_SONG(dvorak_song);
                //PLAY_SONG(qwerty_song);
                PLAY_SONG(plover_song);
#endif
                layer_clear();
                layer_on(_ADJUST);
            }
            return false;
            break;
        case MOUSE:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                //PLAY_SONG(plover_song);
                //PLAY_SONG(plover_gb_song);
                PLAY_SONG(dvorak_song);
#endif
                layer_clear();
                layer_on(_MOUSE);
            }
            return false;
            break;
            /*
        case NUM:
            if (record->event.pressed) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                PLAY_SONG(plover_song);
                //PLAY_SONG(colemak_song);
#endif
                layer_clear();
                layer_on(_NUM);
            }
            return false;
            break;
            */
    }
    return true;
}


// TAP DANCE
// Determine the current tap dance state
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return TD_SINGLE_TAP;
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) return TD_DOUBLE_TAP;
    else return TD_UNKNOWN;
}

// Initialize tap structure associated with example tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does TECLA ESC
void ql_finished(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_ESC);
            break;
        case TD_SINGLE_HOLD:
            //layer_clear();
            layer_on(_NUM);
            break;
        case TD_DOUBLE_TAP:
            // Check to see if the layer is already set
            if (layer_state_is(_NUM)) {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                PLAY_SONG(plover_song);
#endif
                // If already set, then switch it off
                layer_off(_NUM);
            } else {
#ifdef AUDIO_ENABLE
                stop_all_notes();
                PLAY_SONG(colemak_song);
#endif
                // If not already set, then switch the layer on
                layer_on(_NUM);
            }
            break;
        default:
            break;
    }
}

void ql_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(_NUM);
    }
    ql_tap_state.state = TD_NONE;
}






// Functions that control TAB_LCTL tap dance     TECLA TAB con CONTROL
void ql_finished_TAB_LCTL(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_HOLD: register_code(KC_LCTL); break;
        case TD_SINGLE_TAP:  register_code(KC_TAB); break;
        default:
            break;
    }
}

void ql_reset_TAB_LCTL(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (ql_tap_state.state == TD_SINGLE_HOLD) {
        unregister_code(KC_LCTL);
    } else if (ql_tap_state.state == TD_SINGLE_TAP) {
        unregister_code(KC_TAB);
    }
    ql_tap_state.state = TD_NONE;
}


// Functions that control what our tap dance key does   TECLA COMILLAS, COMILLAS DOBLE, NUM
void ql_finished_quote(tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP:
            tap_code(KC_QUOT);
            break;
        case TD_SINGLE_HOLD:
            //layer_clear();
            layer_on(_NUM);
            break;
        case TD_DOUBLE_TAP:
            set_mods(MOD_MASK_SHIFT);
            tap_code(KC_QUOT);
            break;
        default:
            break;
    }
}


void ql_reset_quote(tap_dance_state_t *state, void *user_data) {
    if (ql_tap_state.state == TD_SINGLE_TAP) {
        unregister_code(KC_QUOT);
    } else if (ql_tap_state.state == TD_DOUBLE_TAP) {
        del_mods(MOD_MASK_SHIFT);
        unregister_code(KC_QUOT);
    } else if (ql_tap_state.state == TD_SINGLE_HOLD) {
        layer_off(_NUM);
    }
    ql_tap_state.state = TD_NONE;
}


// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    [QUOT_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset),
    [TAB_LCTL]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished_TAB_LCTL, ql_reset_TAB_LCTL),
    [QUOT_DQUOT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished_quote, ql_reset_quote)
};

// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
        default:
            return TAPPING_TERM;
    }
}
