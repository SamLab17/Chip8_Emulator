//
// Created by Samuel Laberge on 3/23/20.
//

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
    static constexpr uint32_t EMULATION_FREQUENCY = 100;
    static constexpr uint32_t SEC_TO_MILLISEC = 1000;
    // The number of milliseconds between ticks
    static constexpr uint32_t TICK_INTERVAL = (uint32_t) ((1.0 / EMULATION_FREQUENCY) * SEC_TO_MILLISEC);

    uint32_t delayTime();

public:
    // Constructor, loads in program and initializes vm
    Controller(const char *program_name);

    // Start the emulation cycle, this function will return once window is closed
    void start(bool detailed);

    // Terminates the emulation loop. This will be called by a GUI object
    void quit(int exit_code);
};


#endif //CHIP8_EMULATOR_CONTROLLER_H
