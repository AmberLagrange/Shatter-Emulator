#include "ppu.h"

#include <stdlib.h>
#include <logging/logging.h>

struct PPU *init_ppu() {

    struct PPU *ppu = malloc(sizeof(struct PPU));

    if (!ppu) {
        return NULL;
    }

    gameboy_log(LOG_DEBUG, "Initialized PPU!");
    return ppu;
}

void destroy_ppu(struct PPU *ppu) {
    
    free(ppu);
    gameboy_log(LOG_DEBUG, "Destroyed PPU!");
}
