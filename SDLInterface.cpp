/*
 * Chip 8 Emulator & Interpreter
 * Copyright (c) 2020 Samuel Laberge.
 */

#include <iostream>
#include "SDLInterface.h"

// Helper function for creating a SDL_Rect struct
SDL_Rect *create_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    auto *rect = new SDL_Rect;
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    return rect;
}

/* Public interface methods */

void SDLInterface::newWindow(uint32_t width, uint32_t height, const char *title) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        throw std::runtime_error("Could not initialize GUI window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << SDL_GetError() << std::endl;
        throw std::runtime_error("Could not create SDL Renderer");
    }

    printer = new GUITextPrinter(renderer);
    if (!printer) {
        throw std::runtime_error("Failed to create GUI printer");
    }
}

void SDLInterface::drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const Color *c) {
    SDL_Rect *rect = create_rect(x, y, w, h);
    drawRect(rect, c);
    delete rect;
}

void SDLInterface::drawRect(const SDL_Rect *rect, const Color *c) {
    if (rect == nullptr)
        throw std::invalid_argument("rect passed into drawRect was null.");
    uint32_t prev = switchToColor(c);
    SDL_RenderDrawRect(renderer, rect);
    revertColor(prev);
}

void SDLInterface::fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const Color *c) {
    SDL_Rect *rect = create_rect(x, y, w, h);
    fillRect(rect, c);
    delete rect;
}

void SDLInterface::fillRect(const SDL_Rect *rect, const Color *c) {
    if (rect == nullptr)
        throw std::invalid_argument("rect passed into fillRect was null.");
    uint32_t prev = switchToColor(c);
    SDL_RenderFillRect(renderer, rect);
    revertColor(prev);
}

void SDLInterface::displayText(uint32_t x, uint32_t y, const char *str, float scale, const Color *c) {
    // Prevent divide by zero error
    if (scale == 0)
        return;
    if (!str)
        throw std::invalid_argument("str passed into displayText was null.");

    uint32_t scaled_x = x / scale;
    uint32_t scaled_y = y / scale;
    uint32_t len = strlen(str);
    uint32_t prev_color = switchToColor(c);

    // Store previous scale state
    float prev_scale_x, prev_scale_y;
    SDL_RenderGetScale(renderer, &prev_scale_x, &prev_scale_y);
    SDL_RenderSetScale(renderer, scale, scale);

    printer->queueStringRender(scaled_x, scaled_y, str, c);

    //Restore scale and color state
    SDL_RenderSetScale(renderer, prev_scale_x, prev_scale_y);
    revertColor(prev_color);
}

void
SDLInterface::displayMatrix(const SDL_Rect *display_rect, const bool *matrix, uint32_t matrix_w, uint32_t matrix_h,
                            const Color *true_color,
                            const Color *false_color) {
    if (!matrix)
        throw std::invalid_argument("matrix passed to displayMatrix was null.");
    if (!display_rect)
        throw std::invalid_argument("display_rect passed to displayMatrix was null.");

    /*
     * Fill in the display rect with the false color, so we only have to
     * draw "true' cells after this
    */
    fillRect(display_rect, false_color);

    // The dimensions of a single pixel
    int px_width = (display_rect->w) / matrix_w;
    int px_height = (display_rect->h) / matrix_h;

    // Loop through matrix elements
    for (int r = 0; r < matrix_h; r++) {
        for (int c = 0; c < matrix_w; c++) {
            // Define pixel sub-rectangle inside of display_rect
            SDL_Rect pixel_rect{display_rect->x + c * px_width, display_rect->y + r * px_height,
                                px_width, px_height};
            // Fill in rect with appropriate color
            if (matrix[r * matrix_w + c]) {
                fillRect(&pixel_rect, true_color);
            }
        }
    }
}

void SDLInterface::clear(const Color *clear_color) {
    uint32_t prev_color = switchToColor(clear_color);
    SDL_RenderClear(renderer);
    revertColor(prev_color);
}

void SDLInterface::presentChanges() {
    SDL_RenderPresent(renderer);
}

/* Helper methods for switching the color of the renderer */
void SDLInterface::revertColor(uint32_t prev_color_state) {
    uint8_t r = (prev_color_state & RED_MASK) >> RED_SHIFT;
    uint8_t g = (prev_color_state & GREEN_MASK) >> GREEN_SHIFT;
    uint8_t b = (prev_color_state & BLUE_MASK) >> BLUE_SHIFT;
    SDL_SetRenderDrawColor(renderer, r, g, b, 1);
}

uint32_t SDLInterface::switchToColor(const Color *c) {
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, c->red, c->green, c->blue, 1);
    uint32_t prev_state = (r << RED_SHIFT) | (g << GREEN_SHIFT) | (b << BLUE_SHIFT);
    return prev_state;
}



