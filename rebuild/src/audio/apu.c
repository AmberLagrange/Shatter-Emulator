#include "apu.h"

#include <logging/logging.h>

int init_apu(struct APU *apu) {

    gameboy_log(LOG_DEBUG, "Initialized APU!");
    return RETURN_OK;
}

void cleanup_apu(struct APU *apu) {
    
    gameboy_log(LOG_DEBUG, "Destroyed APU!");
}
