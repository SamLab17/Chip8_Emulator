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

using namespace C8Constants;

class Chip8VM {
private:
    /*
     * Checks to make sure a given V register index is valid.
     * Will throw an exception if it is not valid.
     */
    static void checkRegisterIndex(uint16_t index);

    /* Internal data and registers for the Chip 8 VM */
    uint8_t memory[MEM_SIZE]{};
    uint16_t pc;
    uint16_t stack[STACK_SIZE]{};
    uint8_t sp;
    uint8_t v_reg[NUM_V_REGS];
    uint16_t i_reg;
    uint8_t dt_reg;
    uint8_t st_reg;

    /*
     * Graphics matrix of the VM
     * Pixels can be either on or off, thus pixel state is stored
     * as a bool.
     */
    bool graphics[SCREEN_HEIGHT][SCREEN_WIDTH];

    /*
     * Clear the graphics array, sets all cells to false
     */
    void clearGraphics();

    /*
     * Draws the given n-byte sprite at the location (x, y) on the screen.
     */
    void drawSprite(const uint8_t *sprite, uint8_t n, uint8_t x, uint8_t y);

    /* Random Number Generator */
    std::default_random_engine rand_generator;
    std::uniform_int_distribution<uint8_t> u8_dist{0, 255};

    /* Whether the screen has been updated since the last time it was drawn to the screen */
    bool draw_flag = false;
    /* Which of the hex keys are currently being pressed by the user */
    bool key_status[16];

public:
    /*
     * Constructor
     * Loads program into memory, initializes registers and graphics
     * Loads the default C8 font into memory
     */
    Chip8VM(void *program_buffer, uint32_t program_size);

    /*
     * Emulates a clock tick of the Chip8 Machine.
     * This should be called 60 times per second.
     */
    void emulateCycle();

    /*
     * Accessor Methods for GUI Display and Testing
     * Methods which return a pointer return a const pointer to prevent
     * another class from writing to the registers
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

    /*
     * Public interface for a hex key being pressed down.
     * Marks key as currently being pressed
     */
    void keyPressed(uint8_t key_val);

    /*
     * Public interface for a hex key having been released.
     * Marks key as not being pressed.
     */
    void keyReleased(uint8_t key_val);
};

#endif //CHIP8_EMULATOR_CHIP8VM_H
