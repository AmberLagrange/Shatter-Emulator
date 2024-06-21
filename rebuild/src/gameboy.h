#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <audio/apu.h>
#include <cpu/cpu.h>
#include <memory/mmu.h>
#include <video/ppu.h>

struct Gameboy {
    
    struct APU *apu;
    struct CPU *cpu;
    struct MMU *mmu;
    struct PPU *ppu;

    int running;
};

int  init_gameboy(struct Gameboy *gb);
void destroy_gameboy(struct Gameboy *gb);

void start_gameboy(struct Gameboy *gb);

#endif // GAMEBOY_H
