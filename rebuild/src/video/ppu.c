#include "ppu.h"

#include <stdlib.h>
#include <logging/logging.h>

int init_ppu(struct PPU **ppu) {

    *ppu = malloc(sizeof(struct PPU));

    if (!*ppu) {
        return INIT_FAIL;
    }

    gameboy_log(LOG_DEBUG, "Initialized PPU!");
    return RETURN_OK;
}

void destroy_ppu(struct PPU *ppu) {
    
    free(ppu);
    gameboy_log(LOG_DEBUG, "Destroyed PPU!");
}
