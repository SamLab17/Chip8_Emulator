/*
 * Chip 8 Emulator & Interpreter
 * Copyright (c) 2020 Samuel Laberge.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "Controller.h"


int main(int argc, char **argv) {
    auto *c = new Controller("C8Programs/PONG");
    c->start(true);
    // Once emulation has ended (window was closed), we will return here
    return 0;
}
