/*
 * Chip 8 Emulator & Interpreter
 * Copyright (c) 2020 Samuel Laberge.
 */

#include <iostream>
#include "gtest/gtest.h"
#include "Chip8VM.h"
#include "Chip8Constants.h"

using namespace C8Constants;

TEST(Chip8VM, ConstructorClearsMemory) {
    uint16_t zero = 0;
    auto vm = new Chip8VM(&zero, 2);
    const uint8_t *mem = vm->getMemory();
    for (int i = PROG_START; i < MEM_SIZE; i++) {
        ASSERT_EQ(mem[i], 0);
    }
}

TEST(Chip8VM, ConstructorLoadsProgram) {
    uint16_t sample_program[] = {0x0001, 0x0002, 0x0003, 0x0004, 0x0005};
    auto *vm = new Chip8VM(sample_program, 10);
    const uint8_t *mem = vm->getMemory();
    // Check program is in correct location in memory
    auto *progu8 = (uint8_t *) sample_program;
    for (int i = PROG_START; i < 10; i++) {
        ASSERT_EQ(mem[i], progu8[i - PROG_START]);
    }
    delete vm;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


