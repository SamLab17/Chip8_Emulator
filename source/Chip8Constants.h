/*
 * Chip 8 Emulator & Interpreter
 * Copyright (c) 2020 Samuel Laberge.
 */

#ifndef CHIP8_EMULATOR_CHIP8CONSTANTS_H
#define CHIP8_EMULATOR_CHIP8CONSTANTS_H

#include <cstdint>

namespace C8Constants {
    static constexpr uint8_t DEFAULT_FONT[] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    static constexpr uint32_t MEM_SIZE = 4096;
    static constexpr uint32_t PROG_START = 0x200;

    static constexpr uint16_t STACK_SIZE = 16;
    static constexpr uint32_t NUM_V_REGS = 16;

    static constexpr uint16_t SCREEN_WIDTH = 64, SCREEN_HEIGHT = 32;
    static constexpr uint32_t GRAPHICS_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;
}

#endif //CHIP8_EMULATOR_CHIP8CONSTANTS_H
