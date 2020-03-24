//
// Created by Samuel Laberge on 3/23/20.
//

#ifndef CHIP8_EMULATOR_DETAILEDGUI_H
#define CHIP8_EMULATOR_DETAILEDGUI_H


#include "C8GUI.h"
#include "Color.h"
#include "Chip8VM.h"
#include "Controller.h"
#include "SDLInterface.h"
#include "HexKeyboardMap.h"

class DetailedGUI : public C8GUI {
private:
    static constexpr const char *WINDOW_TITLE = "Detailed Chip 8 View";
    static constexpr int BORDER_WIDTH = 10;
    static constexpr SDL_Rect WINDOW_RECT{0, 0, 1000, 600};
    static constexpr SDL_Rect VM_SCREEN_RECT{BORDER_WIDTH, BORDER_WIDTH, 640, 320};
    static constexpr SDL_Rect MEM_PANE_RECT{
            VM_SCREEN_RECT.x + VM_SCREEN_RECT.w + BORDER_WIDTH,
            BORDER_WIDTH,
            WINDOW_RECT.w - VM_SCREEN_RECT.w - 3 * BORDER_WIDTH,
            WINDOW_RECT.h - 2 * BORDER_WIDTH};
    static constexpr SDL_Rect REG_PANE_RECT{
            BORDER_WIDTH,
            VM_SCREEN_RECT.y + VM_SCREEN_RECT.h + BORDER_WIDTH,
            VM_SCREEN_RECT.w,
            WINDOW_RECT.h - VM_SCREEN_RECT.h - 3 * BORDER_WIDTH
    };

    static constexpr Color BACKGROUND{40, 44, 52};
    static constexpr Color BORDER{33, 37, 43};
    static constexpr Color BORDER_LINE{96, 99, 104};

    bool is_paused;
    bool step;

    Controller *controller;
    HexKeyboardMap hex_map{};
    Chip8VM *vm;
    SDLInterface *sdl;

    void drawWindow();

    void drawBorders();

    void drawVMScreen();

    void drawMemoryPane();

    void drawRegisterPane();

    void displayMemoryAroundAddr(int x, int y, uint16_t addr, const uint8_t *mem, Color highlight_color);

public:
    DetailedGUI(Controller *controller, Chip8VM *vm);

    void drawNextFrame() override;

    void processEvent(SDL_Event &e) override;

    ~DetailedGUI();
};


#endif //CHIP8_EMULATOR_DETAILEDGUI_H
