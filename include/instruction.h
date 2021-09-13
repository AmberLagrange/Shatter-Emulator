#pragma once

#include "core.h"

#include <functional>

class Gameboy;

struct Instruction
{
    #ifndef NDEBUG
        std::string mnemonic;
    #endif

    std::function<void (Gameboy*)> op;
    u8 length;
    u8 cyclesBranch;
    u8 cyclesNoBranch;

    static const Instruction instructions[0x100];
    static const Instruction instructionsCB[0x100];

//--------------------------------------Opcode Helpers--------------------------------------//

    static void opcodeInc(Gameboy* gb, u8& reg);
    static void opcodeDec(Gameboy* gb, u8& reg);
    
    static void opcodeLoadu8(Gameboy* gb, u8& reg);
    static void opcodeLoadA(Gameboy* gb, const u8& val);

    static void opcodeXOR(Gameboy* gb, const u8& val);

    static void opcodeJP(Gameboy* gb, bool condition);
    static void opcodeJPOffset(Gameboy* gb, bool condition);

//--------------------------------------Opcodes--------------------------------------//

    //0x00

    static void op00(Gameboy* gb); // NOP
    static void op01(Gameboy* gb); // LD BC,u16
    static void op02(Gameboy* gb); // LD (BC),A
    static void op03(Gameboy* gb); // INC BC
    static void op04(Gameboy* gb); // INC B
    static void op05(Gameboy* gb); // DEC B
    static void op06(Gameboy* gb); // LD B,u8
    static void op07(Gameboy* gb); // RLCA
    static void op08(Gameboy* gb); // LD (u16),SP
    static void op09(Gameboy* gb); // ADD HL,BC
    static void op0A(Gameboy* gb); // LD A,(BC)
    static void op0B(Gameboy* gb); // DEC BC
    static void op0C(Gameboy* gb); // INC C
    static void op0D(Gameboy* gb); // DEC C
    static void op0E(Gameboy* gb); // LD C,u8
    static void op0F(Gameboy* gb); // RRCA

    //0x10

    static void op10(Gameboy* gb); // STOP
    static void op11(Gameboy* gb); // LD DE,u16
    static void op12(Gameboy* gb); // LD (DE),A
    static void op13(Gameboy* gb); // INC DE
    static void op14(Gameboy* gb); // INC D
    static void op15(Gameboy* gb); // DEC D
    static void op16(Gameboy* gb); // LD D,u8
    static void op17(Gameboy* gb); // RLA
    static void op18(Gameboy* gb); // JR i8
    static void op19(Gameboy* gb); // ADD HL,DE
    static void op1A(Gameboy* gb); // LD A,(DE)
    static void op1B(Gameboy* gb); // DEC DE
    static void op1C(Gameboy* gb); // INC E
    static void op1D(Gameboy* gb); // DEC E
    static void op1E(Gameboy* gb); // LD E,u8
    static void op1F(Gameboy* gb); // RRA

    //0x20

    static void op20(Gameboy* gb); // JR NZ,i8
    static void op21(Gameboy* gb); // LD HL,u16
    static void op22(Gameboy* gb); // LD (HL+),A
    static void op23(Gameboy* gb); // INC HL
    static void op24(Gameboy* gb); // INC H
    static void op25(Gameboy* gb); // DEC H
    static void op26(Gameboy* gb); // LD H,u8
    static void op27(Gameboy* gb); // DAA
    static void op28(Gameboy* gb); // JR Z,i8
    static void op29(Gameboy* gb); // ADD HL,HL
    static void op2A(Gameboy* gb); // LD A,(HL+)
    static void op2B(Gameboy* gb); // DEC HL
    static void op2C(Gameboy* gb); // INC L
    static void op2D(Gameboy* gb); // DEC L
    static void op2E(Gameboy* gb); // LD L,u8
    static void op2F(Gameboy* gb); // CPL

    //0x30

    static void op30(Gameboy* gb); // JR NC,i8
    static void op31(Gameboy* gb); // LD SP,u16
    static void op32(Gameboy* gb); // LD (HL-),A
    static void op33(Gameboy* gb); // INC SP
    static void op34(Gameboy* gb); // INC (HL)
    static void op35(Gameboy* gb); // DEC (HL)
    static void op36(Gameboy* gb); // LD (HL),u8
    static void op37(Gameboy* gb); // SCF
    static void op38(Gameboy* gb); // JR C,i8
    static void op39(Gameboy* gb); // ADD HL,SP
    static void op3A(Gameboy* gb); // LD A,(HL-)
    static void op3B(Gameboy* gb); // DEC SP
    static void op3C(Gameboy* gb); // INC A
    static void op3D(Gameboy* gb); // DEC A
    static void op3E(Gameboy* gb); // LD A,u8
    static void op3F(Gameboy* gb); // CCF

    //0x40

    static void op40(Gameboy* gb); // LD B,B
    static void op41(Gameboy* gb); // LD B,C
    static void op42(Gameboy* gb); // LD B,D
    static void op43(Gameboy* gb); // LD B,E
    static void op44(Gameboy* gb); // LD B,H
    static void op45(Gameboy* gb); // LD B,L
    static void op46(Gameboy* gb); // LD B,(HL)
    static void op47(Gameboy* gb); // LD B,A
    static void op48(Gameboy* gb); // LD C,B
    static void op49(Gameboy* gb); // LD C,C
    static void op4A(Gameboy* gb); // LD C,D
    static void op4B(Gameboy* gb); // LD C,E
    static void op4C(Gameboy* gb); // LD C,H
    static void op4D(Gameboy* gb); // LD C,L
    static void op4E(Gameboy* gb); // LD C,(HL)
    static void op4F(Gameboy* gb); // LD C,A

    //0x50

    static void op50(Gameboy* gb); // LD D,B
    static void op51(Gameboy* gb); // LD D,C
    static void op52(Gameboy* gb); // LD D,D
    static void op53(Gameboy* gb); // LD D,E
    static void op54(Gameboy* gb); // LD D,H
    static void op55(Gameboy* gb); // LD D,L
    static void op56(Gameboy* gb); // LD D,(HL)
    static void op57(Gameboy* gb); // LD D,A
    static void op58(Gameboy* gb); // LD E,B
    static void op59(Gameboy* gb); // LD E,C
    static void op5A(Gameboy* gb); // LD E,D
    static void op5B(Gameboy* gb); // LD E,E
    static void op5C(Gameboy* gb); // LD E,H
    static void op5D(Gameboy* gb); // LD E,L
    static void op5E(Gameboy* gb); // LD E,(HL)
    static void op5F(Gameboy* gb); // LD E,A

    //0x60

    static void op60(Gameboy* gb); // LD H,B
    static void op61(Gameboy* gb); // LD H,C
    static void op62(Gameboy* gb); // LD H,D
    static void op63(Gameboy* gb); // LD H,E
    static void op64(Gameboy* gb); // LD H,H
    static void op65(Gameboy* gb); // LD H,L
    static void op66(Gameboy* gb); // LD H,(HL)
    static void op67(Gameboy* gb); // LD H,A
    static void op68(Gameboy* gb); // LD L,B
    static void op69(Gameboy* gb); // LD L,C
    static void op6A(Gameboy* gb); // LD L,D
    static void op6B(Gameboy* gb); // LD L,E
    static void op6C(Gameboy* gb); // LD L,H
    static void op6D(Gameboy* gb); // LD L,L
    static void op6E(Gameboy* gb); // LD L,(HL)
    static void op6F(Gameboy* gb); // LD L,A

    //0x70

    static void op70(Gameboy* gb); // LD (HL),B
    static void op71(Gameboy* gb); // LD (HL),C
    static void op72(Gameboy* gb); // LD (HL),D
    static void op73(Gameboy* gb); // LD (HL),E
    static void op74(Gameboy* gb); // LD (HL),H
    static void op75(Gameboy* gb); // LD (HL),L
    static void op76(Gameboy* gb); // HALT
    static void op77(Gameboy* gb); // LD (HL),A
    static void op78(Gameboy* gb); // LD A,B
    static void op79(Gameboy* gb); // LD A,C
    static void op7A(Gameboy* gb); // LD A,D
    static void op7B(Gameboy* gb); // LD A,E
    static void op7C(Gameboy* gb); // LD A,H
    static void op7D(Gameboy* gb); // LD A,L
    static void op7E(Gameboy* gb); // LD A,(HL)
    static void op7F(Gameboy* gb); // LD A,A

    //0x80

    static void op80(Gameboy* gb); // ADD A,B
    static void op81(Gameboy* gb); // ADD A,C
    static void op82(Gameboy* gb); // ADD A,D
    static void op83(Gameboy* gb); // ADD A,E
    static void op84(Gameboy* gb); // ADD A,H
    static void op85(Gameboy* gb); // ADD A,L
    static void op86(Gameboy* gb); // ADD A,(HL)
    static void op87(Gameboy* gb); // ADD A,A
    static void op88(Gameboy* gb); // ADC A,B
    static void op89(Gameboy* gb); // ADC A,C
    static void op8A(Gameboy* gb); // ADC A,D
    static void op8B(Gameboy* gb); // ADC A,E
    static void op8C(Gameboy* gb); // ADC A,H
    static void op8D(Gameboy* gb); // ADC A,L
    static void op8E(Gameboy* gb); // ADC A,(HL)
    static void op8F(Gameboy* gb); // ADC A,A

    //0x90

    static void op90(Gameboy* gb); // SUB A,B
    static void op91(Gameboy* gb); // SUB A,C
    static void op92(Gameboy* gb); // SUB A,D
    static void op93(Gameboy* gb); // SUB A,E
    static void op94(Gameboy* gb); // SUB A,H
    static void op95(Gameboy* gb); // SUB A,L
    static void op96(Gameboy* gb); // SUB A,(HL)
    static void op97(Gameboy* gb); // SUB A,A
    static void op98(Gameboy* gb); // SBC A,B
    static void op99(Gameboy* gb); // SBC A,C
    static void op9A(Gameboy* gb); // SBC A,D
    static void op9B(Gameboy* gb); // SBC A,E
    static void op9C(Gameboy* gb); // SBC A,H
    static void op9D(Gameboy* gb); // SBC A,L
    static void op9E(Gameboy* gb); // SBC A,(HL)
    static void op9F(Gameboy* gb); // SBC A,A

    //0xA0

    static void opA0(Gameboy* gb); // AND A,B
    static void opA1(Gameboy* gb); // AND A,C
    static void opA2(Gameboy* gb); // AND A,D
    static void opA3(Gameboy* gb); // AND A,E
    static void opA4(Gameboy* gb); // AND A,H
    static void opA5(Gameboy* gb); // AND A,L
    static void opA6(Gameboy* gb); // AND A,(HL)
    static void opA7(Gameboy* gb); // AND A,A
    static void opA8(Gameboy* gb); // XOR A,B
    static void opA9(Gameboy* gb); // XOR A,C
    static void opAA(Gameboy* gb); // XOR A,D
    static void opAB(Gameboy* gb); // XOR A,E
    static void opAC(Gameboy* gb); // XOR A,H
    static void opAD(Gameboy* gb); // XOR A,L
    static void opAE(Gameboy* gb); // XOR A,(HL)
    static void opAF(Gameboy* gb); // XOR A,A

    //0xB0

    static void opB0(Gameboy* gb); // OR A,B
    static void opB1(Gameboy* gb); // OR A,C
    static void opB2(Gameboy* gb); // OR A,D
    static void opB3(Gameboy* gb); // OR A,E
    static void opB4(Gameboy* gb); // OR A,H
    static void opB5(Gameboy* gb); // OR A,L
    static void opB6(Gameboy* gb); // OR A,(HL)
    static void opB7(Gameboy* gb); // OR A,A
    static void opB8(Gameboy* gb); // CP A,B
    static void opB9(Gameboy* gb); // CP A,C
    static void opBA(Gameboy* gb); // CP A,D
    static void opBB(Gameboy* gb); // CP A,E
    static void opBC(Gameboy* gb); // CP A,H
    static void opBD(Gameboy* gb); // CP A,L
    static void opBE(Gameboy* gb); // CP A,(HL)
    static void opBF(Gameboy* gb); // CP A,A

    //0xC0

    static void opC0(Gameboy* gb); // RET NZ
    static void opC1(Gameboy* gb); // POP BC
    static void opC2(Gameboy* gb); // JP NZ,u16
    static void opC3(Gameboy* gb); // JP u16
    static void opC4(Gameboy* gb); // CALL NZ,u16
    static void opC5(Gameboy* gb); // PUSH BC
    static void opC6(Gameboy* gb); // ADD A,u8
    static void opC7(Gameboy* gb); // RST 00h
    static void opC8(Gameboy* gb); // RET Z
    static void opC9(Gameboy* gb); // RET
    static void opCA(Gameboy* gb); // JP Z,u16
    static void opCB(Gameboy* gb); // PREFIX CB
    static void opCC(Gameboy* gb); // CALL Z,u16
    static void opCD(Gameboy* gb); // CALL u16
    static void opCE(Gameboy* gb); // ADC A,u8
    static void opCF(Gameboy* gb); // RST 08h

    //0xD0

    static void opD0(Gameboy* gb); // RET NC
    static void opD1(Gameboy* gb); // POP DE
    static void opD2(Gameboy* gb); // JP NC,u16
    static void opD3(Gameboy* gb); // UNUSED
    static void opD4(Gameboy* gb); // CALL NC,u16
    static void opD5(Gameboy* gb); // PUSH DE
    static void opD6(Gameboy* gb); // SUB A,u8
    static void opD7(Gameboy* gb); // RST 10h
    static void opD8(Gameboy* gb); // RET C
    static void opD9(Gameboy* gb); // RETI
    static void opDA(Gameboy* gb); // JP C,u16
    static void opDB(Gameboy* gb); // UNUSED
    static void opDC(Gameboy* gb); // CALL C,u16
    static void opDD(Gameboy* gb); // UNUSED
    static void opDE(Gameboy* gb); // SBC A,u8
    static void opDF(Gameboy* gb); // RST 18h

    //0xE0

    static void opE0(Gameboy* gb); // LD (FF00+u8),A
    static void opE1(Gameboy* gb); // POP HL
    static void opE2(Gameboy* gb); // LD (FF00+C),A
    static void opE3(Gameboy* gb); // UNUSED
    static void opE4(Gameboy* gb); // UNUSED
    static void opE5(Gameboy* gb); // PUSH HL
    static void opE6(Gameboy* gb); // AND A,u8
    static void opE7(Gameboy* gb); // RST 20h
    static void opE8(Gameboy* gb); // ADD SP,i8
    static void opE9(Gameboy* gb); // JP HL
    static void opEA(Gameboy* gb); // LD (u16),A
    static void opEB(Gameboy* gb); // UNUSED
    static void opEC(Gameboy* gb); // UNUSED
    static void opED(Gameboy* gb); // UNUSED
    static void opEE(Gameboy* gb); // XOR A,u8
    static void opEF(Gameboy* gb); // RST 28h

    //0xF0

    static void opF0(Gameboy* gb); // LD A,(FF00+u8)
    static void opF1(Gameboy* gb); // POP AF
    static void opF2(Gameboy* gb); // LD A,(FF00+C)
    static void opF3(Gameboy* gb); // DI
    static void opF4(Gameboy* gb); // UNUSED
    static void opF5(Gameboy* gb); // PUSH AF
    static void opF6(Gameboy* gb); // OR A,u8
    static void opF7(Gameboy* gb); // RST 30h
    static void opF8(Gameboy* gb); // LD HL,SP+i8
    static void opF9(Gameboy* gb); // LD SP,HL
    static void opFA(Gameboy* gb); // LD A,(u16)
    static void opFB(Gameboy* gb); // EI
    static void opFC(Gameboy* gb); // UNUSED
    static void opFD(Gameboy* gb); // UNUSED
    static void opFE(Gameboy* gb); // CP A,u8
    static void opFF(Gameboy* gb); // RST 38h
};