#include "instruction.h"
#include "cpu.h"
#include "mmu.h"

//--------------------------------------Log Helper Macros--------------------------------------//

#define LOG_A_REG() LOG("A Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.A) << ".")
#define LOG_F_REG() LOG("F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.F) << ".")
#define LOG_B_REG() LOG("B Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.B) << ".")
#define LOG_C_REG() LOG("C Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.C) << ".")
#define LOG_D_REG() LOG("D Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.D) << ".")
#define LOG_E_REG() LOG("E Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.E) << ".")
#define LOG_H_REG() LOG("H Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.H) << ".")
#define LOG_L_REG() LOG("F Register updated to: 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.L) << ".")

#define LOG_FLAGS() LOG("Flags updated to: "                                          \
                        << ((m_Registers.F & Flags::Register::Zero)      ? "Z" : "_") \
                        << ((m_Registers.F & Flags::Register::Negative)  ? "N" : "_") \
                        << ((m_Registers.F & Flags::Register::HalfCarry) ? "H" : "_") \
                        << ((m_Registers.F & Flags::Register::Carry)     ? "C" : "_") \
                        << ".")

#define LOG_AF_REG() LOG("AF Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.AF) << ".")
#define LOG_BC_REG() LOG("BC Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.BC) << ".")
#define LOG_DE_REG() LOG("DE Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.DE) << ".")
#define LOG_HL_REG() LOG("HL Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.HL) << ".")
#define LOG_SP_REG() LOG("SP Register updated to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(m_Registers.SP) << ".")

#define LOG_WRITE(addr, val) LOG("Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(val) << " to address 0x" << std::setw(4) << addr << ".")

#define LOG_JP(addr) LOG("Jumped to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(addr) << ".");
#define LOG_NJP() LOG("Did not jump.");

#define LOG_RET(addr) LOG("Returned to: 0x" << std::setw(4) << std::setfill('0') << std::hex << static_cast<u16>(addr) << ".");
#define LOG_NRET() LOG("Did not return.");

#define LOG_DI() LOG("Disabled Interrupts.")
#define LOG_EI() LOG("Enabled Interrupts.")

//--------------------------------------Opcode Helpers--------------------------------------//

void CPU::opcodeINC(u8& reg)
{

}

void CPU::opcodeDEC(u8& reg)
{
    reg--;
    clearFlag(Flags::Register::Zero | Flags::Register::Negative | Flags::Register::HalfCarry);
    setZeroFromVal(reg);
    setFlag(Flags::Register::Negative);
    if((reg & 0x0f) == 0x0f) setFlag(Flags::Register::HalfCarry);   
}

void CPU::opcodeADD(const u8& val)
{

}

void CPU::opcodeADC(const u8& val)
{

}

void CPU::opcodeSUB(const u8& val)
{

}

void CPU::opcodeSBC(const u8& val)
{

}

void CPU::opcodeAND(const u8& val)
{

}

void CPU::opcodeXOR(const u8& val)
{
    m_Registers.A ^= val;
    clearAllFlags();
    setZeroFromVal(m_Registers.A);
}

void CPU::opcodeOR(const u8& val)
{

}

void CPU::opcodeCP(const u8& val)
{
    const u8& a = m_Registers.A;
    clearAllFlags();
    setFlag(Flags::Register::Negative);

    if(a < val)
        setFlag(Flags::Register::Carry);

    if(a == val)
        setFlag(Flags::Register::Zero);

    if((a & 0x0F) < ((a - val) & 0x0F))
        setFlag(Flags::Register::HalfCarry);

}

void CPU::opcodeJP(bool condition)
{
    u8 low  = m_MMU->read(m_Registers.PC++);
    u8 high = m_MMU->read(m_Registers.PC++);
    u16 addr = static_cast<u16>(high) << 8 | static_cast<u16>(low);

    if(condition)
    {
        m_Registers.PC = addr;
        LOG_JP(addr);
    }
    else
    {
        LOG_NJP();
    }
}

void CPU::opcodeJR(bool condition)
{
    i8 offset = m_MMU->read(m_Registers.PC++);
    u16 addr = m_Registers.PC + offset;

    if(condition)
    {
        m_Registers.PC = addr;
        LOG_JP(addr);
    }
    else
    {
        LOG_NJP();
    }
}

void CPU::opcodeCALL(bool condition)
{

}

void CPU::opcodeRET(bool condiiton)
{

}

void CPU::opcodePUSH(const u8& reg)
{

}

void CPU::opcodePOP(u8& reg)
{
    
}

//--------------------------------------Opcodes--------------------------------------//

//0x00

void CPU::op00() // NOP
{
    
}

void CPU::op01() // LD BC,u16
{

}

void CPU::op02() // LD (BC),A
{

}

void CPU::op03() // INC BC
{

}

void CPU::op04() // INC B
{

}

void CPU::op05() // DEC B
{
    opcodeDEC(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::op06() // LD B,u8
{
    m_Registers.B = m_MMU->read(m_Registers.PC++);

    LOG_B_REG();
}

void CPU::op07() // RLCA
{

}

void CPU::op08() // LD (u16),SP
{

}

void CPU::op09() // ADD HL,BC
{

}

void CPU::op0A() // LD A,(BC)
{

}

void CPU::op0B() // DEC BC
{

}

void CPU::op0C() // INC C
{

}

void CPU::op0D() // DEC C
{
    opcodeDEC(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::op0E() // LD C,u8
{
    m_Registers.C = m_MMU->read(m_Registers.PC++);

    LOG_C_REG();
}

void CPU::op0F() // RRCA
{

}

//0x10

void CPU::op10() // STOP
{

}

void CPU::op11() // LD DE,u16
{

}

void CPU::op12() // LD (DE),A
{

}

void CPU::op13() // INC DE
{

}

void CPU::op14() // INC D
{

}

void CPU::op15() // DEC D
{

}

void CPU::op16() // LD D,u8
{

}

void CPU::op17() // RLA
{

}

void CPU::op18() // JR i8
{

}

void CPU::op19() // ADD HL,DE
{

}

void CPU::op1A() // LD A,(DE)
{

}

void CPU::op1B() // DEC DE
{

}

void CPU::op1C() // INC E
{

}

void CPU::op1D() // DEC E
{

}

void CPU::op1E() // LD E,u8
{

}

void CPU::op1F() // RRA
{

}

//0x20

void CPU::op20() // JR NZ,i8
{
    opcodeJR(!isFlagSet(Flags::Register::Zero));
}

void CPU::op21() // LD HL,u16
{
    u8 low  = m_MMU->read(m_Registers.PC++);
    u8 high = m_MMU->read(m_Registers.PC++);
    m_Registers.HL = static_cast<u16>(high) << 8 | static_cast<u16>(low);

    LOG_HL_REG();
}

void CPU::op22() // LD (HL+),A
{

}

void CPU::op23() // INC HL
{

}

void CPU::op24() // INC H
{

}

void CPU::op25() // DEC H
{

}

void CPU::op26() // LD H,u8
{

}

void CPU::op27() // DAA
{

}

void CPU::op28() // JR Z,i8
{

}

void CPU::op29() // ADD HL,HL
{

}

void CPU::op2A() // LD A,(HL+)
{

}

void CPU::op2B() // DEC HL
{

}

void CPU::op2C() // INC L
{

}

void CPU::op2D() // DEC L
{

}

void CPU::op2E() // LD L,u8
{

}

void CPU::op2F() // CPL
{

}

//0x30

void CPU::op30() // JR NC,i8
{

}

void CPU::op31() // LD SP,u16
{

}

void CPU::op32() // LD (HL-),A
{
    m_MMU->write(m_Registers.HL--, m_Registers.A);

    LOG_WRITE(m_Registers.HL + 1, m_Registers.A);
}

void CPU::op33() // INC SP
{

}

void CPU::op34() // INC (HL)
{

}

void CPU::op35() // DEC (HL)
{

}

void CPU::op36() // LD (HL),u8
{

}

void CPU::op37() // SCF
{

}

void CPU::op38() // JR C,i8
{

}

void CPU::op39() // ADD HL,SP
{

}

void CPU::op3A() // LD A,(HL-)
{

}

void CPU::op3B() // DEC SP
{

}

void CPU::op3C() // INC A
{

}

void CPU::op3D() // DEC A
{

}

void CPU::op3E() // LD A,u8
{
    m_Registers.A = m_MMU->read(m_Registers.PC++);

    LOG_A_REG();
}

void CPU::op3F() // CCF
{

}

//0x40

void CPU::op40() // LD B,B
{

}

void CPU::op41() // LD B,C
{

}

void CPU::op42() // LD B,D
{

}

void CPU::op43() // LD B,E
{

}

void CPU::op44() // LD B,H
{

}

void CPU::op45() // LD B,L
{

}

void CPU::op46() // LD B,(HL)
{

}

void CPU::op47() // LD B,A
{

}

void CPU::op48() // LD C,B
{

}

void CPU::op49() // LD C,C
{

}

void CPU::op4A() // LD C,D
{

}

void CPU::op4B() // LD C,E
{

}

void CPU::op4C() // LD C,H
{

}

void CPU::op4D() // LD C,L
{

}

void CPU::op4E() // LD C,(HL)
{

}

void CPU::op4F() // LD C,A
{

}

//0x50

void CPU::op50() // LD D,B
{

}

void CPU::op51() // LD D,C
{

}

void CPU::op52() // LD D,D
{

}

void CPU::op53() // LD D,E
{

}

void CPU::op54() // LD D,H
{

}

void CPU::op55() // LD D,L
{

}

void CPU::op56() // LD D,(HL)
{

}

void CPU::op57() // LD D,A
{

}

void CPU::op58() // LD E,B
{

}

void CPU::op59() // LD E,C
{

}

void CPU::op5A() // LD E,D
{

}

void CPU::op5B() // LD E,E
{

}

void CPU::op5C() // LD E,H
{

}

void CPU::op5D() // LD E,L
{

}

void CPU::op5E() // LD E,(HL)
{

}

void CPU::op5F() // LD E,A
{

}

//0x60

void CPU::op60() // LD H,B
{

}

void CPU::op61() // LD H,C
{

}

void CPU::op62() // LD H,D
{

}

void CPU::op63() // LD H,E
{

}

void CPU::op64() // LD H,H
{

}

void CPU::op65() // LD H,L
{

}

void CPU::op66() // LD H,(HL)
{

}

void CPU::op67() // LD H,A
{

}

void CPU::op68() // LD L,B
{

}

void CPU::op69() // LD L,C
{

}

void CPU::op6A() // LD L,D
{

}

void CPU::op6B() // LD L,E
{

}

void CPU::op6C() // LD L,H
{

}

void CPU::op6D() // LD L,L
{

}

void CPU::op6E() // LD L,(HL)
{

}

void CPU::op6F() // LD L,A
{

}

//0x70

void CPU::op70() // LD (HL),B
{

}

void CPU::op71() // LD (HL),C
{

}

void CPU::op72() // LD (HL),D
{

}

void CPU::op73() // LD (HL),E
{

}

void CPU::op74() // LD (HL),H
{

}

void CPU::op75() // LD (HL),L
{

}

void CPU::op76() // HALT
{

}

void CPU::op77() // LD (HL),A
{

}

void CPU::op78() // LD A,B
{

}

void CPU::op79() // LD A,C
{

}

void CPU::op7A() // LD A,D
{

}

void CPU::op7B() // LD A,E
{

}

void CPU::op7C() // LD A,H
{

}

void CPU::op7D() // LD A,L
{

}

void CPU::op7E() // LD A,(HL)
{

}

void CPU::op7F() // LD A,A
{

}

//0x80

void CPU::op80() // ADD A,B
{

}

void CPU::op81() // ADD A,C
{

}

void CPU::op82() // ADD A,D
{

}

void CPU::op83() // ADD A,E
{

}

void CPU::op84() // ADD A,H
{

}

void CPU::op85() // ADD A,L
{

}

void CPU::op86() // ADD A,(HL)
{

}

void CPU::op87() // ADD A,A
{

}

void CPU::op88() // ADC A,B
{

}

void CPU::op89() // ADC A,C
{

}

void CPU::op8A() // ADC A,D
{

}

void CPU::op8B() // ADC A,E
{

}

void CPU::op8C() // ADC A,H
{

}

void CPU::op8D() // ADC A,L
{

}

void CPU::op8E() // ADC A,(HL)
{

}

void CPU::op8F() // ADC A,A
{

}

//0x90

void CPU::op90() // SUB A,B
{

}

void CPU::op91() // SUB A,C
{

}

void CPU::op92() // SUB A,D
{

}

void CPU::op93() // SUB A,E
{

}

void CPU::op94() // SUB A,H
{

}

void CPU::op95() // SUB A,L
{

}

void CPU::op96() // SUB A,(HL)
{

}

void CPU::op97() // SUB A,A
{

}

void CPU::op98() // SBC A,B
{

}

void CPU::op99() // SBC A,C
{

}

void CPU::op9A() // SBC A,D
{

}

void CPU::op9B() // SBC A,E
{

}

void CPU::op9C() // SBC A,H
{

}

void CPU::op9D() // SBC A,L
{

}

void CPU::op9E() // SBC A,(HL)
{

}

void CPU::op9F() // SBC A,A
{

}

//0xA0

void CPU::opA0() // AND A,B
{

}

void CPU::opA1() // AND A,C
{

}

void CPU::opA2() // AND A,D
{

}

void CPU::opA3() // AND A,E
{

}

void CPU::opA4() // AND A,H
{

}

void CPU::opA5() // AND A,L
{

}

void CPU::opA6() // AND A,(HL)
{

}

void CPU::opA7() // AND A,A
{

}

void CPU::opA8() // XOR A,B
{

}

void CPU::opA9() // XOR A,C
{

}

void CPU::opAA() // XOR A,D
{

}

void CPU::opAB() // XOR A,E
{

}

void CPU::opAC() // XOR A,H
{

}

void CPU::opAD() // XOR A,L
{

}

void CPU::opAE() // XOR A,(HL)
{

}


void CPU::opAF() // XOR A,A
{
    opcodeXOR(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

//0xB0

void CPU::opB0() // OR A,B
{



}
void CPU::opB1() // OR A,C
{

}

void CPU::opB2() // OR A,D
{

}

void CPU::opB3() // OR A,E
{

}

void CPU::opB4() // OR A,H
{

}

void CPU::opB5() // OR A,L
{

}

void CPU::opB6() // OR A,(HL)
{

}

void CPU::opB7() // OR A,A
{

}

void CPU::opB8() // CP A,B
{

}

void CPU::opB9() // CP A,C
{

}

void CPU::opBA() // CP A,D
{

}

void CPU::opBB() // CP A,E
{

}

void CPU::opBC() // CP A,H
{

}

void CPU::opBD() // CP A,L
{

}

void CPU::opBE() // CP A,(HL)
{

}

void CPU::opBF() // CP A,A
{

}

//0xC0

void CPU::opC0() // RET NZ
{

}

void CPU::opC1() // POP BC
{

}

void CPU::opC2() // JP NZ,u16
{

}

void CPU::opC3() // JP u16
{
    opcodeJP(true);
}

void CPU::opC4() // CALL NZ,u16
{

}

void CPU::opC5() // PUSH BC
{

}

void CPU::opC6() // ADD A,u8
{

}

void CPU::opC7() // RST 00h
{

}

void CPU::opC8() // RET Z
{

}

void CPU::opC9() // RET
{

}

void CPU::opCA() // JP Z,u16
{

}

void CPU::opCB() // PREFIX CB
{

}

void CPU::opCC() // CALL Z,u16
{

}

void CPU::opCD() // CALL u16
{

}

void CPU::opCE() // ADC A,u8
{

}

void CPU::opCF() // RST 08h
{

}

//0xD0

void CPU::opD0() // RET NC
{

}

void CPU::opD1() // POP DE
{

}

void CPU::opD2() // JP NC,u16
{

}

void CPU::opD3() // UNUSED
{

}

void CPU::opD4() // CALL NC,u16
{

}

void CPU::opD5() // PUSH DE
{

}

void CPU::opD6() // SUB A,u8
{

}

void CPU::opD7() // RST 10h
{

}

void CPU::opD8() // RET C
{

}

void CPU::opD9() // RETI
{

}

void CPU::opDA() // JP C,u16
{

}

void CPU::opDB() // UNUSED
{

}

void CPU::opDC() // CALL C,u16
{

}

void CPU::opDD() // UNUSED
{

}

void CPU::opDE() // SBC A,u8
{

}

void CPU::opDF() // RST 18h
{

}

//0xE0

void CPU::opE0() // LD (FF00+u8),A
{
    u8 offset = m_Registers.PC++;
    u16 addr = 0xFF00 | offset;
    m_MMU->write(addr, m_Registers.A);

    LOG_WRITE(addr, m_Registers.A);
}

void CPU::opE1() // POP HL
{

}

void CPU::opE2() // LD (FF00+C),A
{

}

void CPU::opE3() // UNUSED
{

}

void CPU::opE4() // UNUSED
{

}

void CPU::opE5() // PUSH HL
{

}

void CPU::opE6() // AND A,u8
{

}

void CPU::opE7() // RST 20h
{

}

void CPU::opE8() // ADD SP,i8
{

}

void CPU::opE9() // JP HL
{

}

void CPU::opEA() // LD (u16),A
{

}

void CPU::opEB() // UNUSED
{

}

void CPU::opEC() // UNUSED
{

}

void CPU::opED() // UNUSED
{

}

void CPU::opEE() // XOR A,u8
{

}

void CPU::opEF() // RST 28h
{

}

//0xF0

void CPU::opF0() // LD A,(FF00+u8)
{
    u8 offset = m_Registers.PC++;
    u16 addr = 0xFF00 | offset;
    m_Registers.A = m_MMU->read(addr);

    LOG_A_REG();
}

void CPU::opF1() // POP AF
{

}

void CPU::opF2() // LD A,(FF00+C)
{

}

void CPU::opF3() // DI
{
    m_IME = false;

    LOG_DI();
}

void CPU::opF4() // UNUSED
{

}

void CPU::opF5() // PUSH AF
{

}

void CPU::opF6() // OR A,u8
{

}

void CPU::opF7() // RST 30h
{

}

void CPU::opF8() // LD HL,SP+i8
{

}

void CPU::opF9() // LD SP,HL
{

}

void CPU::opFA() // LD A,(u16)
{

}

void CPU::opFB() // EI
{
    m_IME = true;

    LOG_EI();
}

void CPU::opFC() // UNUSED
{

}

void CPU::opFD() // UNUSED
{

}

void CPU::opFE() // CP A,u8
{
    opcodeCP(m_Registers.PC++);

    LOG_A_REG();
    LOG_FLAGS();
}

void CPU::opFF() // RST 38h
{

}

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