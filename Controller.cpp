//
// Created by Samuel Laberge on 3/23/20.
//

#include <SDL.h>
#include <stdexcept>
#include <fstream>
#include "Controller.h"
#include "SimpleGUI.h"

void Controller::quit() {
    running = false;
    SDL_Quit();
}


Controller::Controller(const char *program_name) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("Failed to initialize SDL");
    }
    auto buffer = new char[512];
    std::ifstream input;
    input.open(program_name, std::ios::binary);
    if (input) {
        input.seekg(0, std::ios::beg);
        input.read(buffer, 512);
    } else {
        throw std::runtime_error("Could not read program file");
    }
    vm = new Chip8VM(buffer, input.gcount());
}

uint32_t Controller::delayTime() {
    uint32_t now = SDL_GetTicks();
    if (next_time <= now) {
        return 0;
    }
    return next_time - now;
}

void Controller::start(bool detailed) {
    if (detailed) {
        // Create a DetailedGUI object
    } else {
        gui = new SimpleGUI(this, vm);
    }

    /*
     * Initialize next_time, the time we should draw the
     * next frame.
     */
    next_time = SDL_GetTicks() + TICK_INTERVAL;
    SDL_Event e;

    // Emulation Loop
    running = true;
    while (running) {
        // Events loop
        while (SDL_PollEvent(&e)) {
            // Pass events to GUI
            gui->processEvent(e);
        }

        // Draw the next frame to the window
        gui->drawNextFrame();

        // Delay to maintain frame rate
        SDL_Delay(delayTime());
        next_time += TICK_INTERVAL;
    }
}
