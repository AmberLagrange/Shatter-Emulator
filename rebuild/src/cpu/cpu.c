#include "cpu.h"

#include <logging/logging.h>

int init_cpu(struct CPU *cpu) {

    log_info("Initialized CPU!");

    return 0;
}

void destroy_cpu(struct CPU *cpu) {
    
    log_info("Destroyed CPU!");
}
