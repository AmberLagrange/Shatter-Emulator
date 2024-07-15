#ifndef INSTRUCTIONS_HELPER_H
#define INSTRUCTIONS_HELPER_H

#include <cpu/flags.h>
#include <cpu/instructions.h>

//--------------------------------Bits--------------------------------//

#define GET_BIT(num, bit)       ((num >> bit) & 0x01)
#define SET_BIT(num, bit)       (num |= (1 << bit))
#define RST_BIT(num, bit)       (num &= ~(1 << bit))

//--------------------------------Flags--------------------------------//

#define SET_FLAG_FROM_COND(flag, cond) do {                         \
                                                                    \
    (cond) ? SET_FLAG(flag) : CLEAR_FLAG(flag);                     \
} while (0)

#define SET_ZERO_FROM_VAL(val) do {                                 \
                                                                    \
    SET_FLAG_FROM_COND(FLAG_ZERO, (val == 0));                      \
} while (0)

#define GET_CARRIES_POS(reg, val) do {                              \
                                                                    \
    result = reg + (val);                                           \
    is_carry = (result < reg);                                      \
    is_half_carry =                                                 \
        ((((reg & 0x0F) + (val & 0x0F)) & 0x10) == 0x10);           \
} while (0)

#define GET_CARRIES_NEG(reg, val) do {                              \
                                                                    \
    result = reg - (val);                                           \
    is_carry = (result > reg);                                      \
    is_half_carry = ((u16)(reg & 0x0F) - (u16)(val & 0x0F) < 0);    \
} while (0)

//--------------------------------Tick other gameboy components--------------------------------//

#define M_CYCLE_TICK do {                                           \
                                                                    \
    tick_apu(&gb->apu);                                             \
    tick_ppu(&gb->ppu);                                             \
} while(0)

//--------------------------------Fetch the next opcode--------------------------------//

#define FETCH_CYCLE do {                                            \
                                                                    \
    set_address(&gb->bus, gb->cpu.registers.pc++);                  \
    read_byte(&gb->bus);                                            \
    gb->cpu.registers.ir = gb->bus.data;                            \
    return true; /* Temp while setting up opcodes */                \
} while (0)

#define FETCH_CB_CYCLE do {                                         \
                                                                    \
    set_address(&gb->bus, gb->cpu.registers.pc++);                  \
    read_byte(&gb->bus);                                            \
    gb->cpu.registers.ir = gb->bus.data;                            \
    return execute_cb_opcode(gb); /* Same as above*/                \
} while (0)

//--------------------------------Read and write macros--------------------------------//

#define READ_BYTE(address, val) do {                                \
                                                                    \
    set_address(&gb->bus, address);                                 \
    read_byte(&gb->bus);                                            \
    val = gb->bus.data;                                             \
} while (0)

#define WRITE_BYTE(address, val) do {                               \
                                                                    \
    set_address(&gb->bus, address);                                 \
    write_byte(&gb->bus, val);                                      \
} while (0)

//--------------------------------8 bit load macros--------------------------------//

#define LD_IND_RR_A(reg) do {                                        \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    WRITE_BYTE(reg, gb->cpu.registers.a);                           \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while (0)

#define LD_A_IND_RR(reg) do {                                        \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(reg, gb->cpu.registers.a);                            \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while (0)

#define LD_R_U8(reg) do {                                           \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(gb->cpu.registers.pc++, byte);                        \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    reg = byte;                                                     \
    FETCH_CYCLE;                                                    \
} while (0)

#define LD_R_R(reg1, reg2) do {                                     \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    reg1 = reg2;                                                    \
    FETCH_CYCLE;                                                    \
} while (0)

#define LD_R_IND_HL(reg) do { (void)reg; break; } while (0)

#define LD_IND_HL_R(reg) do { (void)reg; break; } while (0)

//--------------------------------16 bit load macros--------------------------------//

#define LD_RR_U16(reg) do {                                         \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(gb->cpu.registers.pc++, low_byte);                    \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(gb->cpu.registers.pc++, high_byte);                   \
                                                                    \
    /* M3 */                                                        \
    M_CYCLE_TICK;                                                   \
    reg = (((u16)high_byte << 8) | low_byte);                       \
    FETCH_CYCLE;                                                    \
} while (0)

//--------------------------------8 bit arithmetic macros--------------------------------//

#define INC_R(reg) do {                                             \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    reg += 1;                                                       \
    SET_ZERO_FROM_VAL(reg);                                         \
    CLEAR_FLAG(FLAG_NEGATIVE);                                      \
    /* TODO: Half Carry */                                          \
    FETCH_CYCLE;                                                    \
} while (0)

#define DEC_R(reg) do {                                             \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    reg -= 1;                                                       \
    SET_ZERO_FROM_VAL(reg);                                         \
    SET_FLAG(FLAG_NEGATIVE);                                        \
    /* TODO: Half Carry */                                          \
    FETCH_CYCLE;                                                    \
} while (0)

#define ADD(val) do { (void)val; break; } while (0)

#define ADC(val) do { (void)val; break; } while (0)

#define SUB(val) do { (void)val; break; } while (0)

#define SBC(val) do {                                               \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    GET_CARRIES_NEG(gb->cpu.registers.a,                            \
                    val + GET_FLAG(FLAG_CARRY));                    \
    gb->cpu.registers.a = result;                                   \
    SET_ZERO_FROM_VAL(gb->cpu.registers.a);                         \
    SET_FLAG(FLAG_NEGATIVE);                                        \
    SET_FLAG_FROM_COND(FLAG_CARRY, is_half_carry);                  \
    SET_FLAG_FROM_COND(FLAG_HALF, is_carry);                        \
} while (0)

#define AND(val) do { (void)val; break; } while (0)

#define XOR(val) do {                                               \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    gb->cpu.registers.a ^= val;                                     \
    SET_ZERO_FROM_VAL(gb->cpu.registers.a);                         \
    CLEAR_FLAG(FLAG_NEGATIVE);                                      \
    CLEAR_FLAG(FLAG_HALF);                                          \
    CLEAR_FLAG(FLAG_CARRY);                                         \
    FETCH_CYCLE;                                                    \
} while (0)

#define OR(val) do { (void)val; break; } while (0)

#define CP(val) do {                                                \
                                                                    \
    /* Mx */                                                        \
    M_CYCLE_TICK;                                                   \
    GET_CARRIES_NEG(gb->cpu.registers.a, val);                      \
    SET_ZERO_FROM_VAL(result);                                      \
    SET_FLAG(FLAG_NEGATIVE);                                        \
    SET_FLAG_FROM_COND(FLAG_HALF, is_half_carry);                   \
    SET_FLAG_FROM_COND(FLAG_CARRY, is_carry);                       \
    FETCH_CYCLE;                                                    \
} while (0)

//--------------------------------16 bit arithmetic macros--------------------------------//

#define INC_RR(reg) do {                                            \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    reg += 1;                                                       \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while (0)

#define DEC_RR(reg) do {                                            \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    reg -= 1;                                                       \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while (0)

#define ADD_HL_RR(reg) do {                                         \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    GET_CARRIES_POS(gb->cpu.registers.l, (u8)(reg >> 0));           \
    gb->cpu.registers.l = result;                                   \
    CLEAR_FLAG(FLAG_NEGATIVE);                                      \
    SET_FLAG_FROM_COND(FLAG_HALF, is_half_carry);                   \
    SET_FLAG_FROM_COND(FLAG_CARRY, is_carry);                       \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    GET_CARRIES_POS(gb->cpu.registers.h,                            \
                    (u8)(reg >> 8) + GET_FLAG(FLAG_CARRY));         \
    gb->cpu.registers.h = result;                                   \
    CLEAR_FLAG(FLAG_NEGATIVE);                                      \
    SET_FLAG_FROM_COND(FLAG_HALF, is_half_carry);                   \
    SET_FLAG_FROM_COND(FLAG_CARRY, is_carry);                       \
    FETCH_CYCLE;                                                    \
} while (0)

//--------------------------------Control flow macros--------------------------------//

#define JP_COND_ABS(cond) do {                                      \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(gb->cpu.registers.pc++, low_byte);                    \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(gb->cpu.registers.pc++, high_byte);                   \
                                                                    \
    /* M3 */                                                        \
    if (cond) {                                                     \
        M_CYCLE_TICK;                                               \
        gb->cpu.registers.pc = (((u16)high_byte << 8) | low_byte);  \
    }                                                               \
                                                                    \
    /* M3/M4 */                                                     \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while (0);

#define JP_COND_REL(cond) do {                                      \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(gb->cpu.registers.pc++, byte);                        \
    byte = gb->bus.data;                                            \
                                                                    \
    /* M2 */                                                        \
    if (cond) {                                                     \
        gb->cpu.registers.pc += (i8)byte;                           \
    }                                                               \
                                                                    \
    /* M2/M3 */                                                     \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while(0)

#define RST(address) do {                                           \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    gb->cpu.registers.sp--;                                         \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    high_byte = ((gb->cpu.registers.pc & 0xFF00) >> 8);             \
    WRITE_BYTE(gb->cpu.registers.sp--, high_byte);                  \
                                                                    \
    /* M3 */                                                        \
    M_CYCLE_TICK;                                                   \
    low_byte = ((gb->cpu.registers.pc & 0x00FF) >> 0);              \
    WRITE_BYTE(gb->cpu.registers.sp, low_byte);                     \
    gb->cpu.registers.pc = address;                                 \
                                                                    \
    /* M4 */                                                        \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while (0)

//--------------------------------Stack macros--------------------------------//

#define PUSH_RR(reg) do {                                           \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    gb->cpu.registers.sp--;                                         \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    high_byte = (u8)((reg & 0xFF00) >> 8);                          \
    WRITE_BYTE(gb->cpu.registers.sp--, high_byte);                  \
                                                                    \
    /* M3 */                                                        \
    M_CYCLE_TICK;                                                   \
    low_byte = (u8)((reg & 0x00FF) >> 8);                           \
    WRITE_BYTE(gb->cpu.registers.sp, low_byte);                     \
                                                                    \
    /* M4 */                                                        \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while (0)

#define POP_RR(reg) do {                                            \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(gb->cpu.registers.sp++, low_byte);                    \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    READ_BYTE(gb->cpu.registers.sp++, high_byte);                   \
                                                                    \
    /* M3 */                                                        \
    M_CYCLE_TICK;                                                   \
    reg = ((u16)high_byte << 8) | low_byte;                         \
    FETCH_CYCLE;                                                    \
} while (0)

//--------------------------------Misc macros--------------------------------//

#define NOP() do {                                                  \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    FETCH_CYCLE;                                                    \
} while (0)

#define CB_PREFIX() do {                                            \
                                                                    \
    /* M1 */                                                        \
    M_CYCLE_TICK;                                                   \
    FETCH_CB_CYCLE;                                                 \
} while (0)

//--------------------------------CB Prefix macros--------------------------------//

#define RR(reg) do {                                                \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    is_carry = GET_FLAG(FLAG_CARRY);                                \
    SET_FLAG_FROM_COND(FLAG_CARRY, GET_BIT(reg, 0));                \
    reg = (is_carry << 7) | (reg >> 1);                             \
    SET_ZERO_FROM_VAL(reg);                                         \
    CLEAR_FLAG(FLAG_NEGATIVE);                                      \
    CLEAR_FLAG(FLAG_HALF);                                          \
} while (0)

#define SRL(reg) do {                                               \
                                                                    \
    /* M2 */                                                        \
    M_CYCLE_TICK;                                                   \
    is_carry = GET_BIT(reg, 0);                                     \
    reg >>= 1;                                                      \
    SET_ZERO_FROM_VAL(reg);                                         \
    CLEAR_FLAG(FLAG_NEGATIVE);                                      \
    CLEAR_FLAG(FLAG_HALF);                                          \
    SET_FLAG_FROM_COND(FLAG_CARRY, is_carry);                       \
    FETCH_CYCLE;                                                    \
} while (0)

#endif // INSTRUCTIONS_HELPER_H
