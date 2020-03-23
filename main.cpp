#include <iostream>
#include <cstdlib>
#include <fstream>
#include <SDL.h>

#include "Chip8.h"
#include "Chip8GUI.h"

char *read_program(const char *file_name);

bool init_sdl();

void init_lookup_tables();


enum KEY_CLASS {
    UNUSED_KEY, HEX_KEY, GUI_KEY
};
// Lookup table to see what type of key is pressed
KEY_CLASS key_class_table[128];
// Lookup table to map hex keyboard key -> hex value to send to emulator
uint8_t hex_keyboard_table[128];

Chip8VM *vm;
static constexpr int WINDOW_WIDTH = 640;
static constexpr int WINDOW_HEIGHT = 480;

SDL_Window *gui_window = nullptr;
SDL_Surface *window_surface = nullptr;

static uint64_t next_time;
static constexpr uint16_t TICK_INTERVAL = 16;

// TODO: Refactor to make SDL GUI into a class. Constructor can handle initialization
//  of window, destructor can handle cleaning up.

uint64_t time_left() {
    uint64_t now = SDL_GetTicks();
    if (next_time <= now)
        return 0;
    return next_time - now;
}

int main(int argc, char **argv) {
//    vm = new Chip8VM(read_program("CONNECT4"), 512);
//    while(true){
//        vm->emulateCycle();
//        vm->printGraphics();
//        usleep(16666u);
//    }
    init_lookup_tables();
    if (!init_sdl()) {
        std::cout << "Failed to initialize SDL" << std::endl;
        return 1;
    }

    vm = new Chip8VM(read_program("C8Programs/VBRIX"), 512);
    auto gui = new Chip8GUI("Chip 8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    vm->emulateCycle();
    gui->newFrame();
    gui->displayVMInfo(vm, true);
    gui->redrawWindow();

    SDL_Event e;
    bool quit = false;
    bool new_cycle = false;
    bool step_mode = true;
    next_time = SDL_GetTicks() + TICK_INTERVAL;
    while (!quit) {
        // Event loop, handle all events in queue until queue is empty
        while (SDL_PollEvent(&e)) {
            // User pressed 'X' in top corner of window
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                auto key_code = e.key.keysym.sym;
                if (key_code == SDLK_SPACE) {
                    new_cycle = true;
                } else if (key_code == SDLK_RETURN) {
                    step_mode = !step_mode;
                } else if (key_code <= 127) {
                    // Ignore keycodes > 127, we won't consider these keys for now
                    // keycodes 0-127 cover standard alphanumeric keys
                    if (key_class_table[key_code] == HEX_KEY) {
                        // Pass hex value to emulator
                        uint8_t hex_val = hex_keyboard_table[key_code];
                        vm->keyPressed(hex_val);
                    } else if (key_class_table[key_code] == GUI_KEY) {
                        // Interpret this key
                    }
                }
            } else if (e.type == SDL_KEYUP) {
                auto key_code = e.key.keysym.sym;
                if (key_code <= 127) {
                    if (key_class_table[key_code] == HEX_KEY) {
                        uint8_t hex_val = hex_keyboard_table[key_code];
                        vm->keyReleased(hex_val);
                    }
                }
            }
        }
        if (!step_mode || new_cycle) {
            vm->emulateCycle();
            gui->newFrame();
            gui->displayVMInfo(vm, step_mode);
            gui->redrawWindow();
            new_cycle = false;
        }
        SDL_Delay(time_left());
        next_time += TICK_INTERVAL;
        //SDL_Delay(10);
    }

    delete gui;
    delete vm;
    SDL_Quit();
}

/**
 * Initializes the SDL window and surface
 * @return true if successful, false if an error occured
 */
bool init_sdl() {
    return SDL_Init(SDL_INIT_VIDEO) >= 0;
}

void init_lookup_tables() {
    memset(key_class_table, KEY_CLASS::UNUSED_KEY, 128 * sizeof(KEY_CLASS));
    memset(hex_keyboard_table, 0, 128);

    // Create hex keyboard layout
    hex_keyboard_table[SDLK_1] = 1;
    key_class_table[SDLK_1] = HEX_KEY;

    hex_keyboard_table[SDLK_2] = 2;
    key_class_table[SDLK_2] = HEX_KEY;

    hex_keyboard_table[SDLK_3] = 3;
    key_class_table[SDLK_3] = HEX_KEY;

    hex_keyboard_table[SDLK_4] = 0xC;
    key_class_table[SDLK_4] = HEX_KEY;

    hex_keyboard_table[SDLK_q] = 4;
    key_class_table[SDLK_q] = HEX_KEY;

    hex_keyboard_table[SDLK_w] = 5;
    key_class_table[SDLK_w] = HEX_KEY;

    hex_keyboard_table[SDLK_e] = 6;
    key_class_table[SDLK_e] = HEX_KEY;

    hex_keyboard_table[SDLK_r] = 0xD;
    key_class_table[SDLK_r] = HEX_KEY;

    hex_keyboard_table[SDLK_a] = 7;
    key_class_table[SDLK_a] = HEX_KEY;

    hex_keyboard_table[SDLK_s] = 8;
    key_class_table[SDLK_s] = HEX_KEY;

    hex_keyboard_table[SDLK_d] = 9;
    key_class_table[SDLK_d] = HEX_KEY;

    hex_keyboard_table[SDLK_f] = 0xE;
    key_class_table[SDLK_f] = HEX_KEY;

    hex_keyboard_table[SDLK_z] = 0xA;
    key_class_table[SDLK_z] = HEX_KEY;

    hex_keyboard_table[SDLK_x] = 0;
    key_class_table[SDLK_x] = HEX_KEY;

    hex_keyboard_table[SDLK_c] = 0xB;
    key_class_table[SDLK_c] = HEX_KEY;

    hex_keyboard_table[SDLK_v] = 0xF;
    key_class_table[SDLK_v] = HEX_KEY;
}

char *read_program(const char *file_name) {
    auto buffer = new char[512];
    std::ifstream input;
    input.open(file_name, std::ios::binary);
    if (input) {
        input.seekg(0, std::ios::beg);
        input.read(buffer, 512);
    } else {
        std::cout << "Failed to open program file" << std::endl;
    }
    return buffer;
}
