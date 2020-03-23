//
// Created by Samuel Laberge on 3/21/20.
//

#include "FontSprites.h"
#include "Color.h"
#include <stdexcept>

FontSprites::FontSprites(SDL_Renderer *renderer) {
    this->renderer = renderer;
    loadTexture();
    //Initialize character rect look up table
    init_rect_table();

}

void FontSprites::init_rect_table() {
    int x = 0, y = 0;
    for (int i = 0; i < 256; i++) {
        SDL_Rect *curr = &(character_rects[i]);
        curr->x = (x % 16) * CHAR_WIDTH;
        curr->y = (y / 16) * CHAR_HEIGHT;
        curr->w = CHAR_WIDTH;
        curr->h = CHAR_HEIGHT;
        x++;
        y++;
    }
}

void FontSprites::loadTexture() {
    SDL_Surface *loaded_surface = SDL_LoadBMP(SPRITE_SHEET_FILE_NAME);
    if (loaded_surface == nullptr) {
        throw std::runtime_error("Could not find fonts file.");
    }
    texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    if (texture == nullptr) {
        throw std::runtime_error("Could not create font texture from surface.");
    }
}

void FontSprites::queueCharRender(int x, int y, char c, Color color) {
    SDL_Rect clip_to_render = character_rects[c];
    SDL_Rect location{x, y, clip_to_render.w, clip_to_render.h};
    uint8_t r, g, b;
    SDL_GetTextureColorMod(texture, &r, &g, &b);
    SDL_SetTextureColorMod(texture, color.red, color.green, color.blue);
    SDL_RenderCopy(renderer, texture, &clip_to_render, &location);
    SDL_SetTextureColorMod(texture, r, g, b);
    //SDL_RenderPresent(renderer);
}

void FontSprites::queueStringRender(int x, int y, const char *str, int len, Color c) {
    int curr_x = x, curr_y = y;
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

FontSprites::~FontSprites() {
    SDL_DestroyTexture(texture);
}
