cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
project(Shatter)

find_package(SDL2 REQUIRED)
include_directories("${PROJECT_NAME}" ${SDL2_INCLUDE_DIRS} src include)

add_executable("${PROJECT_NAME}"
    src/audio/apu.cpp
    src/cart/mbc.cpp src/cart/romonly.cpp src/cart/mbc1.cpp src/cart/mbc3.cpp src/cart/mbc5.cpp
    src/cpu/cpu.cpp src/cpu/instruction_cb.cpp src/cpu/instruction.cpp src/cpu/registers.cpp src/cpu/timer.cpp
    src/logging/logger.cpp
    src/video/ppu.cpp src/video/screen.cpp
    src/flags.cpp src/gameboy.cpp src/joypad.cpp src/main.cpp src/mmu.cpp)

target_precompile_headers(Shatter PRIVATE include/core.hpp)

target_link_libraries("${PROJECT_NAME}" ${SDL2_LIBRARIES})
