#include "cpu.h"

#include <logging/logging.h>

int init_cpu(struct CPU *cpu) {

    gameboy_log(LOG_DEBUG, "Initialized CPU!");
    return RETURN_OK;
}

void cleanup_cpu(struct CPU *cpu) {
    
    gameboy_log(LOG_DEBUG, "Destroyed CPU!");
}

void reset_cpu(struct CPU *cpu) {

    gameboy_log(LOG_DEBUG, "CPU reset sequence:");

    cpu->ime = false;

    cpu->registers.af = AF_RESET;
    gameboy_log(LOG_DEBUG, "\tAF: 0x%04X", cpu->registers.af);
    
    cpu->registers.bc = BC_RESET;
    gameboy_log(LOG_DEBUG, "\tBC: 0x%04X", cpu->registers.bc);
    
    cpu->registers.de = DE_RESET;
    gameboy_log(LOG_DEBUG, "\tDE: 0x%04X", cpu->registers.de);
    
    cpu->registers.hl = HL_RESET;
    gameboy_log(LOG_DEBUG, "\tHL: 0x%04X", cpu->registers.hl);
    
    cpu->registers.sp = SP_RESET;
    gameboy_log(LOG_DEBUG, "\tSP: 0x%04X", cpu->registers.sp);
    
    cpu->registers.pc = PC_RESET;
    gameboy_log(LOG_DEBUG, "\tPC: 0x%04X", cpu->registers.pc);
}
