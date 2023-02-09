/*
 * Copyright 2022 Kevin Gee <info@controller.works>
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
#include "quantum.h"

#ifdef OLED_ENABLE

static void render_logo(void) {
    static const char PROGMEM raw_logo[] = {
       0,  0,248,248,192,  0,  0,  0,  0,  0,248,248,  0,  0,  0,  0,128,128,128,128,  0,  0,  0,252,252,  0,  0,  0,128,128,  0,  0,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,  0,  0,  0,  0,128,128,128,128,128,  0,  0,  0,  0,128,128,128,128,  0,  0,  0,252,252,  0,  0,  0,  0,  0,  0,128,128,128,128,128,  0,  0,  8,252,128,128,128,128,128,  0,  0, 48,184, 16,128,240,240,128,128,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,255,255,255,248,  0,  0,192,254, 31,255,  0,  0, 56, 63,  1,  0,  0,  3,255,  0,  0,255,255,224,120, 30,  7,  1,252,127,  1,  0,  0,143,255,  0,  0,  0,  0,  0,248,255,  3,  0,  0,  1,127, 56,  0,254,  7,  1,  0,  1, 15,254,  0,248,255,  1,  0,  0,  3,255,248,  0,255,255,  0,  0,  0,  0,  0, 30,127,193,128,  0, 63, 63,  0,  0,255,  3,  1,  0,  1,255,255,  0,  0,255,  0,  0,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,255,255,  0, 31,254,240,127,  3,  0,255,  0,  0,252,255,  3,  1,  1,193,255,  0,  0,255,255, 31, 56,224,192,  0,255,253,  1,  1,  1,225,253,  0,  0,  0,  0,  0, 63,255,128,  0,  0,  0,252, 60,  0,255,192,  0,  0,  0,224,255,  0,127,255,  0,  0,  0,  0,255, 63,  0,255,255,  0,  0,  0,  0,  0,248,240,  1,  1,  7, 30,248,  0,  0,255,  0,  0,  0,  0,255,255,  0,  0,255,  0,  0,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  3,  3,  0,  0,  3,  3,  0,  0,  0,  3,  0,  0,  1,  3,  6,  6,  3,  3,  3,  0,  0,  3,  3,  0,  0,  0,  3,  3,  0,  3,  3,  6,  6,  3,  1,  0,  0,  0,  0,  0,  0,  1,  3,  6,  6,  3,  3,  0,  0,  1,  3,  6,  6,  6,  3,  0,  0,  0,  3,  3,  6,  6,  3,  1,  0,  0,  3,  3,  0,  0,  0,  0,  0,  1,  3,  6,  6,  6,  3,  3,  0,  3,  3,  0,  0,  0,  0,  3,  3,  0,  0,  3,  0,  0,  1,  3,  6,  6,  6,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
}

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    }

    return rotation;
}

bool render_status(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("BASE\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("SYMBOL\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("NAV\n"), false);
            break;
        case 3:
            oled_write_P(PSTR("NUMBER\n"), false);
            break;            
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
    
    return false;
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_master()) {
        render_status();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_logo();  // Renders a static logo
        oled_scroll_left();  // Turns on scrolling
    }
    return false;
}

#endif