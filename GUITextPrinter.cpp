//
// Created by Samuel Laberge on 3/21/20.
//

#include "GUITextPrinter.h"
#include "Color.h"
#include <stdexcept>
#include <cstring>

GUITextPrinter::GUITextPrinter(SDL_Renderer *renderer) {
    this->renderer = renderer;
    loadTexture();
    //Initialize character rect look up table
    init_rect_table();
}

void GUITextPrinter::init_rect_table() {
    int x = 0, y = 0;
    for (auto &char_rect : character_rects) {
        //SDL_Rect *curr = &character_rect;
        char_rect.x = (x % 16) * CHAR_WIDTH;
        char_rect.y = (y / 16) * CHAR_HEIGHT;
        char_rect.w = CHAR_WIDTH;
        char_rect.h = CHAR_HEIGHT;
        x++;
        y++;
    }
}

void GUITextPrinter::loadTexture() {
    SDL_Surface *loaded_surface = SDL_LoadBMP(SPRITE_SHEET_FILE_NAME);
    if (loaded_surface == nullptr) {
        throw std::runtime_error("Could not find fonts file.");
    }
    texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (texture == nullptr) {
        throw std::runtime_error("Could not create font texture from surface.");
    }
}

void GUITextPrinter::queueCharRender(int x, int y, char c, Color color) {
    SDL_Rect clip_to_render = character_rects[c];
    SDL_Rect location{x, y, clip_to_render.w, clip_to_render.h};
    uint8_t r, g, b;
    SDL_GetTextureColorMod(texture, &r, &g, &b);
    SDL_SetTextureColorMod(texture, color.red, color.green, color.blue);
    SDL_RenderCopy(renderer, texture, &clip_to_render, &location);
    SDL_SetTextureColorMod(texture, r, g, b);
}

void GUITextPrinter::queueStringRender(int x, int y, const char *str, Color c) {
    int curr_x = x, curr_y = y;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == '\n') {
            curr_y += CHAR_HEIGHT;
            curr_x = x;
        } else {
            queueCharRender(curr_x, curr_y, str[i], c);
            curr_x += CHAR_WIDTH - 2;
        }
    }
}

GUITextPrinter::~GUITextPrinter() {
    // De-allocates memory used to store sprite sheet texture
    SDL_DestroyTexture(texture);
}
