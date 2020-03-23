//
// Created by Samuel Laberge on 3/23/20.
//

#include "SimpleGUI.h"

SimpleGUI::SimpleGUI(Controller *controller, Chip8VM *vm) {
    this->vm = vm;
    this->controller = controller;
    this->sdl = new SDLInterface;
    sdl->newWindow(WINDOW_RECT.w, WINDOW_RECT.h, WINDOW_TITLE);
    is_paused = true;
}

void SimpleGUI::drawNextFrame() {
    // Draw VM Screen
    vm->emulateCycle();
    sdl->printMatrix(&WINDOW_RECT, vm->getGraphics(), C8Constants::SCREEN_WIDTH, C8Constants::SCREEN_HEIGHT, &BLACK,
                     &WHITE);
    sdl->presentChanges();
}

void SimpleGUI::processEvent(SDL_Event &e) {
    if (e.type == SDL_QUIT) {
        // Quit the program with no error code
        controller->quit(0);

    } else if (e.type == SDL_KEYDOWN) {
        // A key was pressed down
        int key_code = e.key.keysym.sym;
        if (hex_map.isValidHexKey(key_code)) {
            vm->keyPressed(hex_map.hexValue(key_code));
        }
    } else if (e.type == SDL_KEYUP) {
        // A key was released
        int key_code = e.key.keysym.sym;
        if (hex_map.isValidHexKey(key_code)) {
            vm->keyReleased(hex_map.hexValue((key_code)));
        }

    }
    //If not one of the above, then do nothing.
}

