#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

enum Opcode {
    OPCODE_NOP              = 0x00,
    OPCODE_LD_B_U8          = 0x06,
    OPCODE_LD_C_U8          = 0x0E,

    OPCODE_JP_NZ_I8         = 0x20,
    OPCODE_LD_HL_U16        = 0x21,

    OPCODE_IND_HLD_A        = 0x32,

    OPCODE_XOR_A_A          = 0xAF,

    OPCODE_JP_U16           = 0xC3,
    OPCODE_CB_PREFIX        = 0xCB,
};

enum CB_Opcode {
    TODO = 0
};

struct Gameboy;

/**
 * @brief Execute the next opcode
 * 
 * @param gb 
 */
void execute_opcode(struct Gameboy *gb);

/**
 * @brief Execute a sb prefixed opcode
 * 
 * @param gb 
 */
__attribute__((always_inline)) void execute_cb_opcode(struct Gameboy *gb, enum CB_Opcode cb_opcode);

#endif // INSTRUCTIONS_H
