#include "apu.h"

#include <stdlib.h>
#include <logging/logging.h>

struct APU *init_apu() {

    struct APU *apu = malloc(sizeof(struct APU));

    if (!apu) {
        return NULL;
    }

    gameboy_log(LOG_DEBUG, "Initialized APU!");
    return apu;
}

void destroy_apu(struct APU *apu) {
    
    free(apu);
    gameboy_log(LOG_DEBUG, "Destroyed APU!");
}
