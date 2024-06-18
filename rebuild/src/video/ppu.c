#include "ppu.h"

#include <logging/logging.h>

int init_ppu(struct PPU *ppu) {

    log_info("Initialized PPU!");

    return 0;
}

void destroy_ppu(struct PPU *ppu) {
    
    log_info("Destroyed PPU!");
}
