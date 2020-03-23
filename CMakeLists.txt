cmake_minimum_required(VERSION 3.15)
project(Chip8_Emulator)

set(CMAKE_CXX_STANDARD 17)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(Google_tests)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories("${gtest_SOURCE_DIR}/include")

#set(SOURCE_FILES main.cpp Chip8.cpp Chip8.h Chip8GUI.cpp Chip8GUI.h VMTests.cpp Chip8Test.cpp Chip8Test.h)
set(BASE_SOURCE_FILES Chip8.cpp Chip8.h)
set(GUI_SOURCE_FILES main.cpp Chip8GUI.cpp Chip8GUI.h Chip8Constants.h Color.h FontSprites.cpp FontSprites.h)
set(TEST_FILES VMTests.cpp)

add_executable(Chip8_Emulator ${BASE_SOURCE_FILES} ${GUI_SOURCE_FILES})
add_executable(Chip8_Tester ${BASE_SOURCE_FILES} ${TEST_FILES})
target_link_libraries(Chip8_Emulator ${SDL2_LIBRARY})
target_link_libraries(Chip8_Tester gtest_main)

