#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

/* Temp */
#include <stdbool.h>

enum Opcode {

    OPCODE_NOP              = 0x00,
    OPCODE_LD_BC_U16        = 0x01,
    OPCODE_LD_IND_BC_A      = 0x02,
    OPCODE_INC_BC           = 0x03,
    OPCODE_INC_B            = 0x04,
    OPCODE_DEC_B            = 0x05,
    OPCODE_LD_B_U8          = 0x06,
    OPCODE_LD_A_IND_BC      = 0x0A,
    OPCODE_INC_C            = 0x0C,
    OPCODE_DEC_C            = 0x0D,
    OPCODE_LD_C_U8          = 0x0E,

    OPCODE_LD_DE_U16        = 0x11,
    OPCODE_LD_IND_DE_A      = 0x12,
    OPCODE_INC_DE           = 0x13,
    OPCODE_INC_D            = 0x14,
    OPCODE_DEC_D            = 0x15,
    OPCODE_LD_D_U8          = 0x16,
    OPCODE_JR_I8            = 0x18,
    OPCODE_INC_E            = 0x1C,
    OPCODE_DEC_E            = 0x1D,
    OPCODE_LD_E_U8          = 0x1E,
    OPCODE_RRA              = 0x1F,

    OPCODE_JR_NZ_I8         = 0x20,
    OPCODE_LD_HL_U16        = 0x21,
    OPCODE_LD_IND_HLI_A     = 0x22,
    OPCODE_INC_HL           = 0x23,
    OPCODE_INC_H            = 0x24,
    OPCODE_DEC_H            = 0x25,
    OPCODE_LD_H_U8          = 0x26,
    OPCODE_DAA              = 0x27,
    OPCODE_LD_A_IND_HLI     = 0x2A,
    OPCODE_INC_L            = 0x2C,
    OPCODE_DEC_L            = 0x2D,
    OPCODE_LD_L_U8          = 0x2E,

    OPCODE_JR_NC_I8         = 0x30,
    OPCODE_LD_SP_U16        = 0x31,
    OPCODE_LD_IND_HLD_A     = 0x32,
    OPCODE_SCF              = 0x37,
    OPCODE_JR_C_I8          = 0x38,
    OPCODE_ADD_HL_SP        = 0x39,
    OPCODE_INC_A            = 0x3C,
    OPCODE_DEC_A            = 0x3D,
    OPCODE_LD_A_U8          = 0x3E,

    OPCODE_LD_B_B           = 0x40,
    OPCODE_LD_B_C           = 0x41,
    OPCODE_LD_B_D           = 0x42,
    OPCODE_LD_B_E           = 0x43,
    OPCODE_LD_B_H           = 0x44,
    OPCODE_LD_B_L           = 0x45,
    OPCODE_LD_B_IND_HL      = 0x46,
    OPCODE_LD_B_A           = 0x47,
    OPCODE_LD_C_B           = 0x48,
    OPCODE_LD_C_C           = 0x49,
    OPCODE_LD_C_D           = 0x4A,
    OPCODE_LD_C_E           = 0x4B,
    OPCODE_LD_C_H           = 0x4C,
    OPCODE_LD_C_L           = 0x4D,
    OPCODE_LD_C_IND_HL      = 0x4E,
    OPCODE_LD_C_A           = 0x4F,

    OPCODE_LD_D_B           = 0x50,
    OPCODE_LD_D_C           = 0x51,
    OPCODE_LD_D_D           = 0x52,
    OPCODE_LD_D_E           = 0x53,
    OPCODE_LD_D_H           = 0x54,
    OPCODE_LD_D_L           = 0x55,
    OPCODE_LD_D_IND_HL      = 0x56,
    OPCODE_LD_D_A           = 0x57,
    OPCODE_LD_E_B           = 0x58,
    OPCODE_LD_E_C           = 0x59,
    OPCODE_LD_E_D           = 0x5A,
    OPCODE_LD_E_E           = 0x5B,
    OPCODE_LD_E_H           = 0x5C,
    OPCODE_LD_E_L           = 0x5D,
    OPCODE_LD_E_IND_HL      = 0x5E,
    OPCODE_LD_E_A           = 0x5F,

    OPCODE_LD_H_B           = 0x60,
    OPCODE_LD_H_C           = 0x61,
    OPCODE_LD_H_D           = 0x62,
    OPCODE_LD_H_E           = 0x63,
    OPCODE_LD_H_H           = 0x64,
    OPCODE_LD_H_L           = 0x65,
    OPCODE_LD_H_IND_HL      = 0x66,
    OPCODE_LD_H_A           = 0x67,
    OPCODE_LD_L_B           = 0x68,
    OPCODE_LD_L_C           = 0x69,
    OPCODE_LD_L_D           = 0x6A,
    OPCODE_LD_L_E           = 0x6B,
    OPCODE_LD_L_H           = 0x6C,
    OPCODE_LD_L_L           = 0x6D,
    OPCODE_LD_L_IND_HL      = 0x6E,
    OPCODE_LD_L_A           = 0x6F,

    OPCODE_LD_IND_HL_B      = 0x70,
    OPCODE_LD_IND_HL_C      = 0x71,
    OPCODE_LD_IND_HL_D      = 0x72,
    OPCODE_LD_IND_HL_E      = 0x73,
    OPCODE_LD_IND_HL_H      = 0x74,
    OPCODE_LD_IND_HL_L      = 0x75,
    OPCODE_HALT             = 0x76,
    OPCODE_LD_IND_HL_A      = 0x77,
    OPCODE_LD_A_B           = 0x78,
    OPCODE_LD_A_C           = 0x79,
    OPCODE_LD_A_D           = 0x7A,
    OPCODE_LD_A_E           = 0x7B,
    OPCODE_LD_A_H           = 0x7C,
    OPCODE_LD_A_L           = 0x7D,
    OPCODE_LD_A_IND_HL      = 0x7E,
    OPCODE_LD_A_A           = 0x7F,

    OPCODE_ADD_B            = 0x80,
    OPCODE_ADD_C            = 0x81,
    OPCODE_ADD_D            = 0x82,
    OPCODE_ADD_E            = 0x83,
    OPCODE_ADD_H            = 0x84,
    OPCODE_ADD_L            = 0x85,
    OPCODE_ADD_IND_HL       = 0x86,
    OPCODE_ADD_A            = 0x87,
    OPCODE_ADC_B            = 0x88,
    OPCODE_ADC_C            = 0x89,
    OPCODE_ADC_D            = 0x8A,
    OPCODE_ADC_E            = 0x8B,
    OPCODE_ADC_H            = 0x8C,
    OPCODE_ADC_L            = 0x8D,
    OPCODE_ADC_IND_HL       = 0x8E,
    OPCODE_ADC_A            = 0x8F,

    OPCODE_SUB_B            = 0x90,
    OPCODE_SUB_C            = 0x91,
    OPCODE_SUB_D            = 0x92,
    OPCODE_SUB_E            = 0x93,
    OPCODE_SUB_H            = 0x94,
    OPCODE_SUB_L            = 0x95,
    OPCODE_SUB_IND_HL       = 0x96,
    OPCODE_SUB_A            = 0x97,
    OPCODE_SBC_B            = 0x98,
    OPCODE_SBC_C            = 0x99,
    OPCODE_SBC_D            = 0x9A,
    OPCODE_SBC_E            = 0x9B,
    OPCODE_SBC_H            = 0x9C,
    OPCODE_SBC_L            = 0x9D,
    OPCODE_SBC_IND_HL       = 0x9E,
    OPCODE_SBC_A            = 0x9F,

    OPCODE_AND_B            = 0xA0,
    OPCODE_AND_C            = 0xA1,
    OPCODE_AND_D            = 0xA2,
    OPCODE_AND_E            = 0xA3,
    OPCODE_AND_H            = 0xA4,
    OPCODE_AND_L            = 0xA5,
    OPCODE_AND_IND_HL       = 0xA6,
    OPCODE_AND_A            = 0xA7,
    OPCODE_XOR_B            = 0xA8,
    OPCODE_XOR_C            = 0xA9,
    OPCODE_XOR_D            = 0xAA,
    OPCODE_XOR_E            = 0xAB,
    OPCODE_XOR_H            = 0xAC,
    OPCODE_XOR_L            = 0xAD,
    OPCODE_XOR_IND_HL       = 0xAE,
    OPCODE_XOR_A            = 0xAF,

    OPCODE_OR_B             = 0xB0,
    OPCODE_OR_C             = 0xB1,
    OPCODE_OR_D             = 0xB2,
    OPCODE_OR_E             = 0xB3,
    OPCODE_OR_H             = 0xB4,
    OPCODE_OR_L             = 0xB5,
    OPCODE_OR_IND_HL        = 0xB6,
    OPCODE_OR_A             = 0xB7,
    OPCODE_CP_B             = 0xB8,
    OPCODE_CP_C             = 0xB9,
    OPCODE_CP_D             = 0xBA,
    OPCODE_CP_E             = 0xBB,
    OPCODE_CP_H             = 0xBC,
    OPCODE_CP_L             = 0xBD,
    OPCODE_CP_IND_HL        = 0xBE,
    OPCODE_CP_A             = 0xBF,

    OPCODE_POP_BC           = 0xC1,
    OPCODE_JP_U16           = 0xC3,
    OPCODE_PUSH_BC          = 0xC5,
    OPCODE_ADD_U8           = 0xC6,
    OPCODE_RET              = 0xC9,
    OPCODE_CB_PREFIX        = 0xCB,
    OPCODE_CALL             = 0xCD,

    OPCODE_POP_DE           = 0xD1,
    OPCODE_PUSH_DE          = 0xD5,
    OPCODE_RST_0x18         = 0xDF,

    OPCODE_LDH_A_U8         = 0xE0,
    OPCODE_POP_HL           = 0xE1,
    OPCODE_PUSH_HL          = 0xE5,
    OPCODE_AND_U8           = 0xE6,
    OPCODE_LD_IND_U16_A     = 0xEA,

    OPCODE_LDH_U8_A         = 0xF0,
    OPCODE_POP_AF           = 0xF1,
    OPCODE_DI               = 0xF3,
    OPCODE_PUSH_AF          = 0xF5,
    OPCODE_LD_SP_HL         = 0xF9,
    OPCODE_LD_A_NN          = 0xFA,
    OPCODE_CP_U8            = 0xFE,
    OPCODE_RST_0x38         = 0xFF,
};

enum CB_Opcode {
    
    CB_OPCODE_RR_B          = 0x18,
    CB_OPCODE_RR_C          = 0x19,
    CB_OPCODE_RR_D          = 0x1A,
    CB_OPCODE_RR_E          = 0x1B,
    CB_OPCODE_RR_H          = 0x1C,
    CB_OPCODE_RR_L          = 0x1D,
    CB_OPCODE_RR_IND_HL     = 0x1E,
    CB_OPCODE_RR_A          = 0x1F,

    CB_OPCODE_SWAP_A        = 0x37,
    CB_OPCODE_SRL_B         = 0x38,
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
 * @return true 
 * @return false 
 */
__attribute__((always_inline)) bool execute_cb_opcode(struct Gameboy *gb);

#endif /* INSTRUCTIONS_H */
