//
// Created by Samuel Laberge on 3/23/20.
//

#ifndef CHIP8_EMULATOR_SDLINTERFACE_H
#define CHIP8_EMULATOR_SDLINTERFACE_H


#include <SDL.h>
#include "GUITextPrinter.h"

/*
 * Provides a simpler interface to producing graphical output.
 *
 * These functions do not have side-effects. So state like color and render scale
 * are preserved from what they were before the function call. For example,
 * if displayText() is called with a scale of 2, subsequent function calls will NOT have
 * their render scale equal to 2.
 */
class SDLInterface {
    static constexpr uint32_t RED_MASK = 0xFF0000;
    static constexpr uint32_t RED_SHIFT = 16;
    static constexpr uint32_t GREEN_MASK = 0xFF00;
    static constexpr uint32_t GREEN_SHIFT = 8;
    static constexpr uint32_t BLUE_MASK = 0xFF;
    static constexpr uint32_t BLUE_SHIFT = 0;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    GUITextPrinter *printer = nullptr;

    /*
     * Changes the renderer's current color to c.
     * Returns the previous color state the renderer was using.
     */
    uint32_t switchToColor(const Color *c);

    /*
     * Restores the renderer's color state.
     * The parameter is stored in a 32-bit integer which holds the rgb values
     * to restore the color to.
     * The lowest 8 bits are the blue, the next 8 are green, then next 8 are red.
     * See RED, GREEN, and BLUE MASKS above.
     */
    void revertColor(uint32_t prev_color_state);

public:
    /*
     * Creates a new window of size width, height with the given title
     */
    void newWindow(uint32_t width, uint32_t height, const char *title);

    /*
     * Draws the outline of the rectangle defined by the top left corner (x, y) and the given width and height.
     * Can also pass a color to draw this rectangle in
     */
    void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const Color *c = &WHITE);

    /*
     * Overload which takes a SDL_Rect object to define the rectangle
     */
    void drawRect(const SDL_Rect *rect, const Color *c = &WHITE);

    /*
     * Fills in the rectangle defined by the top left corner (x, y) and the given width and height.
     * Can also pass a color to fill this rectangle in with
     */
    void fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const Color *c = &WHITE);

    /*
     * Overload which takes a SDL_Rect object to define the rectangle
     */
    void fillRect(const SDL_Rect *rect, const Color *c = &WHITE);

    /*
     * Displays a string (c-style) at the given x, y with an optional scale and color
     */
    void displayText(uint32_t x, uint32_t y, const char *str, float scale = 1.0f, const Color *c = &WHITE);

    /*
     * Displays a boolean matrix, like the one used by the Chip8VM for graphics.
     * The display will take up the rectangle defined by display_rect.
     * matrix_w and matrix_h define the dimensions of the matrix.
     * Can optionally specify the two colors for the two different pixel states
     */
    void displayMatrix(const SDL_Rect *display_rect, const bool *matrix, uint32_t matrix_w, uint32_t matrix_h,
                       const Color *true_color = &WHITE, const Color *false_color = &BLACK);

    /*
     * Displays all changes made to the window prior to this call.
     * Should only be called once per frame.
     */
    void presentChanges();

    /*
     * Clears the entire window with the given color
     */
    void clear(const Color *clear_color = &WHITE);
};


#endif //CHIP8_EMULATOR_SDLINTERFACE_H
