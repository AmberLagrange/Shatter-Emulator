#include <cpu/instructions.h>

#include <cpu/instructions_helper.h>

#include <audio/apu.h>
#include <logging/logging.h>
#include <memory/bus.h>
#include <video/ppu.h>
#include <gameboy.h>

enum Flags {
    FLAG_ZERO       = 7,
    FLAG_NEGATIVE   = 6,
    FLAG_HALF       = 5,
    FLAG_CARRY      = 4,
};

#define GET_FLAG(flag) ((gb->cpu.registers.f & (1 << flag)) >> flag)
#define SET_FLAG(flag) (gb->cpu.registers.f |= (1 << flag))
#define CLEAR_FLAG(flag) (gb->cpu.registers.f &= ~(1 << flag))
#define TOGGLE_FLAG(flag) (gb->cpu.registers.f ^= (1 << flag))

bool execute_opcode(struct Gameboy *gb) {

    u8 byte;
    u8 low_byte;
    u8 high_byte;

    enum Opcode opcode = (enum Opcode)gb->cpu.registers.ir;

    gameboy_log(LOG_CRITICAL,   "PC: 0x%04X "
                                    "AF: 0x%04X "
                                    "BC: 0x%04X "
                                    "DE: 0x%04X "
                                    "HL: 0x%04X "
                                    "SP: 0x%04X "
                                    "OPCODE: 0x%02X",
                                    gb->cpu.registers.pc - 1,
                                    gb->cpu.registers.af,
                                    gb->cpu.registers.bc,
                                    gb->cpu.registers.de,
                                    gb->cpu.registers.hl,
                                    gb->cpu.registers.sp,
                                    (u8)opcode);

    switch (opcode) {

//--------------------------------0x00--------------------------------//

        // 0x00
        case OPCODE_NOP:

            M_CYCLE_TICK;
            FETCH_CYCLE;

        // 0x03
        case OPCODE_INC_BC:

            INC_16_REG(gb->cpu.registers.bc);

        // 0x05
        case OPCODE_DEC_B:

            DEC_8_REG(gb->cpu.registers.b);

        // 0x06
        case OPCODE_LD_B_U8:

            LD_R_U8(gb->cpu.registers.b);

        // 0x0D
        case OPCODE_DEC_C:

            DEC_8_REG(gb->cpu.registers.c);

        // 0x0E
        case OPCODE_LD_C_U8:

            LD_R_U8(gb->cpu.registers.c);

//--------------------------------0x10--------------------------------//

        // 0x18
        case OPCODE_JR_I8:

            JP_COND_REL(true);

//--------------------------------0x20--------------------------------//

        // 0x20
        case OPCODE_JP_NZ_I8:

            JP_COND_REL(!GET_FLAG(FLAG_ZERO));

        // 0x21
        case OPCODE_LD_HL_U16:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, low_byte);

            // M2
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, high_byte);

            // M3
            M_CYCLE_TICK;
            gb->cpu.registers.hl = (((u16)high_byte << 8) | low_byte);
            FETCH_CYCLE;

        // 0x23
        case OPCODE_INC_HL:

            INC_16_REG(gb->cpu.registers.hl);

        // 0x2A
        case OPCODE_LD_A_IND_HLI:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl++, byte);

            // M2
            M_CYCLE_TICK;
            gb->cpu.registers.a = byte;
            FETCH_CYCLE;

//--------------------------------0x30--------------------------------//

        // 0x31
        case OPCODE_INC_DE:

            INC_16_REG(gb->cpu.registers.de);

        // 0x32
        case OPCODE_LD_IND_HLD_A:

            // M1
            M_CYCLE_TICK;
            WRITE_BYTE(gb->cpu.registers.hl--, gb->cpu.registers.a);

            //M2
            M_CYCLE_TICK;
            FETCH_CYCLE;

        // 0x39
        case OPCODE_ADD_HL_SP:

            ADD_HL_REG(gb->cpu.registers.sp);

        // 0x3C
        case OPCODE_INC_A:

            INC_8_REG(gb->cpu.registers.a);

        // 0x3E
        case OPCODE_LD_A_U8:

            LD_R_U8(gb->cpu.registers.a);

//--------------------------------0x40--------------------------------//

        // 0x40
        case OPCODE_LD_B_B:

            LD_R_R(gb->cpu.registers.b, gb->cpu.registers.b);

        // 0x41
        case OPCODE_LD_B_C:

            LD_R_R(gb->cpu.registers.b, gb->cpu.registers.c);

        // 0x42
        case OPCODE_LD_B_D:

            LD_R_R(gb->cpu.registers.b, gb->cpu.registers.d);

        // 0x43
        case OPCODE_LD_B_E:

            LD_R_R(gb->cpu.registers.b, gb->cpu.registers.e);

        // 0x44
        case OPCODE_LD_B_H:

            LD_R_R(gb->cpu.registers.b, gb->cpu.registers.h);

        // 0x45
        case OPCODE_LD_B_L:

            LD_R_R(gb->cpu.registers.b, gb->cpu.registers.l);

        // 0x46
        case OPCODE_LD_B_IND_HL:

            LD_R_IND_HL(gb->cpu.registers.b);

        // 0x47
        case OPCODE_LD_B_A:

            LD_R_R(gb->cpu.registers.b, gb->cpu.registers.a);

        // 0x48
        case OPCODE_LD_C_B:

            LD_R_R(gb->cpu.registers.c, gb->cpu.registers.b);

        // 0x49
        case OPCODE_LD_C_C:

            LD_R_R(gb->cpu.registers.c, gb->cpu.registers.c);

        // 0x4A
        case OPCODE_LD_C_D:

            LD_R_R(gb->cpu.registers.c, gb->cpu.registers.d);

        // 0x4B
        case OPCODE_LD_C_E:

            LD_R_R(gb->cpu.registers.c, gb->cpu.registers.e);

        // 0x4C
        case OPCODE_LD_C_H:

            LD_R_R(gb->cpu.registers.c, gb->cpu.registers.h);

        // 0x4D
        case OPCODE_LD_C_L:

            LD_R_R(gb->cpu.registers.c, gb->cpu.registers.l);

        // 0x4E
        case OPCODE_LD_C_IND_HL:

            LD_R_IND_HL(gb->cpu.registers.c);

        // 0x4F
        case OPCODE_LD_C_A:

            LD_R_R(gb->cpu.registers.c, gb->cpu.registers.a);

//--------------------------------0x50--------------------------------//

        // 0x50
        case OPCODE_LD_D_B:

            LD_R_R(gb->cpu.registers.d, gb->cpu.registers.b);

        // 0x51
        case OPCODE_LD_D_C:

            LD_R_R(gb->cpu.registers.d, gb->cpu.registers.c);

        // 0x52
        case OPCODE_LD_D_D:

            LD_R_R(gb->cpu.registers.d, gb->cpu.registers.d);

        // 0x53
        case OPCODE_LD_D_E:

            LD_R_R(gb->cpu.registers.d, gb->cpu.registers.e);

        // 0x54
        case OPCODE_LD_D_H:

            LD_R_R(gb->cpu.registers.d, gb->cpu.registers.h);

        // 0x55
        case OPCODE_LD_D_L:

            LD_R_R(gb->cpu.registers.d, gb->cpu.registers.l);

        // 0x56
        case OPCODE_LD_D_IND_HL:

            LD_R_IND_HL(gb->cpu.registers.d);

        // 0x57
        case OPCODE_LD_D_A:

            LD_R_R(gb->cpu.registers.d, gb->cpu.registers.a);

        // 0x58
        case OPCODE_LD_E_B:

            LD_R_R(gb->cpu.registers.e, gb->cpu.registers.b);

        // 0x59
        case OPCODE_LD_E_C:

            LD_R_R(gb->cpu.registers.e, gb->cpu.registers.c);

        // 0x5A
        case OPCODE_LD_E_D:

            LD_R_R(gb->cpu.registers.e, gb->cpu.registers.d);

        // 0x5B
        case OPCODE_LD_E_E:

            LD_R_R(gb->cpu.registers.e, gb->cpu.registers.e);

        // 0x5C
        case OPCODE_LD_E_H:

            LD_R_R(gb->cpu.registers.e, gb->cpu.registers.h);

        // 0x5D
        case OPCODE_LD_E_L:

            LD_R_R(gb->cpu.registers.e, gb->cpu.registers.l);

        // 0x5E
        case OPCODE_LD_E_IND_HL:

            LD_R_IND_HL(gb->cpu.registers.e);

        // 0x5F
        case OPCODE_LD_E_A:

            LD_R_R(gb->cpu.registers.e, gb->cpu.registers.a);

//--------------------------------0x60--------------------------------//

        // 0x60
        case OPCODE_LD_H_B:

            LD_R_R(gb->cpu.registers.h, gb->cpu.registers.b);

        // 0x61
        case OPCODE_LD_H_C:

            LD_R_R(gb->cpu.registers.h, gb->cpu.registers.c);

        // 0x62
        case OPCODE_LD_H_D:

            LD_R_R(gb->cpu.registers.h, gb->cpu.registers.d);

        // 0x63
        case OPCODE_LD_H_E:

            LD_R_R(gb->cpu.registers.h, gb->cpu.registers.e);

        // 0x64
        case OPCODE_LD_H_H:

            LD_R_R(gb->cpu.registers.h, gb->cpu.registers.h);

        // 0x65
        case OPCODE_LD_H_L:

            LD_R_R(gb->cpu.registers.h, gb->cpu.registers.l);

        // 0x66
        case OPCODE_LD_H_IND_HL:

            LD_R_IND_HL(gb->cpu.registers.h);

        // 0x67
        case OPCODE_LD_H_A:

            LD_R_R(gb->cpu.registers.h, gb->cpu.registers.a);

        // 0x68
        case OPCODE_LD_L_B:

            LD_R_R(gb->cpu.registers.l, gb->cpu.registers.b);

        // 0x69
        case OPCODE_LD_L_C:

            LD_R_R(gb->cpu.registers.l, gb->cpu.registers.c);

        // 0x6A
        case OPCODE_LD_L_D:

            LD_R_R(gb->cpu.registers.l, gb->cpu.registers.d);

        // 0x6B
        case OPCODE_LD_L_E:

            LD_R_R(gb->cpu.registers.l, gb->cpu.registers.e);

        // 0x6C
        case OPCODE_LD_L_H:

            LD_R_R(gb->cpu.registers.l, gb->cpu.registers.h);

        // 0x6D
        case OPCODE_LD_L_L:

            LD_R_R(gb->cpu.registers.l, gb->cpu.registers.l);

        // 0x6E
        case OPCODE_LD_L_IND_HL:

            LD_R_IND_HL(gb->cpu.registers.l);

        // 0x6F
        case OPCODE_LD_L_A:

            LD_R_R(gb->cpu.registers.l, gb->cpu.registers.a);

//--------------------------------0x70--------------------------------//

        // 0x70
        case OPCODE_LD_IND_HL_B:

            LD_IND_HL_R(gb->cpu.registers.b);

        // 0x71
        case OPCODE_LD_IND_HL_C:

            LD_IND_HL_R(gb->cpu.registers.c);

        // 0x72
        case OPCODE_LD_IND_HL_D:

            LD_IND_HL_R(gb->cpu.registers.d);

        // 0x73
        case OPCODE_LD_IND_HL_E:

            LD_IND_HL_R(gb->cpu.registers.e);

        // 0x74
        case OPCODE_LD_IND_HL_H:

            LD_IND_HL_R(gb->cpu.registers.h);

        // 0x75
        case OPCODE_LD_IND_HL_L:

            LD_IND_HL_R(gb->cpu.registers.l);

        // 0x76

        // 0x77
        case OPCODE_LD_IND_HL_A:

            LD_IND_HL_R(gb->cpu.registers.a);

        // 0x78
        case OPCODE_LD_A_B:

            LD_R_R(gb->cpu.registers.a, gb->cpu.registers.b);

        // 0x79
        case OPCODE_LD_A_C:

            LD_R_R(gb->cpu.registers.a, gb->cpu.registers.c);

        // 0x7A
        case OPCODE_LD_A_D:

            LD_R_R(gb->cpu.registers.a, gb->cpu.registers.d);

        // 0x7B
        case OPCODE_LD_A_E:

            LD_R_R(gb->cpu.registers.a, gb->cpu.registers.e);

        // 0x7C
        case OPCODE_LD_A_H:

            LD_R_R(gb->cpu.registers.a, gb->cpu.registers.h);

        // 0x7D
        case OPCODE_LD_A_L:

            LD_R_R(gb->cpu.registers.a, gb->cpu.registers.l);

        // 0x7E
        case OPCODE_LD_A_IND_HL:

            LD_R_IND_HL(gb->cpu.registers.a);

        // 0x7F
        case OPCODE_LD_A_A:

            LD_R_R(gb->cpu.registers.a, gb->cpu.registers.a);

//--------------------------------0x80--------------------------------//

        // 0x80
        case OPCODE_ADD_B:

            ADD(gb->cpu.registers.b);

        // 0x81
        case OPCODE_ADD_C:

            ADD(gb->cpu.registers.c);

        // 0x82
        case OPCODE_ADD_D:

            ADD(gb->cpu.registers.d);

        // 0x83
        case OPCODE_ADD_E:

            ADD(gb->cpu.registers.e);

        // 0x84
        case OPCODE_ADD_H:

            ADD(gb->cpu.registers.h);

        // 0x85
        case OPCODE_ADD_L:

            ADD(gb->cpu.registers.l);

        // 0x86
        case OPCODE_ADD_IND_HL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl, byte);

            // M2
            ADD(byte);

        // 0x87
        case OPCODE_ADD_A:

            ADD(gb->cpu.registers.a);

        // 0x88
        case OPCODE_ADC_B:

            ADC(gb->cpu.registers.a);

        // 0x89
        case OPCODE_ADC_C:

            ADC(gb->cpu.registers.c);

        // 0x8A
        case OPCODE_ADC_D:

            ADC(gb->cpu.registers.d);

        // 0x8B
        case OPCODE_ADC_E:

            ADC(gb->cpu.registers.e);

        // 0x8C
        case OPCODE_ADC_H:

            ADC(gb->cpu.registers.h);

        // 0x8D
        case OPCODE_ADC_L:

            ADC(gb->cpu.registers.l);

        // 0x8E
        case OPCODE_ADC_IND_HL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl, byte);

            // M2
            ADC(byte);

        // 0x8F
        case OPCODE_ADC_A:

            ADC(gb->cpu.registers.a);


//--------------------------------0x90--------------------------------//

        // 0x90
        case OPCODE_SUB_B:

            SUB(gb->cpu.registers.b);

        // 0x91
        case OPCODE_SUB_C:

            SUB(gb->cpu.registers.c);

        // 0x92
        case OPCODE_SUB_D:

            SUB(gb->cpu.registers.d);

        // 0x93
        case OPCODE_SUB_E:

            SUB(gb->cpu.registers.e);

        // 0x94
        case OPCODE_SUB_H:

            SUB(gb->cpu.registers.h);

        // 0x95
        case OPCODE_SUB_L:

            SUB(gb->cpu.registers.l);

        // 0x96
        case OPCODE_SUB_IND_HL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl, byte);

            // M2
            SUB(byte);

        // 0x97
        case OPCODE_SUB_A:

            SUB(gb->cpu.registers.a);

        // 0x98
        case OPCODE_SBC_B:

            SBC(gb->cpu.registers.b);

        // 0x99
        case OPCODE_SBC_C:

            SBC(gb->cpu.registers.c);

        // 0x9A
        case OPCODE_SBC_D:

            SBC(gb->cpu.registers.d);

        // 0x9B
        case OPCODE_SBC_E:

            SBC(gb->cpu.registers.e);

        // 0x9C
        case OPCODE_SBC_H:

            SBC(gb->cpu.registers.h);

        // 0x9D
        case OPCODE_SBC_L:

            SBC(gb->cpu.registers.l);

        // 0x9E
        case OPCODE_SBC_IND_HL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl, byte);

            // M2
            SBC(byte);

        // 0x9F
        case OPCODE_SBC_A:

            SBC(gb->cpu.registers.a);


//--------------------------------0xA0--------------------------------//

        // 0xA0
        case OPCODE_AND_B:

            AND(gb->cpu.registers.b);

        // 0xA1
        case OPCODE_AND_C:

            AND(gb->cpu.registers.c);

        // 0xA2
        case OPCODE_AND_D:

            AND(gb->cpu.registers.d);

        // 0xA3
        case OPCODE_AND_E:

            AND(gb->cpu.registers.e);

        // 0xA4
        case OPCODE_AND_H:

            AND(gb->cpu.registers.h);

        // 0xA5
        case OPCODE_AND_L:

            AND(gb->cpu.registers.l);

        // 0xA6
        case OPCODE_AND_IND_HL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl, byte);

            // M2
            AND(byte);

        // 0xA7
        case OPCODE_AND_A:

            AND(gb->cpu.registers.a);

        // 0xA8
        case OPCODE_XOR_B:

            XOR(gb->cpu.registers.b);

        // 0xA9
        case OPCODE_XOR_C:

            XOR(gb->cpu.registers.c);

        // 0xAA
        case OPCODE_XOR_D:

            XOR(gb->cpu.registers.d);

        // 0xAB
        case OPCODE_XOR_E:

            XOR(gb->cpu.registers.e);

        // 0xAC
        case OPCODE_XOR_H:

            XOR(gb->cpu.registers.h);

        // 0xAD
        case OPCODE_XOR_L:

            XOR(gb->cpu.registers.l);

        // 0xAE
        case OPCODE_XOR_IND_HL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl, byte);

            // M2
            XOR(byte);

        // 0xAF
        case OPCODE_XOR_A:

            XOR(gb->cpu.registers.a);

//--------------------------------0xB0--------------------------------//

        // 0xB0
        case OPCODE_OR_B:

            OR(gb->cpu.registers.b);

        // 0xB1
        case OPCODE_OR_C:

            OR(gb->cpu.registers.c);

        // 0xB2
        case OPCODE_OR_D:

            OR(gb->cpu.registers.d);

        // 0xB3
        case OPCODE_OR_E:

            OR(gb->cpu.registers.e);

        // 0xB4
        case OPCODE_OR_H:

            OR(gb->cpu.registers.h);

        // 0xB5
        case OPCODE_OR_L:

            OR(gb->cpu.registers.l);

        // 0xB6
        case OPCODE_OR_IND_HL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl, byte);

            // M2
            OR(byte);

        // 0xB7
        case OPCODE_OR_A:

            OR(gb->cpu.registers.a);

        // 0xB8
        case OPCODE_CP_B:

            CP(gb->cpu.registers.b);

        // 0xB9
        case OPCODE_CP_C:

            CP(gb->cpu.registers.c);

        // 0xBA
        case OPCODE_CP_D:

            CP(gb->cpu.registers.d);

        // 0xBB
        case OPCODE_CP_E:

            CP(gb->cpu.registers.e);

        // 0xBC
        case OPCODE_CP_H:

            CP(gb->cpu.registers.h);

        // 0xBD
        case OPCODE_CP_L:

            CP(gb->cpu.registers.l);

        // 0xBE
        case OPCODE_CP_IND_HL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.hl, byte);

            // M2
            CP(byte);

        // 0xBF
        case OPCODE_CP_A:

            CP(gb->cpu.registers.a);

//--------------------------------0xC0--------------------------------//

        // 0xC1
        case OPCODE_POP_BC:

            POP_REG(gb->cpu.registers.bc);

        // 0xC3
        case OPCODE_JP_U16:

            JP_COND_ABS(true);

        // 0xC5
        case OPCODE_PUSH_BC:

            PUSH_REG(gb->cpu.registers.bc);

        // 0xC9
        case OPCODE_RET:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.sp++, low_byte);

            // M2
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.sp++, high_byte);

            // M3
            M_CYCLE_TICK;
            gb->cpu.registers.pc = (((u16)(high_byte)) << 8) | low_byte;

            // M4
            M_CYCLE_TICK;
            FETCH_CYCLE;

        // 0xCD
        case OPCODE_CALL:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, low_byte);

            // M2
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, high_byte);

            // M3
            M_CYCLE_TICK;
            gb->cpu.registers.sp--;

            // M4
            M_CYCLE_TICK;
            WRITE_BYTE(gb->cpu.registers.sp--, (u8)((gb->cpu.registers.pc & 0xFF00) >> 8));

            // M5
            M_CYCLE_TICK;
            WRITE_BYTE(gb->cpu.registers.sp, (u8)((gb->cpu.registers.pc & 0x00FF) >> 0));
            gb->cpu.registers.pc = (((u16)(high_byte) << 8) | low_byte);

            // M6
            M_CYCLE_TICK;
            FETCH_CYCLE;

//--------------------------------0xD0--------------------------------//

        // 0xD5
        case OPCODE_PUSH_DE:

            PUSH_REG(gb->cpu.registers.de);

        // 0xDF
        case OPCODE_RST_0x18:

            RST(0x0018);

//--------------------------------0xE0--------------------------------//

        // 0xE0
        case OPCODE_LDH_A_U8:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, low_byte);

            // M2
            M_CYCLE_TICK;
            high_byte = UINT8_MAX;
            WRITE_BYTE(((u16)high_byte << 8) | low_byte, gb->cpu.registers.a);

            // M3
            M_CYCLE_TICK;
            FETCH_CYCLE;

        // 0xE1
        case OPCODE_POP_HL:

            POP_REG(gb->cpu.registers.hl);

        // 0xE5
        case OPCODE_PUSH_HL:

            PUSH_REG(gb->cpu.registers.hl);

        // 0xEA
        case OPCODE_LD_NN_A:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, low_byte);

            // M2
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, high_byte);

            // M3
            M_CYCLE_TICK;
            WRITE_BYTE((((u16)high_byte << 8) | low_byte), gb->cpu.registers.a);

            // M4
            M_CYCLE_TICK;
            FETCH_CYCLE;

//--------------------------------0xF0--------------------------------//

        // 0xF0
        case OPCODE_LDH_U8_A:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, low_byte);

            // M2
            M_CYCLE_TICK;
            high_byte = UINT8_MAX;
            set_address(&gb->bus, (((u16)high_byte << 8)) | low_byte);
            gb->cpu.registers.a = gb->bus.data;

            // M3
            M_CYCLE_TICK;
            FETCH_CYCLE;

        // 0xF1
        case OPCODE_POP_AF:

            POP_REG(gb->cpu.registers.af);

        // 0xF3
        case OPCODE_DI:

            // M1
            M_CYCLE_TICK;
            gb->cpu.ime = false;
            FETCH_CYCLE;

        // 0xF5
        case OPCODE_PUSH_AF:

            PUSH_REG(gb->cpu.registers.af);

        // 0xF9
        case OPCODE_LD_SP_HL:

            // M1
            M_CYCLE_TICK;
            gb->cpu.registers.sp = gb->cpu.registers.hl;

            // M2
            M_CYCLE_TICK;
            FETCH_CYCLE;

        // 0xFA
        case OPCODE_LD_A_NN:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, low_byte);

            // M2
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, high_byte);

            // M3
            M_CYCLE_TICK;
            set_address(&gb->bus, (((u16)high_byte) << 8) | low_byte);
            read_byte(&gb->bus);
            byte = gb->bus.data;

            // M4
            M_CYCLE_TICK;
            gb->cpu.registers.a = byte;
            FETCH_CYCLE;

        // 0xFE
        case OPCODE_CP_U8:

            // M1
            M_CYCLE_TICK;
            READ_BYTE(gb->cpu.registers.pc++, byte);

            // M2
            CP(byte);
        
        // 0xFF
        case OPCODE_RST_0x38:

            RST(0x0038);

        default:
            break;

    }

    gameboy_log(LOG_CRITICAL, "Unhandled opcode: 0x%02X", (u8)opcode);
    return false;
}

__attribute__((always_inline)) bool execute_cb_opcode(struct Gameboy *gb, enum CB_Opcode cb_opcode) {
    
    (void)gb;

    switch (cb_opcode) {
        
        default:
            gameboy_log(LOG_CRITICAL, "Unhandled CB opcode: 0x%02X", (u8)cb_opcode);
    }

    return false;
}
