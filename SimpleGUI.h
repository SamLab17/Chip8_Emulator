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

/*
 * Simple View of Chip8 VM.
 * Displays the VM's graphics in a window.
 */
class SimpleGUI : public C8GUI {
private:
    /*
     * Defines the dimensions of the Window to be created
     */
    static constexpr SDL_Rect WINDOW_RECT{0, 0, 640, 320};
    /*
     * Title for the new window
     */
    static constexpr const char *WINDOW_TITLE = "Chip 8 Emulator";

    Chip8VM *vm;
    SDLInterface *sdl;
    Controller *controller;
    HexKeyboardMap hex_map{};
public:
    /*
     * Constructor
     * Initializes & creates SDL Window by calling newWindow(), assigns
     * instance variables
     */
    SimpleGUI(Controller *controller, Chip8VM *vm);

    /*
     * Destructor
     * Deletes the SDLInterface object created in the constructor
     */
    ~SimpleGUI();

    /*
     * Advances VM CPU by one clock cycle and displays
     * the graphics matrix of the VM to the window
     */
    void drawNextFrame() override;

    /*
     * Processes keyboard events. If window is closed, will call
     * Controller::quit(). SimpleGUI only handles hex keyboard input,
     * just passes it to the VM object
     */
    void processEvent(SDL_Event &e) override;
};


#endif //CHIP8_EMULATOR_SIMPLEGUI_H
