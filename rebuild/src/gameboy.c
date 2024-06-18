#include "gameboy.h"
#include "video/ppu.h"

int init_gameboy(struct Gameboy *gb) {
    
    if (init_apu(gb->apu)) {
        goto cleanup_apu;
    }

    if (init_cpu(gb->cpu)) {
        goto cleanup_cpu;
    }

    if (init_ppu(gb->ppu)) {
        goto cleanup_ppu;
    }

    gb->running = 0;

    goto init_success;

    cleanup_ppu:
    destroy_ppu(gb->ppu);

    cleanup_cpu:
    destroy_cpu(gb->cpu);

    cleanup_apu:
    destroy_apu(gb->apu);

    init_fail:
    return 1;

    init_success:
    return 0;
}

void destroy_gameboy(struct Gameboy *gb) {

    destroy_ppu(gb->ppu);
    destroy_cpu(gb->cpu);
    destroy_apu(gb->apu);
}

void start_gameboy(struct Gameboy* gb) {
    
    gb->running = 1;
}