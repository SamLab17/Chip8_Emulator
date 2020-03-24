//
// Created by Samuel Laberge on 3/21/20.
//

#ifndef CHIP8_EMULATOR_GUITEXTPRINTER_H
#define CHIP8_EMULATOR_GUITEXTPRINTER_H


#include <cstdint>
#include <string>

#include <SDL.h>
#include "Color.h"

class GUITextPrinter {
    static constexpr const char *SPRITE_SHEET_FILE_NAME = "font.bmp";
    static constexpr uint8_t CHAR_WIDTH = 10, CHAR_HEIGHT = 12;
    SDL_Renderer *renderer;
    SDL_Rect character_rects[256];

    void init_rect_table();

    SDL_Texture *texture;

    void loadTexture();

public:
    explicit GUITextPrinter(SDL_Renderer *);

    void queueCharRender(int x, int y, char c, Color color = WHITE);

    void queueStringRender(int x, int y, const char *str, Color c = WHITE);

    ~GUITextPrinter();
};


#endif //CHIP8_EMULATOR_GUITEXTPRINTER_H
