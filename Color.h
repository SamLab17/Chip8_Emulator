/*
 * Chip 8 Emulator & Interpreter
 * Copyright (c) 2020 Samuel Laberge.
 */

#ifndef CHIP8_EMULATOR_COLOR_H
#define CHIP8_EMULATOR_COLOR_H


#include <cstdint>

/*
 * Simple class to represent a color with 8 bit RGB values
 */
class Color {
public:
    uint8_t red, green, blue;

    constexpr Color(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}
};

/*
 * Color constants for easy use
 */
constexpr Color WHITE{255, 255, 255};
constexpr Color BLACK{0, 0, 0};
constexpr Color RED{255, 0, 0};
constexpr Color GREEN{0, 255, 0};
constexpr Color BLUE{0, 0, 255};


#endif //CHIP8_EMULATOR_COLOR_H
