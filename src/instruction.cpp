#include "instruction.h"
#include "gameboy.h"

#ifndef NDEBUG
    #include <string>
    #define INSTRUCTION(mnemonic, op, length, cyclesBranch, cyclesNoBranch) { mnemonic, op, length, cyclesBranch, cyclesNoBranch }
#else
    #define INSTRUCTION(mnemonic, op, length, cyclesBranch, cyclesNoBranch) { op, length, cyclesBranch, cyclesNoBranch }
#endif

//--------------------------------------Log Helper Macros--------------------------------------//

#define LOG_A_REG() LOG("A Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.A) << ".")
#define LOG_F_REG() LOG("F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.F) << ".")
#define LOG_B_REG() LOG("B Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.B) << ".")
#define LOG_C_REG() LOG("C Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.C) << ".")
#define LOG_D_REG() LOG("D Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.D) << ".")
#define LOG_E_REG() LOG("E Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.E) << ".")
#define LOG_H_REG() LOG("H Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.H) << ".")
#define LOG_L_REG() LOG("F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.F) << ".")

#define LOG_FLAGS() LOG("Flags updated to: "                                              \
                        << ((gb->m_CPU.m_Registers.F & CPU::Flag::Zero)      ? "Z" : "_") \
                        << ((gb->m_CPU.m_Registers.F & CPU::Flag::Negative)  ? "N" : "_") \
                        << ((gb->m_CPU.m_Registers.F & CPU::Flag::HalfCarry) ? "H" : "_") \
                        << ((gb->m_CPU.m_Registers.F & CPU::Flag::Carry)     ? "C" : "_") \
                        << ".")

#define LOG_AF_REG() LOG("AF Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.AF) << ".")
#define LOG_BC_REG() LOG("BC Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.BC) << ".")
#define LOG_DE_REG() LOG("DE Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.DE) << ".")
#define LOG_HL_REG() LOG("HL Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.HL) << ".")
#define LOG_SP_REG() LOG("SP Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.SP) << ".")

#define LOG_WRITE(addr, val) LOG("Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(val) << " to address 0x" << std::setw(4) << addr << ".")

#define LOG_JP(addr) LOG("Jumped to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(addr) << ".");
#define LOG_NJP() LOG("Did not jump.");

#define LOG_RET(addr) LOG("Returned to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(addr) << ".");
#define LOG_NRET() LOG("Did not return.");

#define LOG_DI() LOG("Disabled Interrupts.")
#define LOG_EI() LOG("Enabled Interrupts.")

//--------------------------------------Opcode Helpers--------------------------------------//

void Instruction::opcodeINC(Gameboy* gb, u8& reg)
{

}

void Instruction::opcodeDEC(Gameboy* gb, u8& reg)
{
    reg--;

    gb->m_CPU.clearFlag(CPU::Flag::Zero | CPU::Flag::Negative | CPU::Flag::HalfCarry);

    gb->m_CPU.setZeroFromVal(reg);

    gb->m_CPU.setFlag(CPU::Flag::Negative);

    if((reg & 0x0f) == 0x0f) gb->m_CPU.setFlag(CPU::Flag::HalfCarry);   
}

void Instruction::opcodeADD(Gameboy* gb, const u8& val)
{

}

void Instruction::opcodeADC(Gameboy* gb, const u8& val)
{

}

void Instruction::opcodeSUB(Gameboy* gb, const u8& val)
{

}

void Instruction::opcodeSBC(Gameboy* gb, const u8& val)
{

}

void Instruction::opcodeAND(Gameboy* gb, const u8& val)
{

}

void Instruction::opcodeXOR(Gameboy* gb, const u8& val)
{
    gb->m_CPU.m_Registers.A ^= val;

    gb->m_CPU.clearAllFlags();

    gb->m_CPU.setZeroFromVal(gb->m_CPU.m_Registers.A);
}

void Instruction::opcodeOR(Gameboy* gb, const u8& val)
{

}

void Instruction::opcodeCP(Gameboy* gb, const u8& val)
{
    const u8& a = gb->m_CPU.m_Registers.A;

    gb->m_CPU.clearAllFlags();

    gb->m_CPU.setFlag(CPU::Flag::Negative);

    if(a < val)
        gb->m_CPU.setFlag(CPU::Flag::Carry);

    if(a == val)
        gb->m_CPU.setFlag(CPU::Flag::Zero);

    if((a & 0x0F) < ((a - val) & 0x0F))
        gb->m_CPU.setFlag(CPU::Flag::HalfCarry);

}

void Instruction::opcodeJP(Gameboy* gb, bool condition)
{
    u8 low  = gb->read(gb->m_CPU.m_Registers.PC++);
    u8 high = gb->read(gb->m_CPU.m_Registers.PC++);
    u16 addr = static_cast<u16>(high) << 8 | static_cast<u16>(low);

    if(condition)
    {
        gb->m_CPU.m_Registers.PC = addr;
        LOG_JP(addr);
    }
    else
    {
        LOG_NJP();
    }
}

void Instruction::opcodeJR(Gameboy* gb, bool condition)
{
    i8 offset = gb->read(gb->m_CPU.m_Registers.PC++);
    u16 addr = gb->m_CPU.m_Registers.PC + offset;

    if(condition)
    {
        gb->m_CPU.m_Registers.PC = addr;
        LOG_JP(addr);
    }
    else
    {
        LOG_NJP();
    }
}

void Instruction::opcodeCALL(Gameboy* gb, bool condition)
{

}

void Instruction::opcodeRET(Gameboy* gb, bool condiiton)
{

}

void Instruction::opcodePUSH(Gameboy* gb, const u8& reg)
{

}

void Instruction::opcodePOP(Gameboy* gb, u8& reg)
{
    
}

//--------------------------------------Opcodes--------------------------------------//

//0x00

void Instruction::op00([[maybe_unused]] Gameboy* gb) // NOP
{

}

void Instruction::op01(Gameboy* gb) // LD BC,u16
{

}

void Instruction::op02(Gameboy* gb) // LD (BC),A
{

}

void Instruction::op03(Gameboy* gb) // INC BC
{

}

void Instruction::op04(Gameboy* gb) // INC B
{

}

void Instruction::op05(Gameboy* gb) // DEC B
{
    opcodeDEC(gb, gb->m_CPU.m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void Instruction::op06(Gameboy* gb) // LD B,u8
{
    gb->m_CPU.m_Registers.B = gb->read(gb->m_CPU.m_Registers.PC++);

    LOG_B_REG();
}

void Instruction::op07(Gameboy* gb) // RLCA
{

}

void Instruction::op08(Gameboy* gb) // LD (u16),SP
{

}

void Instruction::op09(Gameboy* gb) // ADD HL,BC
{

}

void Instruction::op0A(Gameboy* gb) // LD A,(BC)
{

}

void Instruction::op0B(Gameboy* gb) // DEC BC
{

}

void Instruction::op0C(Gameboy* gb) // INC C
{

}

void Instruction::op0D(Gameboy* gb) // DEC C
{
    opcodeDEC(gb, gb->m_CPU.m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void Instruction::op0E(Gameboy* gb) // LD C,u8
{
    gb->m_CPU.m_Registers.C = gb->m_CPU.m_Registers.PC++;

    LOG_C_REG();
}

void Instruction::op0F(Gameboy* gb) // RRCA
{

}

//0x10

void Instruction::op10(Gameboy* gb) // STOP
{

}

void Instruction::op11(Gameboy* gb) // LD DE,u16
{

}

void Instruction::op12(Gameboy* gb) // LD (DE),A
{

}

void Instruction::op13(Gameboy* gb) // INC DE
{

}

void Instruction::op14(Gameboy* gb) // INC D
{

}

void Instruction::op15(Gameboy* gb) // DEC D
{

}

void Instruction::op16(Gameboy* gb) // LD D,u8
{

}

void Instruction::op17(Gameboy* gb) // RLA
{

}

void Instruction::op18(Gameboy* gb) // JR i8
{

}

void Instruction::op19(Gameboy* gb) // ADD HL,DE
{

}

void Instruction::op1A(Gameboy* gb) // LD A,(DE)
{

}

void Instruction::op1B(Gameboy* gb) // DEC DE
{

}

void Instruction::op1C(Gameboy* gb) // INC E
{

}

void Instruction::op1D(Gameboy* gb) // DEC E
{

}

void Instruction::op1E(Gameboy* gb) // LD E,u8
{

}

void Instruction::op1F(Gameboy* gb) // RRA
{

}

//0x20

void Instruction::op20(Gameboy* gb) // JR NZ,i8
{
    opcodeJR(gb, !gb->m_CPU.isFlagSet(CPU::Flag::Zero));
}

void Instruction::op21(Gameboy* gb) // LD HL,u16
{
    u8 low  = gb->read(gb->m_CPU.m_Registers.PC++);
    u8 high = gb->read(gb->m_CPU.m_Registers.PC++);
    gb->m_CPU.m_Registers.HL = static_cast<u16>(high) << 8 | static_cast<u16>(low);

    LOG_HL_REG();
}

void Instruction::op22(Gameboy* gb) // LD (HL+),A
{

}

void Instruction::op23(Gameboy* gb) // INC HL
{

}

void Instruction::op24(Gameboy* gb) // INC H
{

}

void Instruction::op25(Gameboy* gb) // DEC H
{

}

void Instruction::op26(Gameboy* gb) // LD H,u8
{

}

void Instruction::op27(Gameboy* gb) // DAA
{

}

void Instruction::op28(Gameboy* gb) // JR Z,i8
{

}

void Instruction::op29(Gameboy* gb) // ADD HL,HL
{

}

void Instruction::op2A(Gameboy* gb) // LD A,(HL+)
{

}

void Instruction::op2B(Gameboy* gb) // DEC HL
{

}

void Instruction::op2C(Gameboy* gb) // INC L
{

}

void Instruction::op2D(Gameboy* gb) // DEC L
{

}

void Instruction::op2E(Gameboy* gb) // LD L,u8
{

}

void Instruction::op2F(Gameboy* gb) // CPL
{

}

//0x30

void Instruction::op30(Gameboy* gb) // JR NC,i8
{

}

void Instruction::op31(Gameboy* gb) // LD SP,u16
{

}

void Instruction::op32(Gameboy* gb) // LD (HL-),A
{
    gb->write(gb->m_CPU.m_Registers.HL--, gb->m_CPU.m_Registers.A);

    LOG_WRITE(gb->m_CPU.m_Registers.HL + 1, gb->m_CPU.m_Registers.A);
}

void Instruction::op33(Gameboy* gb) // INC SP
{

}

void Instruction::op34(Gameboy* gb) // INC (HL)
{

}

void Instruction::op35(Gameboy* gb) // DEC (HL)
{

}

void Instruction::op36(Gameboy* gb) // LD (HL),u8
{

}

void Instruction::op37(Gameboy* gb) // SCF
{

}

void Instruction::op38(Gameboy* gb) // JR C,i8
{

}

void Instruction::op39(Gameboy* gb) // ADD HL,SP
{

}

void Instruction::op3A(Gameboy* gb) // LD A,(HL-)
{

}

void Instruction::op3B(Gameboy* gb) // DEC SP
{

}

void Instruction::op3C(Gameboy* gb) // INC A
{

}

void Instruction::op3D(Gameboy* gb) // DEC A
{

}

void Instruction::op3E(Gameboy* gb) // LD A,u8
{
    gb->m_CPU.m_Registers.A = gb->m_CPU.m_Registers.PC++;

    LOG_A_REG();
}

void Instruction::op3F(Gameboy* gb) // CCF
{

}

//0x40

void Instruction::op40(Gameboy* gb) // LD B,B
{

}

void Instruction::op41(Gameboy* gb) // LD B,C
{

}

void Instruction::op42(Gameboy* gb) // LD B,D
{

}

void Instruction::op43(Gameboy* gb) // LD B,E
{

}

void Instruction::op44(Gameboy* gb) // LD B,H
{

}

void Instruction::op45(Gameboy* gb) // LD B,L
{

}

void Instruction::op46(Gameboy* gb) // LD B,(HL)
{

}

void Instruction::op47(Gameboy* gb) // LD B,A
{

}

void Instruction::op48(Gameboy* gb) // LD C,B
{

}

void Instruction::op49(Gameboy* gb) // LD C,C
{

}

void Instruction::op4A(Gameboy* gb) // LD C,D
{

}

void Instruction::op4B(Gameboy* gb) // LD C,E
{

}

void Instruction::op4C(Gameboy* gb) // LD C,H
{

}

void Instruction::op4D(Gameboy* gb) // LD C,L
{

}

void Instruction::op4E(Gameboy* gb) // LD C,(HL)
{

}

void Instruction::op4F(Gameboy* gb) // LD C,A
{

}

//0x50

void Instruction::op50(Gameboy* gb) // LD D,B
{

}

void Instruction::op51(Gameboy* gb) // LD D,C
{

}

void Instruction::op52(Gameboy* gb) // LD D,D
{

}

void Instruction::op53(Gameboy* gb) // LD D,E
{

}

void Instruction::op54(Gameboy* gb) // LD D,H
{

}

void Instruction::op55(Gameboy* gb) // LD D,L
{

}

void Instruction::op56(Gameboy* gb) // LD D,(HL)
{

}

void Instruction::op57(Gameboy* gb) // LD D,A
{

}

void Instruction::op58(Gameboy* gb) // LD E,B
{

}

void Instruction::op59(Gameboy* gb) // LD E,C
{

}

void Instruction::op5A(Gameboy* gb) // LD E,D
{

}

void Instruction::op5B(Gameboy* gb) // LD E,E
{

}

void Instruction::op5C(Gameboy* gb) // LD E,H
{

}

void Instruction::op5D(Gameboy* gb) // LD E,L
{

}

void Instruction::op5E(Gameboy* gb) // LD E,(HL)
{

}

void Instruction::op5F(Gameboy* gb) // LD E,A
{

}

//0x60

void Instruction::op60(Gameboy* gb) // LD H,B
{

}

void Instruction::op61(Gameboy* gb) // LD H,C
{

}

void Instruction::op62(Gameboy* gb) // LD H,D
{

}

void Instruction::op63(Gameboy* gb) // LD H,E
{

}

void Instruction::op64(Gameboy* gb) // LD H,H
{

}

void Instruction::op65(Gameboy* gb) // LD H,L
{

}

void Instruction::op66(Gameboy* gb) // LD H,(HL)
{

}

void Instruction::op67(Gameboy* gb) // LD H,A
{

}

void Instruction::op68(Gameboy* gb) // LD L,B
{

}

void Instruction::op69(Gameboy* gb) // LD L,C
{

}

void Instruction::op6A(Gameboy* gb) // LD L,D
{

}

void Instruction::op6B(Gameboy* gb) // LD L,E
{

}

void Instruction::op6C(Gameboy* gb) // LD L,H
{

}

void Instruction::op6D(Gameboy* gb) // LD L,L
{

}

void Instruction::op6E(Gameboy* gb) // LD L,(HL)
{

}

void Instruction::op6F(Gameboy* gb) // LD L,A
{

}

//0x70

void Instruction::op70(Gameboy* gb) // LD (HL),B
{

}

void Instruction::op71(Gameboy* gb) // LD (HL),C
{

}

void Instruction::op72(Gameboy* gb) // LD (HL),D
{

}

void Instruction::op73(Gameboy* gb) // LD (HL),E
{

}

void Instruction::op74(Gameboy* gb) // LD (HL),H
{

}

void Instruction::op75(Gameboy* gb) // LD (HL),L
{

}

void Instruction::op76(Gameboy* gb) // HALT
{

}

void Instruction::op77(Gameboy* gb) // LD (HL),A
{

}

void Instruction::op78(Gameboy* gb) // LD A,B
{

}

void Instruction::op79(Gameboy* gb) // LD A,C
{

}

void Instruction::op7A(Gameboy* gb) // LD A,D
{

}

void Instruction::op7B(Gameboy* gb) // LD A,E
{

}

void Instruction::op7C(Gameboy* gb) // LD A,H
{

}

void Instruction::op7D(Gameboy* gb) // LD A,L
{

}

void Instruction::op7E(Gameboy* gb) // LD A,(HL)
{

}

void Instruction::op7F(Gameboy* gb) // LD A,A
{

}

//0x80

void Instruction::op80(Gameboy* gb) // ADD A,B
{

}

void Instruction::op81(Gameboy* gb) // ADD A,C
{

}

void Instruction::op82(Gameboy* gb) // ADD A,D
{

}

void Instruction::op83(Gameboy* gb) // ADD A,E
{

}

void Instruction::op84(Gameboy* gb) // ADD A,H
{

}

void Instruction::op85(Gameboy* gb) // ADD A,L
{

}

void Instruction::op86(Gameboy* gb) // ADD A,(HL)
{

}

void Instruction::op87(Gameboy* gb) // ADD A,A
{

}

void Instruction::op88(Gameboy* gb) // ADC A,B
{

}

void Instruction::op89(Gameboy* gb) // ADC A,C
{

}

void Instruction::op8A(Gameboy* gb) // ADC A,D
{

}

void Instruction::op8B(Gameboy* gb) // ADC A,E
{

}

void Instruction::op8C(Gameboy* gb) // ADC A,H
{

}

void Instruction::op8D(Gameboy* gb) // ADC A,L
{

}

void Instruction::op8E(Gameboy* gb) // ADC A,(HL)
{

}

void Instruction::op8F(Gameboy* gb) // ADC A,A
{

}

//0x90

void Instruction::op90(Gameboy* gb) // SUB A,B
{

}

void Instruction::op91(Gameboy* gb) // SUB A,C
{

}

void Instruction::op92(Gameboy* gb) // SUB A,D
{

}

void Instruction::op93(Gameboy* gb) // SUB A,E
{

}

void Instruction::op94(Gameboy* gb) // SUB A,H
{

}

void Instruction::op95(Gameboy* gb) // SUB A,L
{

}

void Instruction::op96(Gameboy* gb) // SUB A,(HL)
{

}

void Instruction::op97(Gameboy* gb) // SUB A,A
{

}

void Instruction::op98(Gameboy* gb) // SBC A,B
{

}

void Instruction::op99(Gameboy* gb) // SBC A,C
{

}

void Instruction::op9A(Gameboy* gb) // SBC A,D
{

}

void Instruction::op9B(Gameboy* gb) // SBC A,E
{

}

void Instruction::op9C(Gameboy* gb) // SBC A,H
{

}

void Instruction::op9D(Gameboy* gb) // SBC A,L
{

}

void Instruction::op9E(Gameboy* gb) // SBC A,(HL)
{

}

void Instruction::op9F(Gameboy* gb) // SBC A,A
{

}

//0xA0

void Instruction::opA0(Gameboy* gb) // AND A,B
{

}

void Instruction::opA1(Gameboy* gb) // AND A,C
{

}

void Instruction::opA2(Gameboy* gb) // AND A,D
{

}

void Instruction::opA3(Gameboy* gb) // AND A,E
{

}

void Instruction::opA4(Gameboy* gb) // AND A,H
{

}

void Instruction::opA5(Gameboy* gb) // AND A,L
{

}

void Instruction::opA6(Gameboy* gb) // AND A,(HL)
{

}

void Instruction::opA7(Gameboy* gb) // AND A,A
{

}

void Instruction::opA8(Gameboy* gb) // XOR A,B
{

}

void Instruction::opA9(Gameboy* gb) // XOR A,C
{

}

void Instruction::opAA(Gameboy* gb) // XOR A,D
{

}

void Instruction::opAB(Gameboy* gb) // XOR A,E
{

}

void Instruction::opAC(Gameboy* gb) // XOR A,H
{

}

void Instruction::opAD(Gameboy* gb) // XOR A,L
{

}

void Instruction::opAE(Gameboy* gb) // XOR A,(HL)
{

}


void Instruction::opAF(Gameboy* gb) // XOR A,A
{
    opcodeXOR(gb, gb->m_CPU.m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

//0xB0

void Instruction::opB0(Gameboy* gb) // OR A,B
{



}
void Instruction::opB1(Gameboy* gb) // OR A,C
{

}

void Instruction::opB2(Gameboy* gb) // OR A,D
{

}

void Instruction::opB3(Gameboy* gb) // OR A,E
{

}

void Instruction::opB4(Gameboy* gb) // OR A,H
{

}

void Instruction::opB5(Gameboy* gb) // OR A,L
{

}

void Instruction::opB6(Gameboy* gb) // OR A,(HL)
{

}

void Instruction::opB7(Gameboy* gb) // OR A,A
{

}

void Instruction::opB8(Gameboy* gb) // CP A,B
{

}

void Instruction::opB9(Gameboy* gb) // CP A,C
{

}

void Instruction::opBA(Gameboy* gb) // CP A,D
{

}

void Instruction::opBB(Gameboy* gb) // CP A,E
{

}

void Instruction::opBC(Gameboy* gb) // CP A,H
{

}

void Instruction::opBD(Gameboy* gb) // CP A,L
{

}

void Instruction::opBE(Gameboy* gb) // CP A,(HL)
{

}

void Instruction::opBF(Gameboy* gb) // CP A,A
{

}

//0xC0

void Instruction::opC0(Gameboy* gb) // RET NZ
{

}

void Instruction::opC1(Gameboy* gb) // POP BC
{

}

void Instruction::opC2(Gameboy* gb) // JP NZ,u16
{

}

void Instruction::opC3(Gameboy* gb) // JP u16
{
    opcodeJP(gb, true);
}

void Instruction::opC4(Gameboy* gb) // CALL NZ,u16
{

}

void Instruction::opC5(Gameboy* gb) // PUSH BC
{

}

void Instruction::opC6(Gameboy* gb) // ADD A,u8
{

}

void Instruction::opC7(Gameboy* gb) // RST 00h
{

}

void Instruction::opC8(Gameboy* gb) // RET Z
{

}

void Instruction::opC9(Gameboy* gb) // RET
{

}

void Instruction::opCA(Gameboy* gb) // JP Z,u16
{

}

void Instruction::opCB(Gameboy* gb) // PREFIX CB
{

}

void Instruction::opCC(Gameboy* gb) // CALL Z,u16
{

}

void Instruction::opCD(Gameboy* gb) // CALL u16
{

}

void Instruction::opCE(Gameboy* gb) // ADC A,u8
{

}

void Instruction::opCF(Gameboy* gb) // RST 08h
{

}

//0xD0

void Instruction::opD0(Gameboy* gb) // RET NC
{

}

void Instruction::opD1(Gameboy* gb) // POP DE
{

}

void Instruction::opD2(Gameboy* gb) // JP NC,u16
{

}

void Instruction::opD3(Gameboy* gb) // UNUSED
{

}

void Instruction::opD4(Gameboy* gb) // CALL NC,u16
{

}

void Instruction::opD5(Gameboy* gb) // PUSH DE
{

}

void Instruction::opD6(Gameboy* gb) // SUB A,u8
{

}

void Instruction::opD7(Gameboy* gb) // RST 10h
{

}

void Instruction::opD8(Gameboy* gb) // RET C
{

}

void Instruction::opD9(Gameboy* gb) // RETI
{

}

void Instruction::opDA(Gameboy* gb) // JP C,u16
{

}

void Instruction::opDB(Gameboy* gb) // UNUSED
{

}

void Instruction::opDC(Gameboy* gb) // CALL C,u16
{

}

void Instruction::opDD(Gameboy* gb) // UNUSED
{

}

void Instruction::opDE(Gameboy* gb) // SBC A,u8
{

}

void Instruction::opDF(Gameboy* gb) // RST 18h
{

}

//0xE0

void Instruction::opE0(Gameboy* gb) // LD (FF00+u8),A
{
    u8 offset = gb->m_CPU.m_Registers.PC++;
    u16 addr = 0xFF00 | offset;
    gb->write(addr, gb->m_CPU.m_Registers.A);

    LOG_WRITE(addr, gb->m_CPU.m_Registers.A);
}

void Instruction::opE1(Gameboy* gb) // POP HL
{

}

void Instruction::opE2(Gameboy* gb) // LD (FF00+C),A
{

}

void Instruction::opE3(Gameboy* gb) // UNUSED
{

}

void Instruction::opE4(Gameboy* gb) // UNUSED
{

}

void Instruction::opE5(Gameboy* gb) // PUSH HL
{

}

void Instruction::opE6(Gameboy* gb) // AND A,u8
{

}

void Instruction::opE7(Gameboy* gb) // RST 20h
{

}

void Instruction::opE8(Gameboy* gb) // ADD SP,i8
{

}

void Instruction::opE9(Gameboy* gb) // JP HL
{

}

void Instruction::opEA(Gameboy* gb) // LD (u16),A
{

}

void Instruction::opEB(Gameboy* gb) // UNUSED
{

}

void Instruction::opEC(Gameboy* gb) // UNUSED
{

}

void Instruction::opED(Gameboy* gb) // UNUSED
{

}

void Instruction::opEE(Gameboy* gb) // XOR A,u8
{

}

void Instruction::opEF(Gameboy* gb) // RST 28h
{

}

//0xF0

void Instruction::opF0(Gameboy* gb) // LD A,(FF00+u8)
{
    u8 offset = gb->m_CPU.m_Registers.PC++;
    u16 addr = 0xFF00 | offset;
    gb->m_CPU.m_Registers.A = gb->read(addr);

    LOG_A_REG();
}

void Instruction::opF1(Gameboy* gb) // POP AF
{

}

void Instruction::opF2(Gameboy* gb) // LD A,(FF00+C)
{

}

void Instruction::opF3(Gameboy* gb) // DI
{
    gb->m_CPU.m_IME = false;

    LOG_DI();
}

void Instruction::opF4(Gameboy* gb) // UNUSED
{

}

void Instruction::opF5(Gameboy* gb) // PUSH AF
{

}

void Instruction::opF6(Gameboy* gb) // OR A,u8
{

}

void Instruction::opF7(Gameboy* gb) // RST 30h
{

}

void Instruction::opF8(Gameboy* gb) // LD HL,SP+i8
{

}

void Instruction::opF9(Gameboy* gb) // LD SP,HL
{

}

void Instruction::opFA(Gameboy* gb) // LD A,(u16)
{

}

void Instruction::opFB(Gameboy* gb) // EI
{
    gb->m_CPU.m_IME = true;

    LOG_EI();
}

void Instruction::opFC(Gameboy* gb) // UNUSED
{

}

void Instruction::opFD(Gameboy* gb) // UNUSED
{

}

void Instruction::opFE(Gameboy* gb) // CP A,u8
{
    opcodeCP(gb, gb->m_CPU.m_Registers.PC++);

    LOG_A_REG();
    LOG_FLAGS();
}

void Instruction::opFF(Gameboy* gb) // RST 38h
{

}

//--------------------------------------Opcode Table--------------------------------------//

const Instruction Instruction::instructions[0x100] = 
{
    //0x00
    INSTRUCTION("NOP",              Instruction::op00, 1,  4,  4),
    INSTRUCTION("LD BC,u16",        NULL, 3, 12, 12),
    INSTRUCTION("LD (BC),A",        NULL, 1,  8,  8),
    INSTRUCTION("INC BC",           NULL, 1,  8,  8),
    INSTRUCTION("INC B",            NULL, 1,  4,  4),
    INSTRUCTION("DEC B",            Instruction::op05, 1,  4,  4),
    INSTRUCTION("LD B,u8",          Instruction::op06, 2,  8,  8),
    INSTRUCTION("RLCA",             NULL, 1,  4,  4),
    INSTRUCTION("LD (u16),SP",      NULL, 3, 20, 20),
    INSTRUCTION("ADD HL,BC",        NULL, 1,  8,  8),
    INSTRUCTION("LD A,(BC)",        NULL, 1,  8,  8),
    INSTRUCTION("DEC BC",           NULL, 1,  8,  8),
    INSTRUCTION("INC C",            NULL, 1,  4,  4),
    INSTRUCTION("DEC C",            Instruction::op0D, 1,  4,  4),
    INSTRUCTION("LD C,u8",          Instruction::op0E, 2,  8,  8),
    INSTRUCTION("RRCA",             NULL, 1,  4,  4),

    //0x10
    INSTRUCTION("STOP",             NULL, 2,  4,  4),
    INSTRUCTION("LD DE,u16",        NULL, 3, 12, 12),
    INSTRUCTION("LD (DE),A",        NULL, 1,  8,  8),
    INSTRUCTION("INC DE",           NULL, 1,  8,  8),
    INSTRUCTION("INC D",            NULL, 1,  4,  4),
    INSTRUCTION("DEC D",            NULL, 1,  4,  4),
    INSTRUCTION("LD D,u8",          NULL, 2,  8,  8),
    INSTRUCTION("RLA",              NULL, 1,  4,  4),
    INSTRUCTION("JR i8",            NULL, 2, 12, 12),
    INSTRUCTION("ADD HL,DE",        NULL, 1,  8,  8),
    INSTRUCTION("LD A,(DE)",        NULL, 1,  8,  8),
    INSTRUCTION("DEC DE",           NULL, 1,  8,  8),
    INSTRUCTION("INC E",            NULL, 1,  4,  4),
    INSTRUCTION("DEC E",            NULL, 1,  4,  4),
    INSTRUCTION("LD E,u8",          NULL, 2,  8,  8),
    INSTRUCTION("RRA",              NULL, 1,  4,  4),

    //0x20
    INSTRUCTION("JR NZ,i8",         Instruction::op20, 2, 12,  8),
    INSTRUCTION("LD HL,u16",        Instruction::op21, 3, 12, 12),
    INSTRUCTION("LD (HL+),A",       NULL, 1,  8,  8),
    INSTRUCTION("INC HL",           NULL, 1,  8,  8),
    INSTRUCTION("INC H",            NULL, 1,  4,  4),
    INSTRUCTION("DEC H",            NULL, 1,  4,  4),
    INSTRUCTION("LD H,u8",          NULL, 2,  8,  8),
    INSTRUCTION("DAA",              NULL, 1,  4,  4),
    INSTRUCTION("JR Z,i8",          NULL, 2, 12,  8),
    INSTRUCTION("ADD HL,HL",        NULL, 1,  8,  8),
    INSTRUCTION("LD A,(HL+)",       NULL, 1,  8,  8),
    INSTRUCTION("DEC HL",           NULL, 1,  8,  8),
    INSTRUCTION("INC L",            NULL, 1,  4,  4),
    INSTRUCTION("DEC L",            NULL, 1,  4,  4),
    INSTRUCTION("LD L,u8",          NULL, 2,  8,  8),
    INSTRUCTION("CPL",              NULL, 1,  4,  4),

    //0x30
    INSTRUCTION("JR NC,i8",         NULL, 2, 12,  8),
    INSTRUCTION("LD SP,u16",        NULL, 3, 12, 12),
    INSTRUCTION("LD (HL-),A",       Instruction::op32, 1,  8,  8),
    INSTRUCTION("INC SP",           NULL, 1,  8,  8),
    INSTRUCTION("INC (HL)",         NULL, 1, 12, 12),
    INSTRUCTION("DEC (HL)",         NULL, 1, 12, 12),
    INSTRUCTION("LD (HL),u8",       NULL, 2, 12, 12),
    INSTRUCTION("SCF",              NULL, 1,  4,  4),
    INSTRUCTION("JR C,i8",          NULL, 2, 12,  8),
    INSTRUCTION("ADD HL,SP",        NULL, 1,  8,  8),
    INSTRUCTION("LD A,(HL-)",       NULL, 1,  8,  8),
    INSTRUCTION("DEC SP",           NULL, 1,  8,  8),
    INSTRUCTION("INC A",            NULL, 1,  4,  4),
    INSTRUCTION("DEC A",            NULL, 1,  4,  4),
    INSTRUCTION("LD A,u8",          Instruction::op3E, 2,  8,  8),
    INSTRUCTION("CCF",              NULL, 1,  4,  4),

    //0x40
    INSTRUCTION("LD B,B",           NULL, 1,  4,  4),
    INSTRUCTION("LD B,C",           NULL, 1,  4,  4),
    INSTRUCTION("LD B,D",           NULL, 1,  4,  4),
    INSTRUCTION("LD B,E",           NULL, 1,  4,  4),
    INSTRUCTION("LD B,H",           NULL, 1,  4,  4),
    INSTRUCTION("LD B,L",           NULL, 1,  4,  4),
    INSTRUCTION("LD B,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("LD B,A",           NULL, 1,  4,  4),
    INSTRUCTION("LD C,B",           NULL, 1,  4,  4),
    INSTRUCTION("LD C,C",           NULL, 1,  4,  4),
    INSTRUCTION("LD C,D",           NULL, 1,  4,  4),
    INSTRUCTION("LD C,E",           NULL, 1,  4,  4),
    INSTRUCTION("LD C,H",           NULL, 1,  4,  4),
    INSTRUCTION("LD C,L",           NULL, 1,  4,  4),
    INSTRUCTION("LD C,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("LD C,A",           NULL, 1,  4,  4),

    //0x50
    INSTRUCTION("LD D,B",           NULL, 1,  4,  4),
    INSTRUCTION("LD D,C",           NULL, 1,  4,  4),
    INSTRUCTION("LD D,D",           NULL, 1,  4,  4),
    INSTRUCTION("LD D,E",           NULL, 1,  4,  4),
    INSTRUCTION("LD D,H",           NULL, 1,  4,  4),
    INSTRUCTION("LD D,L",           NULL, 1,  4,  4),
    INSTRUCTION("LD D,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("LD D,A",           NULL, 1,  4,  4),
    INSTRUCTION("LD E,B",           NULL, 1,  4,  4),
    INSTRUCTION("LD E,C",           NULL, 1,  4,  4),
    INSTRUCTION("LD E,D",           NULL, 1,  4,  4),
    INSTRUCTION("LD E,E",           NULL, 1,  4,  4),
    INSTRUCTION("LD E,H",           NULL, 1,  4,  4),
    INSTRUCTION("LD E,L",           NULL, 1,  4,  4),
    INSTRUCTION("LD E,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("LD E,A",           NULL, 1,  4,  4),

    //0x60
    INSTRUCTION("LD H,B",           NULL, 1,  4,  4),
    INSTRUCTION("LD H,C",           NULL, 1,  4,  4),
    INSTRUCTION("LD H,D",           NULL, 1,  4,  4),
    INSTRUCTION("LD H,E",           NULL, 1,  4,  4),
    INSTRUCTION("LD H,H",           NULL, 1,  4,  4),
    INSTRUCTION("LD H,L",           NULL, 1,  4,  4),
    INSTRUCTION("LD H,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("LD H,A",           NULL, 1,  4,  4),
    INSTRUCTION("LD L,B",           NULL, 1,  4,  4),
    INSTRUCTION("LD L,C",           NULL, 1,  4,  4),
    INSTRUCTION("LD L,D",           NULL, 1,  4,  4),
    INSTRUCTION("LD L,E",           NULL, 1,  4,  4),
    INSTRUCTION("LD L,H",           NULL, 1,  4,  4),
    INSTRUCTION("LD L,L",           NULL, 1,  4,  4),
    INSTRUCTION("LD L,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("LD L,A",           NULL, 1,  4,  4),

    //0x70
    INSTRUCTION("LD (HL),B",        NULL, 1,  8,  8),
    INSTRUCTION("LD (HL),C",        NULL, 1,  8,  8),
    INSTRUCTION("LD (HL),D",        NULL, 1,  8,  8),
    INSTRUCTION("LD (HL),E",        NULL, 1,  8,  8),
    INSTRUCTION("LD (HL),H",        NULL, 1,  8,  8),
    INSTRUCTION("LD (HL),L",        NULL, 1,  8,  8),
    INSTRUCTION("HALT",             NULL, 1,  4,  4),
    INSTRUCTION("LD (HL),A",        NULL, 1,  8,  8),
    INSTRUCTION("LD A,B",           NULL, 1,  4,  4),
    INSTRUCTION("LD A,C",           NULL, 1,  4,  4),
    INSTRUCTION("LD A,D",           NULL, 1,  4,  4),
    INSTRUCTION("LD A,E",           NULL, 1,  4,  4),
    INSTRUCTION("LD A,H",           NULL, 1,  4,  4),
    INSTRUCTION("LD A,L",           NULL, 1,  4,  4),
    INSTRUCTION("LD A,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("LD A,A",           NULL, 1,  4,  4),

    //0x80  
    INSTRUCTION("ADD A,B",          NULL, 1,  4,  4),
    INSTRUCTION("ADD A,C",          NULL, 1,  4,  4),
    INSTRUCTION("ADD A,D",          NULL, 1,  4,  4),
    INSTRUCTION("ADD A,E",          NULL, 1,  4,  4),
    INSTRUCTION("ADD A,H",          NULL, 1,  4,  4),
    INSTRUCTION("ADD A,L",          NULL, 1,  4,  4),
    INSTRUCTION("ADD A,(HL)",       NULL, 1,  8,  8),
    INSTRUCTION("ADD A,A",          NULL, 1,  4,  4),
    INSTRUCTION("ADC A,B",          NULL, 1,  4,  4),
    INSTRUCTION("ADC A,C",          NULL, 1,  4,  4),
    INSTRUCTION("ADC A,D",          NULL, 1,  4,  4),
    INSTRUCTION("ADC A,E",          NULL, 1,  4,  4),
    INSTRUCTION("ADC A,H",          NULL, 1,  4,  4),
    INSTRUCTION("ADC A,L",          NULL, 1,  4,  4),
    INSTRUCTION("ADC A,(HL)",       NULL, 1,  8,  8),
    INSTRUCTION("ADC A,A",          NULL, 1,  4,  4),

    //0x90
    INSTRUCTION("SUB A,B",          NULL, 1,  4,  4),
    INSTRUCTION("SUB A,C",          NULL, 1,  4,  4),
    INSTRUCTION("SUB A,D",          NULL, 1,  4,  4),
    INSTRUCTION("SUB A,E",          NULL, 1,  4,  4),
    INSTRUCTION("SUB A,H",          NULL, 1,  4,  4),
    INSTRUCTION("SUB A,L",          NULL, 1,  4,  4),
    INSTRUCTION("SUB A,(HL)",       NULL, 1,  8,  8),
    INSTRUCTION("SUB A,A",          NULL, 1,  4,  4),
    INSTRUCTION("SBC A,B",          NULL, 1,  4,  4),
    INSTRUCTION("SBC A,C",          NULL, 1,  4,  4),
    INSTRUCTION("SBC A,D",          NULL, 1,  4,  4),
    INSTRUCTION("SBC A,E",          NULL, 1,  4,  4),
    INSTRUCTION("SBC A,H",          NULL, 1,  4,  4),
    INSTRUCTION("SBC A,L",          NULL, 1,  4,  4),
    INSTRUCTION("SBC A,(HL)",       NULL, 1,  8,  8),
    INSTRUCTION("SBC A,A",          NULL, 1,  4,  4),

    //0xA0
    INSTRUCTION("AND A,B",          NULL, 1,  4,  4),
    INSTRUCTION("AND A,C",          NULL, 1,  4,  4),
    INSTRUCTION("AND A,D",          NULL, 1,  4,  4),
    INSTRUCTION("AND A,E",          NULL, 1,  4,  4),
    INSTRUCTION("AND A,H",          NULL, 1,  4,  4),
    INSTRUCTION("AND A,L",          NULL, 1,  4,  4),
    INSTRUCTION("AND A,(HL)",       NULL, 1,  8,  8),
    INSTRUCTION("AND A,A",          NULL, 1,  4,  4),
    INSTRUCTION("XOR A,B",          NULL, 1,  4,  4),
    INSTRUCTION("XOR A,C",          NULL, 1,  4,  4),
    INSTRUCTION("XOR A,D",          NULL, 1,  4,  4),
    INSTRUCTION("XOR A,E",          NULL, 1,  4,  4),
    INSTRUCTION("XOR A,H",          NULL, 1,  4,  4),
    INSTRUCTION("XOR A,L",          NULL, 1,  4,  4),
    INSTRUCTION("XOR A,(HL)",       NULL, 1,  8,  8),
    INSTRUCTION("XOR A,A",          Instruction::opAF, 1,  4,  4),

    //0xB0
    INSTRUCTION("OR A,B",           NULL, 1,  4,  4),
    INSTRUCTION("OR A,C",           NULL, 1,  4,  4),
    INSTRUCTION("OR A,D",           NULL, 1,  4,  4),
    INSTRUCTION("OR A,E",           NULL, 1,  4,  4),
    INSTRUCTION("OR A,H",           NULL, 1,  4,  4),
    INSTRUCTION("OR A,L",           NULL, 1,  4,  4),
    INSTRUCTION("OR A,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("OR A,A",           NULL, 1,  4,  4),
    INSTRUCTION("CP A,B",           NULL, 1,  4,  4),
    INSTRUCTION("CP A,C",           NULL, 1,  4,  4),
    INSTRUCTION("CP A,D",           NULL, 1,  4,  4),
    INSTRUCTION("CP A,E",           NULL, 1,  4,  4),
    INSTRUCTION("CP A,H",           NULL, 1,  4,  4),
    INSTRUCTION("CP A,L",           NULL, 1,  4,  4),
    INSTRUCTION("CP A,(HL)",        NULL, 1,  8,  8),
    INSTRUCTION("CP A,A",           NULL, 1,  4,  4),

    //0xC0
    INSTRUCTION("RET NZ",           NULL, 1, 20,  8),
    INSTRUCTION("POP BC",           NULL, 1, 12, 12),
    INSTRUCTION("JP NZ,u16",        NULL, 3, 16, 12),
    INSTRUCTION("JP u16",           Instruction::opC3, 3, 16, 16),
    INSTRUCTION("CALL NZ,u16",      NULL, 3, 24, 12),
    INSTRUCTION("PUSH BC",          NULL, 1, 16, 16),
    INSTRUCTION("ADD A,u8",         NULL, 2,  8,  8),
    INSTRUCTION("RST 00h",          NULL, 1, 16, 16),
    INSTRUCTION("RET Z",            NULL, 1, 20,  8),
    INSTRUCTION("RET",              NULL, 1, 16, 16),
    INSTRUCTION("JP Z,u16",         NULL, 3, 16, 12),
    INSTRUCTION("PREFIX CB",        NULL, 1,  4,  4),
    INSTRUCTION("CALL Z,u16",       NULL, 3, 24, 12),
    INSTRUCTION("CALL u16",         NULL, 3, 24, 24),
    INSTRUCTION("ADC A,u8",         NULL, 2,  8,  8),
    INSTRUCTION("RST 08h",          NULL, 1, 16, 16),

    //0xD0
    INSTRUCTION("RET NC",           NULL, 1, 20,  8),
    INSTRUCTION("POP DE",           NULL, 1, 12, 12),
    INSTRUCTION("JP NC,u16",        NULL, 3, 16, 12),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("CALL NC,u16",      NULL, 3, 24, 12),
    INSTRUCTION("PUSH DE",          NULL, 1, 16, 16),
    INSTRUCTION("SUB A,u8",         NULL, 2,  8,  8),
    INSTRUCTION("RST 10h",          NULL, 1, 16, 16),
    INSTRUCTION("RET C",            NULL, 1, 20,  8),
    INSTRUCTION("RETI",             NULL, 1, 16, 16),
    INSTRUCTION("JP C,u16",         NULL, 3, 16, 12),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("CALL C,u16",       NULL, 3, 24, 12),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("SBC A,u8",         NULL, 2,  8,  8),
    INSTRUCTION("RST 18h",          NULL, 1, 16, 16),

    //0xE0
    INSTRUCTION("LD (FF00+u8),A",   Instruction::opE0, 2, 12, 12),
    INSTRUCTION("POP HL",           NULL, 1, 12, 12),
    INSTRUCTION("LD (FF00+C),A",    NULL, 1,  8,  8),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("PUSH HL",          NULL, 1, 16, 16),
    INSTRUCTION("AND A,u8",         NULL, 2,  8,  8),
    INSTRUCTION("RST 20h",          NULL, 1, 16, 16),
    INSTRUCTION("ADD SP,i8",        NULL, 2, 16, 16),
    INSTRUCTION("JP HL",            NULL, 1,  4,  4),
    INSTRUCTION("LD (u16),A",       NULL, 3, 16, 16),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("XOR A,u8",         NULL, 2,  8,  8),
    INSTRUCTION("RST 28h",          NULL, 1, 16, 16),

    //0xF0
    INSTRUCTION("LD A,(FF00+u8)",   Instruction::opF0, 2, 12, 12),
    INSTRUCTION("POP AF",           NULL, 1, 12, 12),
    INSTRUCTION("LD A,(FF00+C)",    NULL, 1,  8,  8),
    INSTRUCTION("DI",               Instruction::opF3, 1,  4,  4),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("PUSH AF",          NULL, 1, 16, 16),
    INSTRUCTION("OR A,u8",          NULL, 2,  8,  8),
    INSTRUCTION("RST 30h",          NULL, 1, 16, 16),
    INSTRUCTION("LD HL,SP+i8",      NULL, 2, 12, 12),
    INSTRUCTION("LD SP,HL",         NULL, 1,  8,  8),
    INSTRUCTION("LD A,(u16)",       NULL, 3, 16, 16),
    INSTRUCTION("EI",               Instruction::opFB, 1,  4,  4),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
    INSTRUCTION("CP A,u8",          Instruction::opFE, 2,  8,  8),
    INSTRUCTION("RST 38h",          NULL, 1, 16, 16)
};

//------------------------------CB Prefixed------------------------------//

//0x00

//0x10

//0x20

//0x30

//0x40

//0x50

//0x60

//0x70

//0x80

//0x90

//0xA0

//0xB0

//0xC0

//0xD0

//0xE0

//0xF0

const Instruction Instruction::instructionsCB[0x100] = 
{
    //0x0
    INSTRUCTION("RLC B",            NULL, 2,  8,  8),
    INSTRUCTION("RLC C",            NULL, 2,  8,  8),
    INSTRUCTION("RLC D",            NULL, 2,  8,  8),
    INSTRUCTION("RLC E",            NULL, 2,  8,  8),
    INSTRUCTION("RLC H",            NULL, 2,  8,  8),
    INSTRUCTION("RLC L",            NULL, 2,  8,  8),
    INSTRUCTION("RLC (HL)",         NULL, 2, 16, 16),
    INSTRUCTION("RLC A",            NULL, 2,  8,  8),
    INSTRUCTION("RRC B",            NULL, 2,  8,  8),
    INSTRUCTION("RRC C",            NULL, 2,  8,  8),
    INSTRUCTION("RRC D",            NULL, 2,  8,  8),
    INSTRUCTION("RRC E",            NULL, 2,  8,  8),
    INSTRUCTION("RRC H",            NULL, 2,  8,  8),
    INSTRUCTION("RRC L",            NULL, 2,  8,  8),
    INSTRUCTION("RRC (HL)",         NULL, 2, 16, 16),
    INSTRUCTION("RRC A",            NULL, 2,  8,  8),

    //0x10
    INSTRUCTION("RL B",             NULL, 2,  8,  8),
    INSTRUCTION("RL C",             NULL, 2,  8,  8),
    INSTRUCTION("RL D",             NULL, 2,  8,  8),
    INSTRUCTION("RL E",             NULL, 2,  8,  8),
    INSTRUCTION("RL H",             NULL, 2,  8,  8),
    INSTRUCTION("RL L",             NULL, 2,  8,  8),
    INSTRUCTION("RL (HL)",          NULL, 2, 16, 16),
    INSTRUCTION("RL A",             NULL, 2,  8,  8),
    INSTRUCTION("RR B",             NULL, 2,  8,  8),
    INSTRUCTION("RR C",             NULL, 2,  8,  8),
    INSTRUCTION("RR D",             NULL, 2,  8,  8),
    INSTRUCTION("RR E",             NULL, 2,  8,  8),
    INSTRUCTION("RR H",             NULL, 2,  8,  8),
    INSTRUCTION("RR L",             NULL, 2,  8,  8),
    INSTRUCTION("RR (HL)",          NULL, 2, 16, 16),
    INSTRUCTION("RR A",             NULL, 2,  8,  8),

    //0x20
    INSTRUCTION("SLA B",            NULL, 2,  8,  8),
    INSTRUCTION("SLA C",            NULL, 2,  8,  8),
    INSTRUCTION("SLA D",            NULL, 2,  8,  8),
    INSTRUCTION("SLA E",            NULL, 2,  8,  8),
    INSTRUCTION("SLA H",            NULL, 2,  8,  8),
    INSTRUCTION("SLA L",            NULL, 2,  8,  8),
    INSTRUCTION("SLA (HL)",         NULL, 2, 16, 16),
    INSTRUCTION("SLA A",            NULL, 2,  8,  8),
    INSTRUCTION("SRA B",            NULL, 2,  8,  8),
    INSTRUCTION("SRA C",            NULL, 2,  8,  8),
    INSTRUCTION("SRA D",            NULL, 2,  8,  8),
    INSTRUCTION("SRA E",            NULL, 2,  8,  8),
    INSTRUCTION("SRA H",            NULL, 2,  8,  8),
    INSTRUCTION("SRA L",            NULL, 2,  8,  8),
    INSTRUCTION("SRA (HL)",         NULL, 2, 16, 16),
    INSTRUCTION("SRA A",            NULL, 2,  8,  8),

    //0x30
    INSTRUCTION("SWAP B",           NULL, 2,  8,  8),
    INSTRUCTION("SWAP C",           NULL, 2,  8,  8),
    INSTRUCTION("SWAP D",           NULL, 2,  8,  8),
    INSTRUCTION("SWAP E",           NULL, 2,  8,  8),
    INSTRUCTION("SWAP H",           NULL, 2,  8,  8),
    INSTRUCTION("SWAP L",           NULL, 2,  8,  8),
    INSTRUCTION("SWAP (HL)",        NULL, 2, 16, 16),
    INSTRUCTION("SWAP A",           NULL, 2,  8,  8),
    INSTRUCTION("SRL B",            NULL, 2,  8,  8),
    INSTRUCTION("SRL C",            NULL, 2,  8,  8),
    INSTRUCTION("SRL D",            NULL, 2,  8,  8),
    INSTRUCTION("SRL E",            NULL, 2,  8,  8),
    INSTRUCTION("SRL H",            NULL, 2,  8,  8),
    INSTRUCTION("SRL L",            NULL, 2,  8,  8),
    INSTRUCTION("SRL (HL)",         NULL, 2, 16, 16),
    INSTRUCTION("SRL A",            NULL, 2,  8,  8),

    //0x40
    INSTRUCTION("BIT 0,B",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 0,C",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 0,D",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 0,E",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 0,H",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 0,L",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 0,(HL)",       NULL, 2, 12, 12),
    INSTRUCTION("BIT 0,A",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 1,B",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 1,C",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 1,D",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 1,E",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 1,H",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 1,L",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 1,(HL)",       NULL, 2, 12, 12),
    INSTRUCTION("BIT 1,A",          NULL, 2,  8,  8),

    //0x50
    INSTRUCTION("BIT 2,B",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 2,C",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 2,D",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 2,E",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 2,H",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 2,L",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 2,(HL)",       NULL, 2, 12, 12),
    INSTRUCTION("BIT 2,A",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 3,B",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 3,C",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 3,D",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 3,E",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 3,H",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 3,L",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 3,(HL)",       NULL, 2, 12, 12),
    INSTRUCTION("BIT 3,A",          NULL, 2,  8,  8),

    //0x60
    INSTRUCTION("BIT 4,B",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 4,C",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 4,D",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 4,E",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 4,H",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 4,L",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 4,(HL)",       NULL, 2, 12, 12),
    INSTRUCTION("BIT 4,A",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 5,B",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 5,C",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 5,D",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 5,E",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 5,H",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 5,L",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 5,(HL)",       NULL, 2, 12, 12),
    INSTRUCTION("BIT 5,A",          NULL, 2,  8,  8),

    //0x70
    INSTRUCTION("BIT 6,B",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 6,C",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 6,D",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 6,E",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 6,H",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 6,L",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 6,(HL)",       NULL, 2, 12, 12),
    INSTRUCTION("BIT 6,A",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 7,B",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 7,C",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 7,D",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 7,E",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 7,H",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 7,L",          NULL, 2,  8,  8),
    INSTRUCTION("BIT 7,(HL)",       NULL, 2, 12, 12),
    INSTRUCTION("BIT 7,A",          NULL, 2,  8,  8),

    //0x80
    INSTRUCTION("RES 0,B",          NULL, 2,  8,  8),
    INSTRUCTION("RES 0,C",          NULL, 2,  8,  8),
    INSTRUCTION("RES 0,D",          NULL, 2,  8,  8),
    INSTRUCTION("RES 0,E",          NULL, 2,  8,  8),
    INSTRUCTION("RES 0,H",          NULL, 2,  8,  8),
    INSTRUCTION("RES 0,L",          NULL, 2,  8,  8),
    INSTRUCTION("RES 0,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("RES 0,A",          NULL, 2,  8,  8),
    INSTRUCTION("RES 1,B",          NULL, 2,  8,  8),
    INSTRUCTION("RES 1,C",          NULL, 2,  8,  8),
    INSTRUCTION("RES 1,D",          NULL, 2,  8,  8),
    INSTRUCTION("RES 1,E",          NULL, 2,  8,  8),
    INSTRUCTION("RES 1,H",          NULL, 2,  8,  8),
    INSTRUCTION("RES 1,L",          NULL, 2,  8,  8),
    INSTRUCTION("RES 1,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("RES 1,A",          NULL, 2,  8,  8),

    //0x90
    INSTRUCTION("RES 2,B",          NULL, 2,  8,  8),
    INSTRUCTION("RES 2,C",          NULL, 2,  8,  8),
    INSTRUCTION("RES 2,D",          NULL, 2,  8,  8),
    INSTRUCTION("RES 2,E",          NULL, 2,  8,  8),
    INSTRUCTION("RES 2,H",          NULL, 2,  8,  8),
    INSTRUCTION("RES 2,L",          NULL, 2,  8,  8),
    INSTRUCTION("RES 2,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("RES 2,A",          NULL, 2,  8,  8),
    INSTRUCTION("RES 3,B",          NULL, 2,  8,  8),
    INSTRUCTION("RES 3,C",          NULL, 2,  8,  8),
    INSTRUCTION("RES 3,D",          NULL, 2,  8,  8),
    INSTRUCTION("RES 3,E",          NULL, 2,  8,  8),
    INSTRUCTION("RES 3,H",          NULL, 2,  8,  8),
    INSTRUCTION("RES 3,L",          NULL, 2,  8,  8),
    INSTRUCTION("RES 3,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("RES 3,A",          NULL, 2,  8,  8),

    //0xA0
    INSTRUCTION("RES 4,B",          NULL, 2,  8,  8),
    INSTRUCTION("RES 4,C",          NULL, 2,  8,  8),
    INSTRUCTION("RES 4,D",          NULL, 2,  8,  8),
    INSTRUCTION("RES 4,E",          NULL, 2,  8,  8),
    INSTRUCTION("RES 4,H",          NULL, 2,  8,  8),
    INSTRUCTION("RES 4,L",          NULL, 2,  8,  8),
    INSTRUCTION("RES 4,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("RES 4,A",          NULL, 2,  8,  8),
    INSTRUCTION("RES 5,B",          NULL, 2,  8,  8),
    INSTRUCTION("RES 5,C",          NULL, 2,  8,  8),
    INSTRUCTION("RES 5,D",          NULL, 2,  8,  8),
    INSTRUCTION("RES 5,E",          NULL, 2,  8,  8),
    INSTRUCTION("RES 5,H",          NULL, 2,  8,  8),
    INSTRUCTION("RES 5,L",          NULL, 2,  8,  8),
    INSTRUCTION("RES 5,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("RES 5,A",          NULL, 2,  8,  8),

    //0xB0
    INSTRUCTION("RES 6,B",          NULL, 2,  8,  8),
    INSTRUCTION("RES 6,C",          NULL, 2,  8,  8),
    INSTRUCTION("RES 6,D",          NULL, 2,  8,  8),
    INSTRUCTION("RES 6,E",          NULL, 2,  8,  8),
    INSTRUCTION("RES 6,H",          NULL, 2,  8,  8),
    INSTRUCTION("RES 6,L",          NULL, 2,  8,  8),
    INSTRUCTION("RES 6,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("RES 6,A",          NULL, 2,  8,  8),
    INSTRUCTION("RES 7,B",          NULL, 2,  8,  8),
    INSTRUCTION("RES 7,C",          NULL, 2,  8,  8),
    INSTRUCTION("RES 7,D",          NULL, 2,  8,  8),
    INSTRUCTION("RES 7,E",          NULL, 2,  8,  8),
    INSTRUCTION("RES 7,H",          NULL, 2,  8,  8),
    INSTRUCTION("RES 7,L",          NULL, 2,  8,  8),
    INSTRUCTION("RES 7,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("RES 7,A",          NULL, 2,  8,  8),

    //0xC0
    INSTRUCTION("SET 0,B",          NULL, 2,  8,  8),
    INSTRUCTION("SET 0,C",          NULL, 2,  8,  8),
    INSTRUCTION("SET 0,D",          NULL, 2,  8,  8),
    INSTRUCTION("SET 0,E",          NULL, 2,  8,  8),
    INSTRUCTION("SET 0,H",          NULL, 2,  8,  8),
    INSTRUCTION("SET 0,L",          NULL, 2,  8,  8),
    INSTRUCTION("SET 0,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("SET 0,A",          NULL, 2,  8,  8),
    INSTRUCTION("SET 1,B",          NULL, 2,  8,  8),
    INSTRUCTION("SET 1,C",          NULL, 2,  8,  8),
    INSTRUCTION("SET 1,D",          NULL, 2,  8,  8),
    INSTRUCTION("SET 1,E",          NULL, 2,  8,  8),
    INSTRUCTION("SET 1,H",          NULL, 2,  8,  8),
    INSTRUCTION("SET 1,L",          NULL, 2,  8,  8),
    INSTRUCTION("SET 1,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("SET 1,A",          NULL, 2,  8,  8),

    //0xD0
    INSTRUCTION("SET 2,B",          NULL, 2,  8,  8),
    INSTRUCTION("SET 2,C",          NULL, 2,  8,  8),
    INSTRUCTION("SET 2,D",          NULL, 2,  8,  8),
    INSTRUCTION("SET 2,E",          NULL, 2,  8,  8),
    INSTRUCTION("SET 2,H",          NULL, 2,  8,  8),
    INSTRUCTION("SET 2,L",          NULL, 2,  8,  8),
    INSTRUCTION("SET 2,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("SET 2,A",          NULL, 2,  8,  8),
    INSTRUCTION("SET 3,B",          NULL, 2,  8,  8),
    INSTRUCTION("SET 3,C",          NULL, 2,  8,  8),
    INSTRUCTION("SET 3,D",          NULL, 2,  8,  8),
    INSTRUCTION("SET 3,E",          NULL, 2,  8,  8),
    INSTRUCTION("SET 3,H",          NULL, 2,  8,  8),
    INSTRUCTION("SET 3,L",          NULL, 2,  8,  8),
    INSTRUCTION("SET 3,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("SET 3,A",          NULL, 2,  8,  8),

    //0xE0
    INSTRUCTION("SET 4,B",          NULL, 2,  8,  8),
    INSTRUCTION("SET 4,C",          NULL, 2,  8,  8),
    INSTRUCTION("SET 4,D",          NULL, 2,  8,  8),
    INSTRUCTION("SET 4,E",          NULL, 2,  8,  8),
    INSTRUCTION("SET 4,H",          NULL, 2,  8,  8),
    INSTRUCTION("SET 4,L",          NULL, 2,  8,  8),
    INSTRUCTION("SET 4,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("SET 4,A",          NULL, 2,  8,  8),
    INSTRUCTION("SET 5,B",          NULL, 2,  8,  8),
    INSTRUCTION("SET 5,C",          NULL, 2,  8,  8),
    INSTRUCTION("SET 5,D",          NULL, 2,  8,  8),
    INSTRUCTION("SET 5,E",          NULL, 2,  8,  8),
    INSTRUCTION("SET 5,H",          NULL, 2,  8,  8),
    INSTRUCTION("SET 5,L",          NULL, 2,  8,  8),
    INSTRUCTION("SET 5,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("SET 5,A",          NULL, 2,  8,  8),

    //0xF0
    INSTRUCTION("SET 6,B",          NULL, 2,  8,  8),
    INSTRUCTION("SET 6,C",          NULL, 2,  8,  8),
    INSTRUCTION("SET 6,D",          NULL, 2,  8,  8),
    INSTRUCTION("SET 6,E",          NULL, 2,  8,  8),
    INSTRUCTION("SET 6,H",          NULL, 2,  8,  8),
    INSTRUCTION("SET 6,L",          NULL, 2,  8,  8),
    INSTRUCTION("SET 6,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("SET 6,A",          NULL, 2,  8,  8),
    INSTRUCTION("SET 7,B",          NULL, 2,  8,  8),
    INSTRUCTION("SET 7,C",          NULL, 2,  8,  8),
    INSTRUCTION("SET 7,D",          NULL, 2,  8,  8),
    INSTRUCTION("SET 7,E",          NULL, 2,  8,  8),
    INSTRUCTION("SET 7,H",          NULL, 2,  8,  8),
    INSTRUCTION("SET 7,L",          NULL, 2,  8,  8),
    INSTRUCTION("SET 7,(HL)",       NULL, 2, 16, 16),
    INSTRUCTION("SET 7,A",          NULL, 2,  8,  8)
};