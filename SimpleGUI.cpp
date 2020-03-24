/*
 * Chip 8 Emulator & Interpreter
 * Copyright (c) 2020 Samuel Laberge.
 */

#include "SimpleGUI.h"

SimpleGUI::SimpleGUI(Controller *controller, Chip8VM *vm) {
    this->vm = vm;
    this->controller = controller;
    this->sdl = new SDLInterface;
    sdl->newWindow(WINDOW_RECT.w, WINDOW_RECT.h, WINDOW_TITLE);
}

void SimpleGUI::drawNextFrame() {
    // Emulate a cycle and Draw the resulting screen
    vm->emulateCycle();
    sdl->displayMatrix(&WINDOW_RECT, vm->getGraphics(), C8Constants::SCREEN_WIDTH, C8Constants::SCREEN_HEIGHT);
    sdl->presentChanges();
}

void SimpleGUI::processEvent(SDL_Event &e) {
    if (e.type == SDL_QUIT) {
        // Quit the program with no error code
        controller->quit();

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

SimpleGUI::~SimpleGUI() {
    delete sdl;
}

