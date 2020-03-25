cmake_minimum_required(VERSION 3.15)
project(Chip8_Emulator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Google Unit Tests
# Project should have a directory under Chip8_Emulator named Google_tests
add_subdirectory(Google_tests)

# SDL Package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories("${gtest_SOURCE_DIR}/include")

# Base files for using the Chip8 VM
set(VM_SOURCE source/Chip8VM.cpp source/Chip8VM.h)

# Source files for running the GUI executable
set(GUI_SOURCE_FILES
        source/main.cpp
        source/DetailedGUI.cpp
        source/DetailedGUI.h
        source/Chip8Constants.h
        source/Color.h
        source/GUITextPrinter.cpp
        source/GUITextPrinter.h
        source/C8GUI.h
        source/SimpleGUI.cpp
        source/SimpleGUI.h
        source/HexKeyboardMap.cpp
        source/HexKeyboardMap.h
        source/Controller.cpp
        source/Controller.h
        source/SDLInterface.cpp
        source/SDLInterface.h
        source/DetailedGUI.cpp
        source/DetailedGUI.h
        )

# Source files for running Unit tests
set(TEST_FILES source/VMTests.cpp)

# GUI Exectuable
add_executable(Chip8_Emulator ${VM_SOURCE} ${GUI_SOURCE_FILES})

# Unit Test Executable
add_executable(Chip8_Tester ${VM_SOURCE} ${TEST_FILES})

target_link_libraries(Chip8_Emulator ${SDL2_LIBRARY})
target_link_libraries(Chip8_Tester gtest_main)


