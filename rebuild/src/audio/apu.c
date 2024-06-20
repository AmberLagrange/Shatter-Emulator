#include "apu.h"

#include <stdlib.h>
#include <logging/logging.h>

int init_apu(struct APU **apu) {

    *apu = malloc(sizeof(struct APU));

    if (!*apu) {
        return INIT_FAIL;
    }

    gameboy_log(LOG_DEBUG, "Initialized APU!");
    return RETURN_OK;
}

void destroy_apu(struct APU *apu) {
    
    free(apu);
    gameboy_log(LOG_DEBUG, "Destroyed APU!");
}
