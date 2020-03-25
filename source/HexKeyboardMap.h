/*
 * Chip 8 Emulator & Interpreter
 * Copyright (c) 2020 Samuel Laberge.
 */

#ifndef CHIP8_EMULATOR_HEXKEYBOARDMAP_H
#define CHIP8_EMULATOR_HEXKEYBOARDMAP_H


#include <cstdint>

class HexKeyboardMap {
    static constexpr uint32_t MAP_SIZE = 128;
    bool valid_hex_keys[MAP_SIZE];
    uint8_t hex_value[MAP_SIZE];
public:
    HexKeyboardMap();

    bool isValidHexKey(int key);

    uint8_t hexValue(int key);
};


#endif //CHIP8_EMULATOR_HEXKEYBOARDMAP_H
