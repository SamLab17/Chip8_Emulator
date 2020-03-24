//
// Created by Samuel Laberge on 3/23/20.
//

#include "DetailedGUI.h"

DetailedGUI::DetailedGUI(Controller *controller, Chip8VM *vm) {
    this->controller = controller;
    this->vm = vm;
    is_paused = true;
    step = false;
    sdl = new SDLInterface();
    sdl->newWindow(WINDOW_RECT.w, WINDOW_RECT.h, WINDOW_TITLE);
    drawWindow();
}

void DetailedGUI::drawWindow() {
    drawBorders();
    drawVMScreen();
    drawMemoryPane();
    drawRegisterPane();
}

void DetailedGUI::drawBorders() {
    // Draw top border
    sdl->fillRect(0, 0, WINDOW_RECT.w, WINDOW_RECT.h, &BORDER);
    //Draw bottom border
    sdl->fillRect(0, WINDOW_RECT.h - BORDER_WIDTH, WINDOW_RECT.w, BORDER_WIDTH, &BORDER);
    // Draw left border
    sdl->fillRect(0, BORDER_WIDTH, BORDER_WIDTH, WINDOW_RECT.h - 2 * BORDER_WIDTH, &BORDER);
    //Draw right border
    sdl->fillRect(WINDOW_RECT.w - BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH, WINDOW_RECT.h - 2 * BORDER_WIDTH, &BORDER);
}

void DetailedGUI::drawVMScreen() {
    sdl->displayMatrix(&VM_SCREEN_RECT, vm->getGraphics(), C8Constants::SCREEN_WIDTH, C8Constants::SCREEN_HEIGHT);
    if (is_paused) {
        sdl->displayText(VM_SCREEN_RECT.x + VM_SCREEN_RECT.w - 55,
                         VM_SCREEN_RECT.y + VM_SCREEN_RECT.h - 15, "PAUSED", 1.0, &RED);
    }
    sdl->drawRect(&VM_SCREEN_RECT, &BORDER_LINE);
}

void DetailedGUI::drawMemoryPane() {
    sdl->fillRect(&MEM_PANE_RECT, &BACKGROUND);
    sdl->drawRect(&MEM_PANE_RECT, &BORDER_LINE);
    sdl->displayText(MEM_PANE_RECT.x + 4, MEM_PANE_RECT.y + 6, "Memory", 2.0);
    displayMemoryAroundAddr(MEM_PANE_RECT.x + 4, MEM_PANE_RECT.y + 40, vm->getPC(), vm->getMemory(), RED);
    displayMemoryAroundAddr(MEM_PANE_RECT.x + 150, MEM_PANE_RECT.y + 40, vm->getIRegister(), vm->getMemory(), BLUE);
}

void DetailedGUI::displayMemoryAroundAddr(int x, int y, uint16_t addr, const uint8_t *mem, Color highlight_color) {
    int num_lines_around_addr = 16;
    int y_inc = 16;
    for (int i = num_lines_around_addr; i >= 1; i--) {
        if (addr - i >= 0) {
            char buffer[20];
            sprintf(buffer, "0x%04x : 0x%02x", addr - i, mem[addr - i]);
            sdl->displayText(x, y, buffer, 1.0, &WHITE);
            y += y_inc;
        }
    }

    if (addr < C8Constants::MEM_SIZE) {
        char buffer_at_pc[20];
        sprintf(buffer_at_pc, "0x%04x : 0x%02x", addr, mem[addr]);
        sdl->displayText(x, y, buffer_at_pc, 1.0, &highlight_color);
        y += y_inc;
    }

    for (int i = 1; i <= num_lines_around_addr; i++) {
        if (addr + i < C8Constants::MEM_SIZE) {
            char buffer[20];
            sprintf(buffer, "0x%04x : 0x%02x", addr + i, mem[addr + i]);
            sdl->displayText(x, y, buffer, 1.0, &WHITE);
            y += y_inc;
        }
    }
}

void DetailedGUI::drawRegisterPane() {
    sdl->fillRect(&REG_PANE_RECT, &BACKGROUND);
    sdl->drawRect(&REG_PANE_RECT, &BORDER_LINE);
    sdl->displayText(REG_PANE_RECT.x + 2, REG_PANE_RECT.y + 6, "Registers", 2.0);

    char buffer[256];
    const uint8_t *v_regs = vm->getVRegisters();

    sprintf(buffer,
            "V0: 0x%02x  V1: 0x%02x  V2: 0x%02x  V3: 0x%02x\nV4: 0x%02x  V5: 0x%02x  V6: 0x%02x  V7: 0x%02x\nV8: 0x%02x  V9: 0x%02x  VA: 0x%02x  VB: 0x%02x\nVC: 0x%02x  VD: 0x%02x  VE: 0x%02x  VF: 0x%02x",
            v_regs[0], v_regs[1], v_regs[2], v_regs[3], v_regs[4], v_regs[5], v_regs[6], v_regs[7], v_regs[8],
            v_regs[9],
            v_regs[0xA], v_regs[0xB], v_regs[0xC], v_regs[0xD], v_regs[0xE], v_regs[0xF]);
    sdl->displayText(REG_PANE_RECT.x + 4, REG_PANE_RECT.y + 40, buffer, 2.0);

    memset(buffer, 0, 256);
    sprintf(buffer, " I: 0x%04x          SP: 0x%02x\nDT: 0x%02x            ST: 0x%02x\nPC: 0x%04x          Op: 0x%04x",
            vm->getIRegister(), vm->getStackPointer(), vm->getDelayTimer(), vm->getSoundTimer(),
            vm->getPC(), vm->getOpCode());
    sdl->displayText(REG_PANE_RECT.x + 4, REG_PANE_RECT.y + 140, buffer, 2.0);
}

void DetailedGUI::drawNextFrame() {
    if (!is_paused || (is_paused && step)) {
        vm->emulateCycle();
        step = false;
    }
    drawWindow();
    sdl->presentChanges();
}

void DetailedGUI::processEvent(SDL_Event &e) {
    if (e.type == SDL_QUIT) {
        // Quit the program with no error code
        controller->quit();
    } else if (e.type == SDL_KEYDOWN) {
        // A key was pressed down
        int key_code = e.key.keysym.sym;
        if (hex_map.isValidHexKey(key_code)) {
            vm->keyPressed(hex_map.hexValue(key_code));
        } else if (key_code == SDLK_ESCAPE || key_code == SDLK_RETURN) {
            is_paused = !is_paused;
        } else if (key_code == SDLK_SPACE && is_paused) {
            step = true;
        }
    } else if (e.type == SDL_KEYUP) {
        // A key was released
        int key_code = e.key.keysym.sym;
        if (hex_map.isValidHexKey(key_code)) {
            vm->keyReleased(hex_map.hexValue((key_code)));
        }
    }
    //If not one of the above, then do nothing.

}


