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
#include <getopt.h>
#include "Controller.h"

/*
 * Prints the command line usage of this program.
 * Can have optional option flags followed by a required
 * path to a Chip 8 Program
 */
void print_usage() {
    std::cout << "./emulator [options] program_path" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-h                   :   Display this message" << std::endl;
    std::cout << "-d                   :   Use a detailed GUI window" << std::endl;
    std::cout << "-v                   :   Show version info" << std::endl;
}

/*
 * Prints out the version information if -v is passed in
 */
void print_version_info() {
    std::cout << "Chip 8 Emulator & Interpreter v1.0" << std::endl;
    std::cout << "Copyright (c) 2020 Samuel Laberge." << std::endl;
}


int main(int argc, char **argv) {
    // Missing required program argument
    if (argc < 2) {
        print_usage();
        return 1;
    }

    // Parse optional option arguments
    int c;
    bool detailed_view = false;
    while ((c = getopt(argc, argv, "hvd")) != -1) {
        switch (c) {
            case 'h':
                print_usage();
                return 0;
            case 'v':
                print_version_info();
                return 0;
            case 'd':
                detailed_view = true;
                break;
            case '?':
                std::cout << "Unknown option character: " << optopt << std::endl;
                print_usage();
                return 1;
            default:
                return 1;
        }
    }

    // Check to see if program name was included in arguments
    if (optind == argc) {
        std::cout << "Missing program name argument." << std::endl;
        print_usage();
        return 1;
    }
    char *program_name = argv[optind];

    // Create Controller object and start emulation
    auto *controller = new Controller(program_name);
    controller->start(detailed_view);
    // Once emulation has ended (window was closed), we will return here
    return 0;
}
