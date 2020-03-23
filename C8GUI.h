//
// Created by Samuel Laberge on 3/23/20.
//

#ifndef CHIP8_EMULATOR_C8GUI_H
#define CHIP8_EMULATOR_C8GUI_H


#include <SDL_events.h>

class C8GUI {
public:
    virtual void drawNextFrame() = 0;

    virtual void processEvent(SDL_Event &e) = 0;
};


#endif //CHIP8_EMULATOR_C8GUI_H
