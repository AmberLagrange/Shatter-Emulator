#include "cpu.h"

#include <stdlib.h>
#include <logging/logging.h>

struct CPU *create_cpu() {

    struct CPU *cpu = malloc(sizeof(struct CPU));

    if (!cpu) {
        return NULL;
    }

    gameboy_log(LOG_DEBUG, "Initialized CPU!");
    return cpu;
}

void destroy_cpu(struct CPU *cpu) {
    
    free(cpu);
    gameboy_log(LOG_DEBUG, "Destroyed CPU!");
}

void reset_cpu(struct CPU *cpu) {

    cpu->registers.af = AF_RESET;
    cpu->registers.bc = BC_RESET;
    cpu->registers.de = DE_RESET;
    cpu->registers.hl = HL_RESET;
    cpu->registers.sp = SP_RESET;
    cpu->registers.pc = PC_RESET;

    gameboy_log(LOG_DEBUG, "CPU reset sequence:");
    gameboy_log(LOG_DEBUG, "\tAF: 0x%04X", cpu->registers.af);
    gameboy_log(LOG_DEBUG, "\tBC: 0x%04X", cpu->registers.bc);
    gameboy_log(LOG_DEBUG, "\tDE: 0x%04X", cpu->registers.de);
    gameboy_log(LOG_DEBUG, "\tHL: 0x%04X", cpu->registers.hl);
    gameboy_log(LOG_DEBUG, "\tSP: 0x%04X", cpu->registers.sp);
    gameboy_log(LOG_DEBUG, "\tPC: 0x%04X", cpu->registers.pc);
}
