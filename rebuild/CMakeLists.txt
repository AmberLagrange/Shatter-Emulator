cmake_minimum_required(VERSION 3.29.6)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
project(Shatter)

find_package(SDL2 REQUIRED)
include_directories("${PROJECT_NAME}" ${SDL2_INCLUDE_DIRS} src include)

add_executable("${PROJECT_NAME}"
    src/main.c src/gameboy.c
    src/audio/apu.c
    src/cpu/cpu.c
    src/memory/mmu.c
    src/video/ppu.c
    src/logging/logging.c)

target_link_libraries("${PROJECT_NAME}" ${SDL2_LIBRARIES})
