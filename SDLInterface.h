//
// Created by Samuel Laberge on 3/23/20.
//

#ifndef CHIP8_EMULATOR_SDLINTERFACE_H
#define CHIP8_EMULATOR_SDLINTERFACE_H


#include <SDL.h>
#include "GUIPrinter.h"

class SDLInterface {
    static constexpr uint32_t RED_MASK = 0xFF0000;
    static constexpr uint32_t RED_SHIFT = 16;
    static constexpr uint32_t GREEN_MASK = 0xFF00;
    static constexpr uint32_t GREEN_SHIFT = 8;
    static constexpr uint32_t BLUE_MASK = 0xFF;
    static constexpr uint32_t BLUE_SHIFT = 0;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    GUIPrinter *printer = nullptr;

    uint32_t switchToColor(const Color *c);

    void revertColor(uint32_t prev_color_state);

public:
    void newWindow(uint32_t width, uint32_t height, const char *title);

    //SDL_Renderer* getRenderer() { return renderer; }
    void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const Color *c = &WHITE);

    void drawRect(SDL_Rect *rect, const Color *c = &WHITE);

    void fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const Color *c = &WHITE);

    void fillRect(SDL_Rect *rect, const Color *c = &WHITE);

    void printText(uint32_t x, uint32_t y, const char *str, float scale = 1.0f, const Color *c = &WHITE);

    void printMatrix(SDL_Rect *display_rect, bool *matrix, uint32_t matrix_w, uint32_t matrix_h,
                     const Color *true_color, const Color *false_color);

    void clear();
};


#endif //CHIP8_EMULATOR_SDLINTERFACE_H
