#include "ppu.h"

#include <logging/logging.h>

int init_ppu(struct PPU *ppu) {

    gameboy_log(LOG_DEBUG, "Initialized PPU!");
    return RETURN_OK;
}

void cleanup_ppu(struct PPU *ppu) {
    
    gameboy_log(LOG_DEBUG, "Destroyed PPU!");
}

void tick_ppu(struct PPU *ppu) {

    // TODO:
}
