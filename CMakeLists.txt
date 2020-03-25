cmake_minimum_required(VERSION 3.15)
project(Chip8_Emulator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Google Unit Tests
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.gtest googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif ()

# SDL Package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories("${gtest_SOURCE_DIR}/include")

# Base files for using the Chip8 VM
set(VM_SOURCE source/Chip8VM.cpp source/Chip8VM.h)

# Source files for running the GUI executable
set(GUI_SOURCE_FILES
        source/GUIMain.cpp
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
add_executable(emulator ${VM_SOURCE} ${GUI_SOURCE_FILES})

# Unit Test Executable
add_executable(tester ${VM_SOURCE} ${TEST_FILES})

target_link_libraries(emulator ${SDL2_LIBRARY})
target_link_libraries(tester gtest_main)


