//
// Created by Samuel Laberge on 3/19/20.
//

#ifndef CHIP8_EMULATOR_CHIP8VM_H
#define CHIP8_EMULATOR_CHIP8VM_H

#include <cstdint>
#include <exception>
#include <random>
#include <iostream>

#include "Chip8Constants.h"

// TODO: Write unit tests for the CPU
using namespace C8Constants;

class Chip8VM {
private:
    static void checkRegisterIndex(uint16_t index);

    /* Internal data and registers for VM */
    uint8_t memory[MEM_SIZE]{};
    uint16_t pc;
    uint16_t stack[STACK_SIZE]{};
    uint8_t sp;
    uint8_t v_reg[NUM_V_REGS];
    uint16_t i_reg;
    uint8_t dt_reg;
    uint8_t st_reg;

    /* VM Graphics */
    bool graphics[SCREEN_HEIGHT][SCREEN_WIDTH];
    void clearGraphics();
    void drawSprite(const uint8_t *sprite, uint8_t n, uint8_t x, uint8_t y);

    /* Random Number Generator */
    std::default_random_engine rand_generator;
    std::uniform_int_distribution<uint8_t> u8_dist{0, 255};

    /* Whether the screen has been updated since the last time it was drawn to the screen */
    bool draw_flag = false;
    /* Which of the hex keys are currently being pressed by the user */
    bool key_status[16];

public:
    Chip8VM(void *program_buffer, uint32_t program_size);

    /* Emulate one clock cycle of the VM */
    void emulateCycle();

    /*
     * Accessor Methods for GUI Display and Testing
     */
    uint16_t getPC() { return pc; }

    const uint8_t *getVRegisters() { return v_reg; }

    uint8_t getDelayTimer() { return dt_reg; }

    uint8_t getSoundTimer() { return st_reg; }

    uint16_t getIRegister() { return i_reg; }

    uint8_t getStackPointer() { return sp; }

    const uint16_t *getStack() { return stack; }

    const uint8_t *getMemory() { return memory; }

    uint16_t getOpCode() { return (memory[pc] << 8u) | memory[pc + 1]; }

    const bool *getGraphics() { return reinterpret_cast<const bool *>(&graphics); }

    void keyPressed(uint8_t key_val);
    void keyReleased(uint8_t key_val);
};

#endif //CHIP8_EMULATOR_CHIP8VM_H
