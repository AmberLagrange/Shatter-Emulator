#include "gameboy.h"

#include <logging/logging.h>

int init_gameboy(struct Gameboy *gb) {
    
    if (init_bus(&gb->bus)) {
        gameboy_log(LOG_CRITICAL, "Failed to initialize Bus!");
        goto bus_init_fail;
    }

    if (init_apu(&gb->apu)) {
        gameboy_log(LOG_CRITICAL, "Failed to initialize APU!");
        goto apu_init_fail;
    }

    if (init_cpu(&gb->cpu)) {
        gameboy_log(LOG_CRITICAL, "Failed to initialize CPU!");
        goto cpu_init_fail;
    }

    if (init_mmu(&gb->mmu)) {
        gameboy_log(LOG_CRITICAL, "Failed to initialize MMU!");
        goto mmu_init_fail;
    }

    if (init_ppu(&gb->ppu)) {
        gameboy_log(LOG_CRITICAL, "Failed to initialize PPPU!");
        goto ppu_init_fail;
    }

    set_mmu(&gb->bus, &gb->mmu);
    gb->running = 0;

    goto init_success;

    ppu_init_fail:
    cleanup_mmu(&gb->mmu);

    mmu_init_fail:
    cleanup_cpu(&gb->cpu);

    cpu_init_fail:
    cleanup_apu(&gb->apu);

    apu_init_fail:
    cleanup_bus(&gb->bus);

    bus_init_fail:
    return INIT_FAIL;

    init_success:
    return RETURN_OK;
}

void cleanup_gameboy(struct Gameboy *gb) {

    cleanup_cartridge(&gb->cart);
    cleanup_ppu(&gb->ppu);
    cleanup_mmu(&gb->mmu);
    cleanup_cpu(&gb->cpu);
    cleanup_apu(&gb->apu);
    cleanup_bus(&gb->bus);
}

void start_gameboy(struct Gameboy* gb) {
    
    gameboy_log(LOG_DEBUG, "Starting Gameboy:");
    reset_cpu(&gb->cpu);
    gb->running = 1;
}