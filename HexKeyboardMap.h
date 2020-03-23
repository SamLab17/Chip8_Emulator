//
// Created by Samuel Laberge on 3/23/20.
//

#ifndef CHIP8_EMULATOR_HEXKEYBOARDMAP_H
#define CHIP8_EMULATOR_HEXKEYBOARDMAP_H


#include <cstdint>

class HexKeyboardMap {
    static constexpr uint32_t MAP_SIZE = 128;
    bool valid_hex_keys[MAP_SIZE];
    uint8_t hex_value[MAP_SIZE];
public:
    HexKeyboardMap();
};


#endif //CHIP8_EMULATOR_HEXKEYBOARDMAP_H
