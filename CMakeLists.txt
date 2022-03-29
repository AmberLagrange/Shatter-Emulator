cmake_minimum_required(VERSION 3.16.3)

set (CMAKE_CXX_STANDARD 20)
project(Shatter)

find_package(SDL2 REQUIRED)
include_directories("${PROJECT_NAME}" ${SDL2_INCLUDE_DIRS} src include)

add_executable("${PROJECT_NAME}"
    src/audio/apu.cpp
    src/cart/cartridge.cpp
    src/cpu/cpu.cpp src/cpu/instruction_cb.cpp src/cpu/instruction.cpp src/cpu/registers.cpp src/cpu/timer.cpp
    src/logging/logger.cpp
    src/video/ppu.cpp src/video/screen.cpp
    src/gameboy.cpp src/main.cpp src/mmu.cpp src/scheduler.cpp)

target_link_libraries("${PROJECT_NAME}" ${SDL2_LIBRARIES})
