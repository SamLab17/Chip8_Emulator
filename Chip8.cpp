//
// Created by Samuel Laberge on 3/19/20.
//

#include <cstring>
#include <stdexcept>
#include <iostream>
#include "Chip8.h"

using namespace C8Constants;

Chip8VM::Chip8VM(void *program_buffer, uint32_t program_size) {
    // Execution starts at address 512
    pc = PROG_START;
    sp = 0;
    i_reg = 0;
    dt_reg = 0xFF;
    st_reg = 0xFF;

    //Clear memory
    memset(memory, 0, MEM_SIZE);
    //Load font into memory
    memcpy(memory, DEFAULT_FONT, 5 * 16);

    clearGraphics();

    //Load program into memory
    for (uint32_t i = 0; i < program_size; i++) {
        memory[i + PROG_START] = ((char *) program_buffer)[i];
        //printf("%x\n", memory[i+PRGM_START]);
    }

    memset(key_status, false, 16 * sizeof(bool));
}

void Chip8VM::emulateCycle() {
    if (pc + 1 >= MEM_SIZE) {
        throw std::out_of_range("PC was out of bounds");
    }
    //Fetch
    uint16_t instruction = (memory[pc] << 8u) | memory[pc + 1];
    //printf("Instruction : %x\n", instruction);
    //printf("PC = %x\n", pc);
    uint8_t first_nibble = (instruction & 0xF000u) >> 12u;
    //printf("First nibble : %x\n", first_nibble);
    // Variable names come from documentation names
    uint16_t nnn = instruction & 0x0FFFu;
    uint8_t n = instruction & 0x000Fu;
    uint8_t x = (instruction & 0x0F00u) >> 8u;
    uint8_t y = (instruction & 0x00F0u) >> 4u;
    uint8_t kk = instruction & 0x00FFu;

    //Decode
    bool advance_pc = true;
    switch (first_nibble) {
        case 0:
            switch (kk) {
                case 0xe0:
                    // Clear the display
                    clearGraphics();
                    break;
                case 0xee:
                    // Return from a subroutine
                    pc = stack[sp--];
                    advance_pc = false;
                    break;
                default:
                    std::cout << "Invalid instruction: " << instruction << std::endl;
                    throw std::invalid_argument("Invalid opcode 0x0");
            }
            break;
        case 1:
            // Jump to location nnn
            pc = nnn;
            advance_pc = false;
            break;
        case 2:
            // Call subroutine at nnn
            sp++;
            if (sp >= STACK_SIZE)
                throw std::overflow_error("Stack overflow");
            stack[sp] = pc + 2;
            pc = nnn;
            advance_pc = false;
            break;
        case 3:
            // Skip next instruction if Vx = kk
            checkRegisterIndex(x);
            if (v_reg[x] == kk)
                pc += 2;
            // Leave advance_pc true so we add another 2 to pc
            break;
        case 4:
            // Skip next instruction if Vx != kk
            checkRegisterIndex(x);
            if (v_reg[x] != kk)
                pc += 2;
            break;
        case 5:
            // Skip next instruction if Vx = Vy
            checkRegisterIndex(x);
            checkRegisterIndex(y);
            if (v_reg[x] == v_reg[y])
                pc += 2;
            break;
        case 6:
            // Set Vx = kk
            checkRegisterIndex(x);
            v_reg[x] = kk;
            break;
        case 7:
            // Set Vx = Vx + kk
            checkRegisterIndex(x);
            v_reg[x] += kk;
            break;
        case 8:
            checkRegisterIndex(x);
            switch (n) {
                case 0:
                    // Set Vx = Vy
                    checkRegisterIndex(y);
                    v_reg[x] = v_reg[y];
                    break;
                case 1:
                    // Set Vx = Vx OR Vy
                    checkRegisterIndex(y);
                    v_reg[x] |= v_reg[y];
                    break;
                case 2:
                    // Set Vx = Vx AND Vy
                    checkRegisterIndex(y);
                    v_reg[x] &= v_reg[y];
                    break;
                case 3:
                    // Set Vx = Vx XOR Vy
                    checkRegisterIndex(y);
                    v_reg[x] ^= v_reg[y];
                    break;
                case 4:
                    // Set Vx = Vx + Vy, set VF = carry
                    checkRegisterIndex(y);
                    if (v_reg[x] + v_reg[y] < v_reg[x])
                        v_reg[0xF] = 1;
                    else
                        v_reg[0xF] = 0;
                    v_reg[x] += v_reg[y];
                    break;
                case 5:
                    // Set Vx = Vx - Vy, set VF = NOT borrow
                    checkRegisterIndex(y);
                    if (v_reg[x] > v_reg[y])
                        v_reg[0xF] = 1;
                    else
                        v_reg[0xF] = 0;
                    v_reg[x] -= v_reg[y];
                    break;
                case 6:
                    // Set Vx = Vx SHR 1
                    v_reg[0xF] = v_reg[x] & 0x1u;
                    v_reg[x] >>= 1u;
                    break;
                case 7:
                    // Set Vx = Vy - Vx, set VF = NOT borrow
                    checkRegisterIndex(y);
                    if (v_reg[y] > v_reg[x])
                        v_reg[0xF] = 1;
                    else
                        v_reg[0xF] = 0;
                    v_reg[x] = v_reg[y] - v_reg[x];
                    break;
                case 0xE:
                    // Set Vx = Vx SHL 1
                    v_reg[0xF] = v_reg[x] & 0x1u;
                    v_reg[x] <<= 1u;
                    break;
                default:
                    throw std::invalid_argument("n value is incorrect");
            }
            break;
        case 9:
            // Skip next instruction if Vx != Vy
            checkRegisterIndex(x);
            checkRegisterIndex(y);
            if (v_reg[x] != v_reg[y])
                pc += 2;
            break;
        case 0xA:
            // Set I = nnn
            i_reg = nnn;
            break;
        case 0xB:
            // Jump to location nnn + V0
            pc = nnn + v_reg[0];
            advance_pc = false;
            break;
        case 0xC:
            // Set Vx = random byte AND kk
            checkRegisterIndex(x);
            v_reg[x] = u8_dist(rand_generator) & kk;
            break;
        case 0xD: {
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
            checkRegisterIndex(x);
            checkRegisterIndex(y);
            uint8_t sprite[n];
            for (int i = 0; i < n; i++)
                sprite[i] = memory[i_reg + i];
            drawSprite(sprite, n, v_reg[x], v_reg[y]);
            break;
        }
        case 0xE:
            // Keyboard input
            //throw std::invalid_argument("0xE not implemented yet.");
            switch (kk) {
                case 0x9E:
                    // Skip next instruction if key with the value of Vx is pressed
                    checkRegisterIndex(x);
                    if (key_status[v_reg[x]])
                        pc += 2;
                    break;
                case 0xA1:
                    // Skip next instruction if key with the value of Vx is not pressed
                    checkRegisterIndex(x);
                    if (!key_status[v_reg[x]])
                        pc += 2;
                    break;
                default:
                    throw std::invalid_argument("invalid opcode 0xE");
            }
            break;
        case 0xF:
            switch (kk) {
                case 0x07:
                    // Set Vx = delay timer value
                    checkRegisterIndex(x);
                    v_reg[x] = dt_reg;
                    //std::cout<<"Set v["<<0+x<<"] to dt_reg = "<<0 + dt_reg<<std::endl;
                    break;
                case 0x0A:
                    // Wait for a key press, store the value of the key in Vx
                    checkRegisterIndex(x);
                    advance_pc = false;
                    for (int i = 0; i < 16; i++) {
                        if (key_status[i]) {
                            v_reg[x] = i;
                            advance_pc = true;
                            break;
                        }
                    }
                    break;
                case 0x15:
                    // Set delay timer = Vx
                    checkRegisterIndex(x);
                    dt_reg = v_reg[x];
                    break;
                case 0x18:
                    // Set sound timer = Vx
                    checkRegisterIndex(x);
                    st_reg = v_reg[x];
                    break;
                case 0x1E:
                    // Set I = I + Vx
                    checkRegisterIndex(x);
                    i_reg += v_reg[x];
                    break;
                case 0x29:
                    // Set I = location of sprite for digit Vx.
                    checkRegisterIndex(x);
                    i_reg = 5 * v_reg[x];
                    break;
                case 0x33: {
                    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    checkRegisterIndex(x);
                    uint8_t hundreds = v_reg[x] / 100;
                    uint8_t tens = (v_reg[x] % 100) / 10;
                    uint8_t ones = (v_reg[x] % 10);
                    //std::cout << "Writing register BCD to memory" << std::endl;
                    memory[i_reg] = hundreds;
                    memory[i_reg + 1] = tens;
                    memory[i_reg + 2] = ones;
                    break;
                }
                case 0x55:
                    // Store registers V0 through Vx in memory starting at location I.
                    checkRegisterIndex(x);
                    //std::cout << "Writing registers to memory" << std::endl;
                    for (int i = 0; i <= x; i++)
                        memory[i_reg + i] = v_reg[i];
                    break;
                case 0x65:
                    // Read registers V0 through Vx from memory starting at location I.
                    checkRegisterIndex(x);
                    for (int i = 0; i <= x; i++)
                        v_reg[i] = memory[i_reg + i];
                    break;
                default:
                    throw std::invalid_argument("Invalid opcode 0xF");
            }
            break;
        default:
            throw std::invalid_argument("first_nibble value is incorrect");
    }

    //Execute
    if (advance_pc)
        pc += 2;

    if (dt_reg)
        dt_reg--;
    if (st_reg)
        st_reg--;
}

void Chip8VM::checkRegisterIndex(uint16_t index) {
    if (index >= NUM_V_REGS)
        throw std::invalid_argument("Invalid register index");
}

void Chip8VM::clearGraphics() {
    memset(graphics, 0, GRAPHICS_SIZE);
    draw_flag = true;
}

void Chip8VM::drawSprite(const uint8_t *sprite, uint8_t n, uint8_t x, uint8_t y) {
    v_reg[0xF] = 0;
    // Loop through the lines of the sprite
    for (int i = 0; i < n; i++) {
        // Iterate left to right through the bits of the line byte
        for (int bit = 7; bit >= 0; bit--) {
            int curr_x = (x + (7 - bit)) % SCREEN_WIDTH;
            int curr_y = (y + i) % SCREEN_HEIGHT;

            bool px = (sprite[i] & (1u << bit)) != 0;
            bool prev_px = graphics[curr_y][curr_x];
            graphics[curr_y][curr_x] = px != prev_px;
            if (!graphics[curr_y][curr_x] && prev_px)
                v_reg[0xF] = 1;
        }
    }
    draw_flag = true;
}

void Chip8VM::printGraphics() {
    if (draw_flag) {
        for (int r = 0; r < SCREEN_HEIGHT; r++) {
            for (int c = 0; c < SCREEN_WIDTH; c++) {
                if (graphics[r][c])
                    std::cout << "#";
                else
                    std::cout << "-";
            }
            std::cout << std::endl;
        }
        draw_flag = false;
    }
}

void Chip8VM::keyPressed(uint8_t key_val) {
    if (key_val >= 16)
        throw std::invalid_argument("Invalid key code passed to keyPressed.");
    key_status[key_val] = true;
}

void Chip8VM::keyReleased(uint8_t key_val) {
    if (key_val >= 16)
        throw std::invalid_argument("Invalid key code passed to keyReleased.");
    key_status[key_val] = false;
}

//
//void Chip8VM::print_regs(){
//    printf("V Reg: ");
//    for(const auto& value: v_reg){
//        printf("%02x ", value);
//    }
//    printf("- I Reg: %0x ", i_reg);
//    printf("- dt_reg: %0x ", dt_reg);
//    printf("- st_reg: %0x ", st_reg);
//    printf("\n");
//}