#include "apu.h"

#include <logging/logging.h>

int init_apu(struct APU *apu) {

    log_info("Initialized APU!");

    return 0;
}

void destroy_apu(struct APU *apu) {
    
    log_info("Destroyed APU!");
}
