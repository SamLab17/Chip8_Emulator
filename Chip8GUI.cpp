//
// Created by Samuel Laberge on 3/20/20.
//

#include "Chip8GUI.h"
#include "FontSprites.h"
#include "Chip8Constants.h"
#include <stdexcept>
#include <iostream>

Chip8GUI::Chip8GUI(const char *title, uint32_t x, uint32_t y) {
    gui_window = SDL_CreateWindow(title, x, y, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (gui_window == nullptr) {
        throw std::runtime_error("Could not initialize GUI window");
    }
    window_surface = SDL_GetWindowSurface(gui_window);
    if (window_surface == nullptr) {
        throw std::runtime_error("Could fetch window surface");
    }

    renderer = SDL_GetRenderer(gui_window);
    if (renderer == nullptr) {
        throw std::runtime_error("Could not create SDL Renderer");
    }

    fontSprites = new FontSprites(renderer);
    newFrame();

}

Chip8GUI::~Chip8GUI() {
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(window_surface);
    SDL_DestroyWindow(gui_window);
}

/**
 * Sets the color of the SDL Renderer to the passed in color
 * @param c The color to change the draw renderer to
 * @return the previous color being used by the renderer
 */
Color Chip8GUI::setColor(const Color &c) {
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, c.red, c.green, c.blue, SDL_ALPHA_OPAQUE);
    return Color{r, g, b};
}

void Chip8GUI::queueRectRender(const Color &c, SDL_Rect rect) {
    setColor(c);
    SDL_RenderFillRect(renderer, &rect);
}

void Chip8GUI::displayRegisters(const uint8_t *v_regs, uint16_t i_reg, uint8_t dt, uint8_t st, uint16_t pc, uint8_t sp,
                                uint16_t op) {
    char buffer[256];
    sprintf(buffer,
            "V0: 0x%02x  V1: 0x%02x  V2: 0x%02x  V3: 0x%02x\nV4: 0x%02x  V5: 0x%02x  V6: 0x%02x  V7: 0x%02x\nV8: 0x%02x  V9: 0x%02x  VA: 0x%02x  VB: 0x%02x\nVC: 0x%02x  VD: 0x%02x  VE: 0x%02x  VF: 0x%02x",
            v_regs[0], v_regs[1], v_regs[2], v_regs[3], v_regs[4], v_regs[5], v_regs[6], v_regs[7], v_regs[8],
            v_regs[9],
            v_regs[0xA], v_regs[0xB], v_regs[0xC], v_regs[0xD], v_regs[0xE], v_regs[0xF]);
    SDL_RenderSetScale(renderer, 2, 2);
    fontSprites->queueStringRender(REGISTER_PANE_ORIGIN.x / 2 + 2, REGISTER_PANE_ORIGIN.y / 2 + 20, buffer,
                                   strlen(buffer));

    memset(buffer, 0, 256);
    sprintf(buffer, " I: 0x%04x          SP: 0x%02x\nDT: 0x%02x            ST: 0x%02x\nPC: 0x%04x          Op: 0x%04x",
            i_reg, sp, dt, st, pc, op);
    fontSprites->queueStringRender(REGISTER_PANE_ORIGIN.x / 2 + 2, REGISTER_PANE_ORIGIN.y / 2 + 70, buffer,
                                   strlen(buffer));

    SDL_RenderSetScale(renderer, 1, 1);
}

void Chip8GUI::newFrame() {
    //Draw outer frame
    setColor(FRAME);
    SDL_Rect full_window_rect{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &full_window_rect);

    //Draw inner frames
    SDL_Rect memory_divider{VM_SCREEN_WIDTH, 0, 10, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &memory_divider);
    SDL_Rect reg_divider{0, VM_SCREEN_HEIGHT, VM_SCREEN_WIDTH, 10};
    SDL_RenderFillRect(renderer, &reg_divider);

    //Draw pane backgrounds
    setColor(BACKGROUND);
    SDL_Rect mem_rect{MEMORY_PANE_ORIGIN.x, MEMORY_PANE_ORIGIN.y, WINDOW_WIDTH - MEMORY_PANE_ORIGIN.x - FRAME_WIDTH,
                      WINDOW_HEIGHT - MEMORY_PANE_ORIGIN.y - FRAME_WIDTH};
    SDL_RenderFillRect(renderer, &mem_rect);

    SDL_Rect reg_rect{REGISTER_PANE_ORIGIN.x, REGISTER_PANE_ORIGIN.y, VM_SCREEN_WIDTH,
                      WINDOW_HEIGHT - REGISTER_PANE_ORIGIN.y - FRAME_WIDTH};
    SDL_RenderFillRect(renderer, &reg_rect);
    //Draw VM Screen Background
    setColor(BLACK);
    SDL_Rect vm_rect{VM_SCREEN_ORIGIN.x, VM_SCREEN_ORIGIN.y, VM_SCREEN_WIDTH, VM_SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &vm_rect);

    setColor(FRAME_LINE);
    SDL_RenderDrawRect(renderer, &mem_rect);
    SDL_RenderDrawRect(renderer, &reg_rect);
    SDL_RenderDrawRect(renderer, &vm_rect);


    SDL_RenderSetScale(renderer, 2, 2);
    fontSprites->queueStringRender(MEMORY_PANE_ORIGIN.x / 2 + 2, MEMORY_PANE_ORIGIN.y / 2 + 2, "Memory View", 11);
    fontSprites->queueStringRender(REGISTER_PANE_ORIGIN.x / 2 + 2, REGISTER_PANE_ORIGIN.y / 2 + 2, "Registers", 9);
    SDL_RenderSetScale(renderer, 1, 1);
}

void Chip8GUI::displayMemoryPane(const uint8_t *mem, uint16_t pc, uint16_t i_reg) {
    //Display 25 lines from memory around the pc, 12 above, 12 below pc location
    int curr_x = MEMORY_PANE_ORIGIN.x + 4;
    int curr_y = MEMORY_PANE_ORIGIN.y + 40;
    displayRegionAroundAddr(MEMORY_PANE_ORIGIN.x + 4, MEMORY_PANE_ORIGIN.y + 40, pc, mem, RED);
    displayRegionAroundAddr(MEMORY_PANE_ORIGIN.x + 150, MEMORY_PANE_ORIGIN.y + 40, i_reg, mem, BLUE);
}

void Chip8GUI::displayRegionAroundAddr(int x, int y, uint16_t addr, const uint8_t *mem, Color highlight_color) {
    int num_lines_around_addr = 16;
    int y_inc = 16;
    for (int i = num_lines_around_addr; i >= 1; i--) {
        if (addr - i >= 0) {
            char buffer[20];
            sprintf(buffer, "0x%04x : 0x%02x", addr - i, mem[addr - i]);
            fontSprites->queueStringRender(x, y, buffer, strlen(buffer));
            y += y_inc;
        }
    }

    if (addr < C8Constants::MEM_SIZE) {
        char buffer_at_pc[20];
        sprintf(buffer_at_pc, "0x%04x : 0x%02x", addr, mem[addr]);
        fontSprites->queueStringRender(x, y, buffer_at_pc, strlen(buffer_at_pc), highlight_color);
        y += y_inc;
    }

    for (int i = 1; i <= num_lines_around_addr; i++) {
        if (addr + i < C8Constants::MEM_SIZE) {
            char buffer[20];
            sprintf(buffer, "0x%04x : 0x%02x", addr + i, mem[addr + i]);
            fontSprites->queueStringRender(x, y, buffer, strlen(buffer));
            y += y_inc;
        }
    }
}

void Chip8GUI::displayGraphics(const bool *graphics) {
    constexpr int PIXEL_WIDTH = VM_SCREEN_WIDTH / C8Constants::SCREEN_WIDTH;
    constexpr int PIXEL_HEIGHT = VM_SCREEN_HEIGHT / C8Constants::SCREEN_HEIGHT;
    for (int r = 0; r < C8Constants::SCREEN_HEIGHT; r++) {
        for (int c = 0; c < C8Constants::SCREEN_WIDTH; c++) {
            SDL_Rect pixel_rect{VM_SCREEN_ORIGIN.x + c * PIXEL_WIDTH, VM_SCREEN_ORIGIN.y + r * PIXEL_HEIGHT,
                                PIXEL_WIDTH, PIXEL_HEIGHT};
            if (graphics[r * C8Constants::SCREEN_WIDTH + c]) {
                queueRectRender(PIXEL_ON, pixel_rect);
            } else {
                queueRectRender(PIXEL_OFF, pixel_rect);
            }
        }
    }
}

void Chip8GUI::writeStatus(bool is_paused) {
    if (is_paused) {
        fontSprites->queueStringRender(VM_SCREEN_ORIGIN.x + VM_SCREEN_WIDTH - 55,
                                       VM_SCREEN_ORIGIN.y + VM_SCREEN_HEIGHT - 15, "PAUSED", 6, RED);
    }
}

void Chip8GUI::displayVMInfo(Chip8VM *vm, bool is_paused) {
    displayRegisters(vm->getVRegisters(), vm->getIRegister(), vm->getDelayTimer(), vm->getSoundTimer(), vm->getPC(),
                     vm->getStackPointer(), vm->getOpCode());
    displayMemoryPane(vm->getMemory(), vm->getPC(), vm->getIRegister());
    displayGraphics(vm->getGraphics());
    writeStatus(is_paused);
}
