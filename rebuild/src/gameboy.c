#include "gameboy.h"

#include "audio/apu.h"
#include "cpu/cpu.h"
#include "logging/logging.h"
#include "memory/mmu.h"

int init_gameboy(struct Gameboy *gb) {
    
    gb->apu = init_apu();
    if (!gb->apu) {
        goto cleanup_apu;
    }

    gb->cpu = init_cpu();
    if (!gb->cpu) {
        goto cleanup_cpu;
    }

    gb->mmu = init_mmu();
    if (!gb->mmu) {
        goto cleanup_mmu;
    }

    gb->ppu = init_ppu();
    if (!gb->ppu) {
        goto cleanup_ppu;
    }

    gb->running = 0;

    goto init_success;

    cleanup_ppu:
    destroy_ppu(gb->ppu);

    cleanup_mmu:
    destroy_mmu(gb->mmu);

    cleanup_cpu:
    destroy_cpu(gb->cpu);

    cleanup_apu:
    destroy_apu(gb->apu);

    init_fail:
    return INIT_FAIL;

    init_success:
    return RETURN_OK;
}

void destroy_gameboy(struct Gameboy *gb) {

    destroy_ppu(gb->ppu);
    destroy_mmu(gb->mmu);
    destroy_cpu(gb->cpu);
    destroy_apu(gb->apu);
}

void start_gameboy(struct Gameboy* gb) {
    
    gameboy_log(LOG_DEBUG, "Starting Gameboy:");
    reset_cpu(gb->cpu);
    gb->running = 1;
}