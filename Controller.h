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
 *  > Go through event queue, pass the buck to GUI
 */

class Controller {
private:
    Chip8VM *vm;
    C8GUI *gui;
    uint32_t next_time;
    bool running;
    // The number of milliseconds between ticks
    static constexpr uint32_t TICK_INTERVAL = 10;

    uint32_t delayTime();

public:
    Controller(const char *program_name);

    void start(bool detailed);

    void quit(int exit_code);
};


#endif //CHIP8_EMULATOR_CONTROLLER_H
