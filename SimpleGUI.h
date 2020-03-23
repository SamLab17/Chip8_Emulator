//
// Created by Samuel Laberge on 3/23/20.
//

#ifndef CHIP8_EMULATOR_SIMPLEGUI_H
#define CHIP8_EMULATOR_SIMPLEGUI_H


#include "C8GUI.h"
#include "HexKeyboardMap.h"
#include "Chip8VM.h"
#include "Controller.h"
#include "SDLInterface.h"

class SimpleGUI : public C8GUI {
private:
    static constexpr SDL_Rect WINDOW_RECT{0, 0, 640, 320};
    static constexpr const char *WINDOW_TITLE = "Chip 8 Emulator";
    HexKeyboardMap hex_map{};
    Chip8VM *vm;
    Controller *controller;
    SDLInterface *sdl;
    bool is_paused;
public:
    /* Constructor Destructor */
    SimpleGUI(Controller *controller, Chip8VM *vm);

    ~SimpleGUI();

    /* C8GUI Interface methods */
    void drawNextFrame() override;

    void processEvent(SDL_Event &e) override;
};


#endif //CHIP8_EMULATOR_SIMPLEGUI_H
