// Forked/inspired by: https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum

#include QMK_KEYBOARD_H

// #include "g/keymap_combo.h"
#include "oneshot.h"
#include "swapper.h"

// Keymap helpers
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)
#define SPC_PAD LT(NUM_PAD, KC_SPC)
#define SFT_BSP LSFT_T(KC_BSPC)
#define xxxxxxx KC_NO

// MacOS macros
#define SPOTL LGUI(KC_SPC)
#define ONEPW LGUI(KC_BSLS)
#define SC_SHOT SGUI(KC_4)

// Browser navigation
#define TAB_R LAG(KC_RIGHT)
#define TAB_L LAG(KC_LEFT)
#define BR_BACK LGUI(KC_LEFT)
#define BR_FORW LGUI(KC_RGHT) 

// VIM window navigation
#define VIM_BUFL RCTL(KC_H)
#define VIM_BUFR RCTL(KC_L)

enum layers {
    BASE,
    SYM,
    NAV,
    NUM,
    NUM_PAD,
    BASE_CLMK,
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,

    SW_WIN,  // Switch to next window (cmd-tab)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,     KC_Y,      KC_U,    KC_I,    KC_O,    KC_QUOT,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,     KC_H,      KC_J,    KC_K,    KC_L,    KC_P,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,     KC_N,      KC_M,    KC_COMM, KC_DOT,  KC_SCLN,
                                   LA_NAV,  SPC_PAD,  SFT_BSP,   LA_SYM
    ),

    [SYM] = LAYOUT(
        KC_GRV,  KC_LPRN, KC_LCBR, KC_LBRC, KC_LT,   KC_GT,   KC_RBRC, KC_RCBR, KC_RPRN, KC_HASH,
        KC_ESC,  KC_EQL,  KC_MINS, KC_UNDS, KC_SLSH, KC_COLN, OS_CMD,  OS_CTRL, OS_ALT,  OS_SHFT,
        KC_AT,   KC_QUES, KC_PLUS, KC_PERC, KC_EXLM, KC_AMPR,  KC_DLR, KC_PIPE, KC_BSLS, KC_CIRC,
                                   _______, _______, _______, _______
    ),

    [NAV] = LAYOUT(
        KC_TAB,  SPOTL,   TAB_L,   TAB_R,   KC_VOLU, VIM_BUFL, KC_WH_D, KC_WH_U, VIM_BUFR, KC_DEL, 
        OS_SHFT, OS_ALT,  OS_CTRL, OS_CMD,  KC_VOLD, KC_LEFT,  KC_DOWN, KC_UP,   KC_RGHT,  KC_ENT,
        ONEPW,   SC_SHOT, BR_BACK, BR_FORW, KC_MPLY, KC_HOME,  KC_PGDN, KC_PGUP, KC_END,   KC_BSPC,
                                   _______, _______, _______, _______
    ),

    [NUM] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_F11,  xxxxxxx, xxxxxxx, xxxxxxx, TG(BASE), TG(BASE_CLMK), xxxxxxx, xxxxxxx, xxxxxxx, KC_F12,
                                   _______, _______, _______, _______
    ),

    [NUM_PAD] = LAYOUT(
        xxxxxxx,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_7,   KC_8,    KC_9, xxxxxxx,
        xxxxxxx,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_4,   KC_5,    KC_6, xxxxxxx,
        xxxxxxx,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_1,   KC_2,    KC_3, xxxxxxx,
                                    _______, _______, KC_0,    _______
    ),
    
    [BASE_CLMK] = LAYOUT(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,     KC_J,      KC_L,    KC_U,    KC_Y,    KC_QUOT,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_G,     KC_M,      KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,     KC_K,      KC_H,    KC_COMM, KC_DOT,  KC_SCLN,
                                   LA_NAV,  SPC_PAD,  SFT_BSP,   LA_SYM
    ),
};

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LGUI, KC_TAB, SW_WIN,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}
