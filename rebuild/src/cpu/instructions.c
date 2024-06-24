#include "instructions.h"

// Temp
#include <stdlib.h>

#include <audio/apu.h>
#include <gameboy.h>
#include <memory/bus.h>
#include <video/ppu.h>

#include <logging/logging.h>

#define M_CYCLE_TICK do {                           \
    tick_apu(&gb->apu);                             \
    tick_ppu(&gb->ppu);                             \
} while(0)

#define READ_NEXT_BYTE do {                         \
    set_address(&gb->bus, gb->cpu.registers.pc);    \
    read_byte(&gb->bus);                            \
    gb->cpu.registers.pc++;                         \
} while (0)

#define FETCH_CYCLE do {                            \
    set_address(&gb->bus, gb->cpu.registers.pc);    \
    read_byte(&gb->bus);                            \
    gb->cpu.registers.pc++;                         \
} while (0)

enum Flags {
    FLAG_ZERO       = 7,
    FLAG_NEGATIVE   = 6,
    FLAG_HALF       = 5,
    FLAG_CARRY      = 4,
};

#define SET_FLAG(flag) (gb->cpu.registers.f |= (1 << flag))
#define CLEAR_FLAG(flag) (gb->cpu.registers.f &= ~(1 << flag))
#define TOGGLE_FLAG(flag) (gb->cpu.registers.f ^= (1 << flag))

#define LD_R_U8(reg) do {   \
    /* M1 */                \
    M_CYCLE_TICK;           \
    READ_NEXT_BYTE;         \
    byte = gb->bus.data;    \
                            \
    /* M2 */                \
    M_CYCLE_TICK;           \
    reg = byte;             \
    FETCH_CYCLE;            \
} while (0)

void execute_opcode(struct Gameboy *gb) {

    u8 byte;
    u8 low_byte;
    u8 high_byte;

    enum Opcode opcode = (enum Opcode)gb->bus.data;

    switch (opcode) {

        case OPCODE_NOP:

            M_CYCLE_TICK;
            FETCH_CYCLE;
            return;

        case OPCODE_LD_B_U8:

            LD_R_U8(gb->cpu.registers.b);
            return;

        case OPCODE_LD_C_U8:

            LD_R_U8(gb->cpu.registers.c);
            return;

        case OPCODE_LD_HL_U16:

            // M1
            M_CYCLE_TICK;
            READ_NEXT_BYTE;
            low_byte = gb->bus.data;

            // M2
            M_CYCLE_TICK;
            READ_NEXT_BYTE;
            high_byte = gb->bus.data;

            // M3
            M_CYCLE_TICK;
            gb->cpu.registers.hl = (((u16)high_byte << 8) | low_byte);
            FETCH_CYCLE;
            return;

        case OPCODE_XOR_A_A:

            // M1
            M_CYCLE_TICK;
            gb->cpu.registers.a ^= gb->cpu.registers.a;
            (gb->cpu.registers.a == 0) ? SET_FLAG(FLAG_ZERO) : CLEAR_FLAG(FLAG_ZERO);
            CLEAR_FLAG(FLAG_NEGATIVE);
            CLEAR_FLAG(FLAG_HALF);
            CLEAR_FLAG(FLAG_CARRY);
            FETCH_CYCLE;
            return;

        case OPCODE_JP_U16:

            // M1
            M_CYCLE_TICK;
            READ_NEXT_BYTE;
            low_byte = gb->bus.data;

            // M2
            M_CYCLE_TICK;
            READ_NEXT_BYTE;
            high_byte = gb->bus.data;
            
            // M3
            M_CYCLE_TICK;
            gb->cpu.registers.pc = (((u16)high_byte << 8) | low_byte);

            // M4
            M_CYCLE_TICK;
            FETCH_CYCLE;
            return;

        default:
            gameboy_log(LOG_CRITICAL, "Unhandled opcode: 0x%02X", (u8)opcode);
            abort();

    }

}

__attribute__((always_inline)) void execute_cb_opcode(struct Gameboy *gb, enum CB_Opcode cb_opcode) {
    
    switch (cb_opcode) {
        
        default:
            gameboy_log(LOG_CRITICAL, "Unhandled CB opcode: 0x%02X", (u8)cb_opcode);
            abort();
    }

}
