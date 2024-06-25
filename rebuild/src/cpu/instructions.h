#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

// Temp
#include <stdbool.h>

enum Opcode {
    OPCODE_NOP              = 0x00,
    OPCODE_DEC_B            = 0x05,
    OPCODE_LD_B_U8          = 0x06,
    OPCODE_DEC_C            = 0x0D,
    OPCODE_LD_C_U8          = 0x0E,

    OPCODE_JP_NZ_I8         = 0x20,
    OPCODE_LD_HL_U16        = 0x21,

    OPCODE_IND_HLD_A        = 0x32,
    OPCODE_LD_A_U8          = 0x3E,

    OPCODE_XOR_A_A          = 0xAF,

    OPCODE_JP_U16           = 0xC3,
    OPCODE_CB_PREFIX        = 0xCB,

    OPCODE_DI               = 0xF3,
};

enum CB_Opcode {
    TODO = 0
};

struct Gameboy;

/**
 * @brief Execute the next opcode
 * 
 * @param gb 
 * @return true 
 * @return false 
 */
bool execute_opcode(struct Gameboy *gb);

/**
 * @brief Execute a CB prefixed opcode
 * 
 * @param gb 
 * @param cb_opcode 
 * @return true 
 * @return false 
 */
bool execute_cb_opcode(struct Gameboy *gb, enum CB_Opcode cb_opcode);

#endif // INSTRUCTIONS_H
