#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <audio/apu.h>
#include <cpu/cpu.h>
#include <cartridge/cartridge.h>
#include <memory/bus.h>
#include <memory/mmu.h>
#include <video/ppu.h>

struct Gameboy {
    
    struct Bus bus;
    
    struct APU apu;
    struct CPU cpu;
    struct MMU mmu;
    struct PPU ppu;
    
    struct Cartridge cart;

    int running;
};

/**
 * @brief Initializes the Gameboy
 * 
 * @param gb 
 * @return int 
 */
int init_gameboy(struct Gameboy *gb);

/**
 * @brief Cleans up the Gameboy
 * 
 * @param gb 
 */
void cleanup_gameboy(struct Gameboy *gb);

/**
 * @brief Start running the Gameboy
 * 
 * @param gb 
 */
void start_gameboy(struct Gameboy *gb);

#endif // GAMEBOY_H
