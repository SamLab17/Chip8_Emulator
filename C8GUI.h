//
// Created by Samuel Laberge on 3/23/20.
//

#ifndef CHIP8_EMULATOR_C8GUI_H
#define CHIP8_EMULATOR_C8GUI_H


#include <SDL_events.h>

/*
 * Standard interface for the different GUIs that can be controlled by the
 * Controller class.
 */
class C8GUI {
public:
    /*
     * Asks the GUI implementation to draw the next frame for the window.
     * This method will be called at a set frequency (60Hz is default for the
     * Chip8 System) and thus it is also responsible for calling emulateCycle()
     * on the VM object.
     */
    virtual void drawNextFrame() = 0;

    /*
     * SDL Events read in by the Controller will be passed into the GUI
     * through this method. The GUI implementation will process these events
     * in the way it chooses and can ignore events by just returning
     */
    virtual void processEvent(SDL_Event &e) = 0;
};


#endif //CHIP8_EMULATOR_C8GUI_H
