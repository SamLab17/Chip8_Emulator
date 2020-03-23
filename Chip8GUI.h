//
// Created by Samuel Laberge on 3/20/20.
//

#ifndef CHIP8_EMULATOR_CHIP8GUI_H
#define CHIP8_EMULATOR_CHIP8GUI_H

#include <cstdint>
#include <SDL.h>

#include "Color.h"
#include "FontSprites.h"
#include "Chip8.h"

// TODO: Load a font as a Sprite sheet. Create a mapping between ASCII code -> location in sprite sheet.
// TODO: Create a general displayString function which renders character by character to a given x, y
// TODO: Print out registers to screen



class Chip8GUI {
private:
    static constexpr uint32_t WINDOW_WIDTH = 1000;
    static constexpr uint32_t WINDOW_HEIGHT = 600;
    static constexpr uint32_t VM_SCREEN_WIDTH = 640;
    static constexpr uint32_t VM_SCREEN_HEIGHT = 320;
    static constexpr uint32_t FRAME_WIDTH = 10;

    static constexpr SDL_Point VM_SCREEN_ORIGIN{FRAME_WIDTH, FRAME_WIDTH};
    static constexpr SDL_Point MEMORY_PANE_ORIGIN{
            FRAME_WIDTH + VM_SCREEN_WIDTH + FRAME_WIDTH,
            FRAME_WIDTH
    };
    static constexpr SDL_Point REGISTER_PANE_ORIGIN{
            FRAME_WIDTH,
            FRAME_WIDTH + VM_SCREEN_HEIGHT + FRAME_WIDTH
    };

    static constexpr Color BACKGROUND{40, 44, 52};
    static constexpr Color FRAME{33, 37, 43};
    static constexpr Color FRAME_LINE{96, 99, 104};
    static constexpr Color PIXEL_ON = BLACK;
    static constexpr Color PIXEL_OFF = WHITE;

    SDL_Window *gui_window = nullptr;
    SDL_Surface *window_surface = nullptr;
    SDL_Renderer *renderer = nullptr;
    FontSprites *fontSprites;

    Color setColor(const Color &);

    void queueRectRender(const Color &, SDL_Rect);

    void displayRegionAroundAddr(int x, int y, uint16_t addr, const uint8_t *mem, Color highlight_color);

public:
    Chip8GUI(const char *title, uint32_t x, uint32_t y);

    void displayRegisters(const uint8_t *v_regs, uint16_t i_reg, uint8_t dt, uint8_t st, uint16_t pc, uint8_t sp,
                          uint16_t op);

    void displayMemoryPane(const uint8_t *mem, uint16_t pc, uint16_t i_reg);

    void displayGraphics(const bool *graphics);

    void redrawWindow() {
        SDL_RenderPresent(renderer); // Should only be called once per frame
    }

    ~Chip8GUI();

    void newFrame();

    void writeStatus(bool is_paused);

    void displayVMInfo(Chip8VM *chip8_vm, bool is_paused);
};


#endif //CHIP8_EMULATOR_CHIP8GUI_H
