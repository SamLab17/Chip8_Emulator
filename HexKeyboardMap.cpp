//
// Created by Samuel Laberge on 3/23/20.
//

#include <cstring>
#include <SDL.h>
#include "HexKeyboardMap.h"


HexKeyboardMap::HexKeyboardMap() {
    memset(hex_value, 0x10, MAP_SIZE);
    memset(valid_hex_keys, false, MAP_SIZE);

    // Create hex keyboard layout
    hex_value[SDLK_1] = 1;
    valid_hex_keys[SDLK_1] = true;

    hex_value[SDLK_2] = 2;
    valid_hex_keys[SDLK_2] = true;

    hex_value[SDLK_3] = 3;
    valid_hex_keys[SDLK_3] = true;

    hex_value[SDLK_4] = 0xC;
    valid_hex_keys[SDLK_4] = true;

    hex_value[SDLK_q] = 4;
    valid_hex_keys[SDLK_q] = true;

    hex_value[SDLK_w] = 5;
    valid_hex_keys[SDLK_w] = true;

    hex_value[SDLK_e] = 6;
    valid_hex_keys[SDLK_e] = true;

    hex_value[SDLK_r] = 0xD;
    valid_hex_keys[SDLK_r] = true;

    hex_value[SDLK_a] = 7;
    valid_hex_keys[SDLK_a] = true;

    hex_value[SDLK_s] = 8;
    valid_hex_keys[SDLK_s] = true;

    hex_value[SDLK_d] = 9;
    valid_hex_keys[SDLK_d] = true;

    hex_value[SDLK_f] = 0xE;
    valid_hex_keys[SDLK_f] = true;

    hex_value[SDLK_z] = 0xA;
    valid_hex_keys[SDLK_z] = true;

    hex_value[SDLK_x] = 0;
    valid_hex_keys[SDLK_x] = true;

    hex_value[SDLK_c] = 0xB;
    valid_hex_keys[SDLK_c] = true;

    hex_value[SDLK_v] = 0xF;
    valid_hex_keys[SDLK_v] = true;
}
