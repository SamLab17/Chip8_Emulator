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

    /*
     * Pointer to the SDL_Renderer used to render to the
     * GUI window.
     */
    SDL_Renderer *renderer;

    /*
     * Lookup table which maps a character's ASCII value to
     * a Rect defining where that character's sprite is located in the
     * sprite sheet.
     */
    SDL_Rect character_rects[256];

    // Initializes the above table.
    void init_rect_table();

    /*
     * In-memory representation of the sprite sheet image.
     * Clips of this texture will be used to display characters
     */
    SDL_Texture *texture;

    // Loads the sprite sheet into memory
    void loadTexture();

public:

    /*
     * Constructor which takes in the GUI window's renderer.
     * Will initialize the lookup table, read in the sprite sheet image
     * using the file name defined above.
     */
    explicit GUITextPrinter(SDL_Renderer *);

    /*
     * Prints a single character, c, to the screen at position x, y in a given color
     */
    void queueCharRender(int x, int y, char c, Color color = WHITE);

    /*
     * Prints a string at a position x, y in a given color
     */
    void queueStringRender(int x, int y, const char *str, Color c = WHITE);

    /*
     * Destructor
     * De-allocates the sprite sheet texture which was read in
     */
    ~GUITextPrinter();
};


#endif //CHIP8_EMULATOR_GUITEXTPRINTER_H
