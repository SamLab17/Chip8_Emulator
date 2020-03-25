/*
 * Chip 8 Emulator & Interpreter
 * Copyright (c) 2020 Samuel Laberge.
 */

#ifndef CHIP8_EMULATOR_CONTROLLER_H
#define CHIP8_EMULATOR_CONTROLLER_H

#include "Chip8VM.h"
#include "C8GUI.h"

/*
 * This class will be responsible for:
 *  > Initializing and finalizing SDL
 *  > Reading the C8 program from disk
 *  > Creating the C8VM and GUI objects
 *  > Running the VM at 60Hz
 *  > Go through event queue, pass events to GUI to handle
 */
class Controller {
private:
    Chip8VM *vm;
    C8GUI *gui;
    uint32_t next_time;
    bool running;
    // Number of cpu cycles to emulate per second
    static constexpr uint32_t EMULATION_FREQUENCY = 60;
    static constexpr uint32_t SEC_TO_MILLISEC = 1000;
    // The number of milliseconds between ticks
    static constexpr uint32_t TICK_INTERVAL = (uint32_t) ((1.0 / EMULATION_FREQUENCY) * SEC_TO_MILLISEC);

    uint32_t delayTime();

public:
    /*
     * Controller constructor.
     * Reads in program file from the disk and initializes the VM object
     * with this program.
     */
    Controller(const char *program_name);

    /*
     * Starts the emulation loop for the program specified in the constructor.
     * This function will return once emulation is ended by closing the GUI window.
     */
    void start(bool detailed);

    /*
     * Terminates the emulation loop.
     * This method will be called by the GUI object which was created in start() once
     * it receives an event which tells it to stop emulation.
     */
    void quit();
};


#endif //CHIP8_EMULATOR_CONTROLLER_H
