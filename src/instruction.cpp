#include "instruction.h"
#include "cpu.h"
#include "mmu.h"

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
        m_Branched = true;

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
        m_Branched = true;

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

void CPU::opcode0x00() // NOP
{
    
}

void CPU::opcode0x01() // LD BC,u16
{

}

void CPU::opcode0x02() // LD (BC),A
{

}

void CPU::opcode0x03() // INC BC
{

}

void CPU::opcode0x04() // INC B
{

}

void CPU::opcode0x05() // DEC B
{
    opcodeDEC(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcode0x06() // LD B,u8
{
    m_Registers.B = m_MMU->read(m_Registers.PC++);

    LOG_B_REG();
}

void CPU::opcode0x07() // RLCA
{

}

void CPU::opcode0x08() // LD (u16),SP
{

}

void CPU::opcode0x09() // ADD HL,BC
{

}

void CPU::opcode0x0A() // LD A,(BC)
{

}

void CPU::opcode0x0B() // DEC BC
{

}

void CPU::opcode0x0C() // INC C
{

}

void CPU::opcode0x0D() // DEC C
{
    opcodeDEC(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcode0x0E() // LD C,u8
{
    m_Registers.C = m_MMU->read(m_Registers.PC++);

    LOG_C_REG();
}

void CPU::opcode0x0F() // RRCA
{

}

//0x10

void CPU::opcode0x10() // STOP
{

}

void CPU::opcode0x11() // LD DE,u16
{

}

void CPU::opcode0x12() // LD (DE),A
{

}

void CPU::opcode0x13() // INC DE
{

}

void CPU::opcode0x14() // INC D
{

}

void CPU::opcode0x15() // DEC D
{

}

void CPU::opcode0x16() // LD D,u8
{

}

void CPU::opcode0x17() // RLA
{

}

void CPU::opcode0x18() // JR i8
{

}

void CPU::opcode0x19() // ADD HL,DE
{

}

void CPU::opcode0x1A() // LD A,(DE)
{

}

void CPU::opcode0x1B() // DEC DE
{

}

void CPU::opcode0x1C() // INC E
{

}

void CPU::opcode0x1D() // DEC E
{

}

void CPU::opcode0x1E() // LD E,u8
{

}

void CPU::opcode0x1F() // RRA
{

}

//0x20

void CPU::opcode0x20() // JR NZ,i8
{
    opcodeJR(!isFlagSet(Flags::Register::Zero));
}

void CPU::opcode0x21() // LD HL,u16
{
    u8 low  = m_MMU->read(m_Registers.PC++);
    u8 high = m_MMU->read(m_Registers.PC++);
    m_Registers.HL = static_cast<u16>(high) << 8 | static_cast<u16>(low);

    LOG_HL_REG();
}

void CPU::opcode0x22() // LD (HL+),A
{

}

void CPU::opcode0x23() // INC HL
{

}

void CPU::opcode0x24() // INC H
{

}

void CPU::opcode0x25() // DEC H
{

}

void CPU::opcode0x26() // LD H,u8
{

}

void CPU::opcode0x27() // DAA
{

}

void CPU::opcode0x28() // JR Z,i8
{

}

void CPU::opcode0x29() // ADD HL,HL
{

}

void CPU::opcode0x2A() // LD A,(HL+)
{

}

void CPU::opcode0x2B() // DEC HL
{

}

void CPU::opcode0x2C() // INC L
{

}

void CPU::opcode0x2D() // DEC L
{

}

void CPU::opcode0x2E() // LD L,u8
{

}

void CPU::opcode0x2F() // CPL
{

}

//0x30

void CPU::opcode0x30() // JR NC,i8
{

}

void CPU::opcode0x31() // LD SP,u16
{
    u8 low  = m_MMU->read(m_Registers.PC++);
    u8 high = m_MMU->read(m_Registers.PC++);
    u16 val = static_cast<u16>(high) << 8 | static_cast<u16>(low);

    m_Registers.SP = val;
}

void CPU::opcode0x32() // LD (HL-),A
{
    m_MMU->write(m_Registers.HL--, m_Registers.A);

    LOG_WRITE(m_Registers.HL + 1, m_Registers.A);
}

void CPU::opcode0x33() // INC SP
{

}

void CPU::opcode0x34() // INC (HL)
{

}

void CPU::opcode0x35() // DEC (HL)
{

}

void CPU::opcode0x36() // LD (HL),u8
{

}

void CPU::opcode0x37() // SCF
{

}

void CPU::opcode0x38() // JR C,i8
{

}

void CPU::opcode0x39() // ADD HL,SP
{

}

void CPU::opcode0x3A() // LD A,(HL-)
{

}

void CPU::opcode0x3B() // DEC SP
{

}

void CPU::opcode0x3C() // INC A
{

}

void CPU::opcode0x3D() // DEC A
{

}

void CPU::opcode0x3E() // LD A,u8
{
    m_Registers.A = m_MMU->read(m_Registers.PC++);

    LOG_A_REG();
}

void CPU::opcode0x3F() // CCF
{

}

//0x40

void CPU::opcode0x40() // LD B,B
{

}

void CPU::opcode0x41() // LD B,C
{

}

void CPU::opcode0x42() // LD B,D
{

}

void CPU::opcode0x43() // LD B,E
{

}

void CPU::opcode0x44() // LD B,H
{

}

void CPU::opcode0x45() // LD B,L
{

}

void CPU::opcode0x46() // LD B,(HL)
{

}

void CPU::opcode0x47() // LD B,A
{

}

void CPU::opcode0x48() // LD C,B
{

}

void CPU::opcode0x49() // LD C,C
{

}

void CPU::opcode0x4A() // LD C,D
{

}

void CPU::opcode0x4B() // LD C,E
{

}

void CPU::opcode0x4C() // LD C,H
{

}

void CPU::opcode0x4D() // LD C,L
{

}

void CPU::opcode0x4E() // LD C,(HL)
{

}

void CPU::opcode0x4F() // LD C,A
{

}

//0x50

void CPU::opcode0x50() // LD D,B
{

}

void CPU::opcode0x51() // LD D,C
{

}

void CPU::opcode0x52() // LD D,D
{

}

void CPU::opcode0x53() // LD D,E
{

}

void CPU::opcode0x54() // LD D,H
{

}

void CPU::opcode0x55() // LD D,L
{

}

void CPU::opcode0x56() // LD D,(HL)
{

}

void CPU::opcode0x57() // LD D,A
{

}

void CPU::opcode0x58() // LD E,B
{

}

void CPU::opcode0x59() // LD E,C
{

}

void CPU::opcode0x5A() // LD E,D
{

}

void CPU::opcode0x5B() // LD E,E
{

}

void CPU::opcode0x5C() // LD E,H
{

}

void CPU::opcode0x5D() // LD E,L
{

}

void CPU::opcode0x5E() // LD E,(HL)
{

}

void CPU::opcode0x5F() // LD E,A
{

}

//0x60

void CPU::opcode0x60() // LD H,B
{

}

void CPU::opcode0x61() // LD H,C
{

}

void CPU::opcode0x62() // LD H,D
{

}

void CPU::opcode0x63() // LD H,E
{

}

void CPU::opcode0x64() // LD H,H
{

}

void CPU::opcode0x65() // LD H,L
{

}

void CPU::opcode0x66() // LD H,(HL)
{

}

void CPU::opcode0x67() // LD H,A
{

}

void CPU::opcode0x68() // LD L,B
{

}

void CPU::opcode0x69() // LD L,C
{

}

void CPU::opcode0x6A() // LD L,D
{

}

void CPU::opcode0x6B() // LD L,E
{

}

void CPU::opcode0x6C() // LD L,H
{

}

void CPU::opcode0x6D() // LD L,L
{

}

void CPU::opcode0x6E() // LD L,(HL)
{

}

void CPU::opcode0x6F() // LD L,A
{

}

//0x70

void CPU::opcode0x70() // LD (HL),B
{

}

void CPU::opcode0x71() // LD (HL),C
{

}

void CPU::opcode0x72() // LD (HL),D
{

}

void CPU::opcode0x73() // LD (HL),E
{

}

void CPU::opcode0x74() // LD (HL),H
{

}

void CPU::opcode0x75() // LD (HL),L
{

}

void CPU::opcode0x76() // HALT
{

}

void CPU::opcode0x77() // LD (HL),A
{

}

void CPU::opcode0x78() // LD A,B
{

}

void CPU::opcode0x79() // LD A,C
{

}

void CPU::opcode0x7A() // LD A,D
{

}

void CPU::opcode0x7B() // LD A,E
{

}

void CPU::opcode0x7C() // LD A,H
{

}

void CPU::opcode0x7D() // LD A,L
{

}

void CPU::opcode0x7E() // LD A,(HL)
{

}

void CPU::opcode0x7F() // LD A,A
{

}

//0x80

void CPU::opcode0x80() // ADD A,B
{

}

void CPU::opcode0x81() // ADD A,C
{

}

void CPU::opcode0x82() // ADD A,D
{

}

void CPU::opcode0x83() // ADD A,E
{

}

void CPU::opcode0x84() // ADD A,H
{

}

void CPU::opcode0x85() // ADD A,L
{

}

void CPU::opcode0x86() // ADD A,(HL)
{

}

void CPU::opcode0x87() // ADD A,A
{

}

void CPU::opcode0x88() // ADC A,B
{

}

void CPU::opcode0x89() // ADC A,C
{

}

void CPU::opcode0x8A() // ADC A,D
{

}

void CPU::opcode0x8B() // ADC A,E
{

}

void CPU::opcode0x8C() // ADC A,H
{

}

void CPU::opcode0x8D() // ADC A,L
{

}

void CPU::opcode0x8E() // ADC A,(HL)
{

}

void CPU::opcode0x8F() // ADC A,A
{

}

//0x90

void CPU::opcode0x90() // SUB A,B
{

}

void CPU::opcode0x91() // SUB A,C
{

}

void CPU::opcode0x92() // SUB A,D
{

}

void CPU::opcode0x93() // SUB A,E
{

}

void CPU::opcode0x94() // SUB A,H
{

}

void CPU::opcode0x95() // SUB A,L
{

}

void CPU::opcode0x96() // SUB A,(HL)
{

}

void CPU::opcode0x97() // SUB A,A
{

}

void CPU::opcode0x98() // SBC A,B
{

}

void CPU::opcode0x99() // SBC A,C
{

}

void CPU::opcode0x9A() // SBC A,D
{

}

void CPU::opcode0x9B() // SBC A,E
{

}

void CPU::opcode0x9C() // SBC A,H
{

}

void CPU::opcode0x9D() // SBC A,L
{

}

void CPU::opcode0x9E() // SBC A,(HL)
{

}

void CPU::opcode0x9F() // SBC A,A
{

}

//0xA0

void CPU::opcode0xA0() // AND A,B
{

}

void CPU::opcode0xA1() // AND A,C
{

}

void CPU::opcode0xA2() // AND A,D
{

}

void CPU::opcode0xA3() // AND A,E
{

}

void CPU::opcode0xA4() // AND A,H
{

}

void CPU::opcode0xA5() // AND A,L
{

}

void CPU::opcode0xA6() // AND A,(HL)
{

}

void CPU::opcode0xA7() // AND A,A
{

}

void CPU::opcode0xA8() // XOR A,B
{

}

void CPU::opcode0xA9() // XOR A,C
{

}

void CPU::opcode0xAA() // XOR A,D
{

}

void CPU::opcode0xAB() // XOR A,E
{

}

void CPU::opcode0xAC() // XOR A,H
{

}

void CPU::opcode0xAD() // XOR A,L
{

}

void CPU::opcode0xAE() // XOR A,(HL)
{

}

void CPU::opcode0xAF() // XOR A,A
{
    opcodeXOR(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

//0xB0

void CPU::opcode0xB0() // OR A,B
{



}
void CPU::opcode0xB1() // OR A,C
{

}

void CPU::opcode0xB2() // OR A,D
{

}

void CPU::opcode0xB3() // OR A,E
{

}

void CPU::opcode0xB4() // OR A,H
{

}

void CPU::opcode0xB5() // OR A,L
{

}

void CPU::opcode0xB6() // OR A,(HL)
{

}

void CPU::opcode0xB7() // OR A,A
{

}

void CPU::opcode0xB8() // CP A,B
{

}

void CPU::opcode0xB9() // CP A,C
{

}

void CPU::opcode0xBA() // CP A,D
{

}

void CPU::opcode0xBB() // CP A,E
{

}

void CPU::opcode0xBC() // CP A,H
{

}

void CPU::opcode0xBD() // CP A,L
{

}

void CPU::opcode0xBE() // CP A,(HL)
{

}

void CPU::opcode0xBF() // CP A,A
{

}

//0xC0

void CPU::opcode0xC0() // RET NZ
{

}

void CPU::opcode0xC1() // POP BC
{

}

void CPU::opcode0xC2() // JP NZ,u16
{

}

void CPU::opcode0xC3() // JP u16
{
    opcodeJP(true);
}

void CPU::opcode0xC4() // CALL NZ,u16
{

}

void CPU::opcode0xC5() // PUSH BC
{

}

void CPU::opcode0xC6() // ADD A,u8
{

}

void CPU::opcode0xC7() // RST 00h
{

}

void CPU::opcode0xC8() // RET Z
{

}

void CPU::opcode0xC9() // RET
{

}

void CPU::opcode0xCA() // JP Z,u16
{

}

void CPU::opcode0xCB() // PREFIX CB
{

}

void CPU::opcode0xCC() // CALL Z,u16
{

}

void CPU::opcode0xCD() // CALL u16
{

}

void CPU::opcode0xCE() // ADC A,u8
{

}

void CPU::opcode0xCF() // RST 08h
{

}

//0xD0

void CPU::opcode0xD0() // RET NC
{

}

void CPU::opcode0xD1() // POP DE
{

}

void CPU::opcode0xD2() // JP NC,u16
{

}

void CPU::opcode0xD3() // UNUSED
{

}

void CPU::opcode0xD4() // CALL NC,u16
{

}

void CPU::opcode0xD5() // PUSH DE
{

}

void CPU::opcode0xD6() // SUB A,u8
{

}

void CPU::opcode0xD7() // RST 10h
{

}

void CPU::opcode0xD8() // RET C
{

}

void CPU::opcode0xD9() // RETI
{

}

void CPU::opcode0xDA() // JP C,u16
{

}

void CPU::opcode0xDB() // UNUSED
{

}

void CPU::opcode0xDC() // CALL C,u16
{

}

void CPU::opcode0xDD() // UNUSED
{

}

void CPU::opcode0xDE() // SBC A,u8
{

}

void CPU::opcode0xDF() // RST 18h
{

}

//0xE0

void CPU::opcode0xE0() // LD (FF00+u8),A
{
    u8 offset = m_Registers.PC++;
    u16 addr = 0xFF00 | offset;
    m_MMU->write(addr, m_Registers.A);

    LOG_WRITE(addr, m_Registers.A);
}

void CPU::opcode0xE1() // POP HL
{

}

void CPU::opcode0xE2() // LD (FF00+C),A
{

}

void CPU::opcode0xE3() // UNUSED
{

}

void CPU::opcode0xE4() // UNUSED
{

}

void CPU::opcode0xE5() // PUSH HL
{

}

void CPU::opcode0xE6() // AND A,u8
{

}

void CPU::opcode0xE7() // RST 20h
{

}

void CPU::opcode0xE8() // ADD SP,i8
{

}

void CPU::opcode0xE9() // JP HL
{

}

void CPU::opcode0xEA() // LD (u16),A
{

}

void CPU::opcode0xEB() // UNUSED
{

}

void CPU::opcode0xEC() // UNUSED
{

}

void CPU::opcode0xED() // UNUSED
{

}

void CPU::opcode0xEE() // XOR A,u8
{

}

void CPU::opcode0xEF() // RST 28h
{

}

//0xF0

void CPU::opcode0xF0() // LD A,(FF00+u8)
{
    u8 offset = m_Registers.PC++;
    u16 addr = 0xFF00 | offset;
    m_Registers.A = m_MMU->read(addr);

    LOG_A_REG();
}

void CPU::opcode0xF1() // POP AF
{

}

void CPU::opcode0xF2() // LD A,(FF00+C)
{

}

void CPU::opcode0xF3() // DI
{
    m_IME = false;

    LOG_DI();
}

void CPU::opcode0xF4() // UNUSED
{

}

void CPU::opcode0xF5() // PUSH AF
{

}

void CPU::opcode0xF6() // OR A,u8
{

}

void CPU::opcode0xF7() // RST 30h
{

}

void CPU::opcode0xF8() // LD HL,SP+i8
{

}

void CPU::opcode0xF9() // LD SP,HL
{

}

void CPU::opcode0xFA() // LD A,(u16)
{

}

void CPU::opcode0xFB() // EI
{
    m_IME = true;

    LOG_EI();
}

void CPU::opcode0xFC() // UNUSED
{

}

void CPU::opcode0xFD() // UNUSED
{

}

void CPU::opcode0xFE() // CP A,u8
{
    opcodeCP(m_Registers.PC++);

    LOG_A_REG();
    LOG_FLAGS();
}

void CPU::opcode0xFF() // RST 38h
{

}

//--------------------------------------CB Opcodes--------------------------------------//

//0x00

void CPU::opcodeCB0x00() // RLC B
{
	
}

void CPU::opcodeCB0x01() // RLC C
{
	
}

void CPU::opcodeCB0x02() // RLC D
{
	
}

void CPU::opcodeCB0x03() // RLC E
{
	
}

void CPU::opcodeCB0x04() // RLC H
{
	
}

void CPU::opcodeCB0x05() // RLC L
{
	
}

void CPU::opcodeCB0x06() // RLC (HL)
{
	
}

void CPU::opcodeCB0x07() // RLC A
{
	
}

void CPU::opcodeCB0x08() // RRC B
{
	
}

void CPU::opcodeCB0x09() // RRC C
{
	
}

void CPU::opcodeCB0x0A() // RRC D
{
	
}

void CPU::opcodeCB0x0B() // RRC E
{
	
}

void CPU::opcodeCB0x0C() // RRC H
{
	
}

void CPU::opcodeCB0x0D() // RRC L
{
	
}

void CPU::opcodeCB0x0E() // RRC (HL)
{
	
}

void CPU::opcodeCB0x0F() // RRC A
{
	
}

//0x10

void CPU::opcodeCB0x10() // RL B
{
	
}

void CPU::opcodeCB0x11() // RL C
{
	
}

void CPU::opcodeCB0x12() // RL D
{
	
}

void CPU::opcodeCB0x13() // RL E
{
	
}

void CPU::opcodeCB0x14() // RL H
{
	
}

void CPU::opcodeCB0x15() // RL L
{
	
}

void CPU::opcodeCB0x16() // RL (HL)
{
	
}

void CPU::opcodeCB0x17() // RL A
{
	
}

void CPU::opcodeCB0x18() // RR B
{
	
}

void CPU::opcodeCB0x19() // RR C
{
	
}

void CPU::opcodeCB0x1A() // RR D
{
	
}

void CPU::opcodeCB0x1B() // RR E
{
	
}

void CPU::opcodeCB0x1C() // RR H
{
	
}

void CPU::opcodeCB0x1D() // RR L
{
	
}

void CPU::opcodeCB0x1E() // RR (HL)
{
	
}

void CPU::opcodeCB0x1F() // RR A
{
	
}

//0x20

void CPU::opcodeCB0x20() // SLA B
{
	
}

void CPU::opcodeCB0x21() // SLA C
{
	
}

void CPU::opcodeCB0x22() // SLA D
{
	
}

void CPU::opcodeCB0x23() // SLA E
{
	
}

void CPU::opcodeCB0x24() // SLA H
{
	
}

void CPU::opcodeCB0x25() // SLA L
{
	
}

void CPU::opcodeCB0x26() // SLA (HL)
{
	
}

void CPU::opcodeCB0x27() // SLA A
{
	
}

void CPU::opcodeCB0x28() // SRA B
{
	
}

void CPU::opcodeCB0x29() // SRA C
{
	
}

void CPU::opcodeCB0x2A() // SRA D
{
	
}

void CPU::opcodeCB0x2B() // SRA E
{
	
}

void CPU::opcodeCB0x2C() // SRA H
{
	
}

void CPU::opcodeCB0x2D() // SRA L
{
	
}

void CPU::opcodeCB0x2E() // SRA (HL)
{
	
}

void CPU::opcodeCB0x2F() // SRA A
{
	
}

//0x30

void CPU::opcodeCB0x30() // SWAP B
{
	
}

void CPU::opcodeCB0x31() // SWAP C
{
	
}

void CPU::opcodeCB0x32() // SWAP D
{
	
}

void CPU::opcodeCB0x33() // SWAP E
{
	
}

void CPU::opcodeCB0x34() // SWAP H
{
	
}

void CPU::opcodeCB0x35() // SWAP L
{
	
}

void CPU::opcodeCB0x36() // SWAP (HL)
{
	
}

void CPU::opcodeCB0x37() // SWAP A
{
	
}

void CPU::opcodeCB0x38() // SRL B
{
	
}

void CPU::opcodeCB0x39() // SRL C
{
	
}

void CPU::opcodeCB0x3A() // SRL D
{
	
}

void CPU::opcodeCB0x3B() // SRL E
{
	
}

void CPU::opcodeCB0x3C() // SRL H
{
	
}

void CPU::opcodeCB0x3D() // SRL L
{
	
}

void CPU::opcodeCB0x3E() // SRL (HL)
{
	
}

void CPU::opcodeCB0x3F() // SRL A
{
	
}

//0x40

void CPU::opcodeCB0x40() // BIT 0,B
{
	
}

void CPU::opcodeCB0x41() // BIT 0,C
{
	
}

void CPU::opcodeCB0x42() // BIT 0,D
{
	
}

void CPU::opcodeCB0x43() // BIT 0,E
{
	
}

void CPU::opcodeCB0x44() // BIT 0,H
{
	
}

void CPU::opcodeCB0x45() // BIT 0,L
{
	
}

void CPU::opcodeCB0x46() // BIT 0,(HL)
{
	
}

void CPU::opcodeCB0x47() // BIT 0,A
{
	
}

void CPU::opcodeCB0x48() // BIT 1,B
{
	
}

void CPU::opcodeCB0x49() // BIT 1,C
{
	
}

void CPU::opcodeCB0x4A() // BIT 1,D
{
	
}

void CPU::opcodeCB0x4B() // BIT 1,E
{
	
}

void CPU::opcodeCB0x4C() // BIT 1,H
{
	
}

void CPU::opcodeCB0x4D() // BIT 1,L
{
	
}

void CPU::opcodeCB0x4E() // BIT 1,(HL)
{
	
}

void CPU::opcodeCB0x4F() // BIT 1,A
{
	
}

//0x50

void CPU::opcodeCB0x50() // BIT 2,B
{
	
}

void CPU::opcodeCB0x51() // BIT 2,C
{
	
}

void CPU::opcodeCB0x52() // BIT 2,D
{
	
}

void CPU::opcodeCB0x53() // BIT 2,E
{
	
}

void CPU::opcodeCB0x54() // BIT 2,H
{
	
}

void CPU::opcodeCB0x55() // BIT 2,L
{
	
}

void CPU::opcodeCB0x56() // BIT 2,(HL)
{
	
}

void CPU::opcodeCB0x57() // BIT 2,A
{
	
}

void CPU::opcodeCB0x58() // BIT 3,B
{
	
}

void CPU::opcodeCB0x59() // BIT 3,C
{
	
}

void CPU::opcodeCB0x5A() // BIT 3,D
{
	
}

void CPU::opcodeCB0x5B() // BIT 3,E
{
	
}

void CPU::opcodeCB0x5C() // BIT 3,H
{
	
}

void CPU::opcodeCB0x5D() // BIT 3,L
{
	
}

void CPU::opcodeCB0x5E() // BIT 3,(HL)
{
	
}

void CPU::opcodeCB0x5F() // BIT 3,A
{
	
}

//0x60

void CPU::opcodeCB0x60() // BIT 4,B
{
	
}

void CPU::opcodeCB0x61() // BIT 4,C
{
	
}

void CPU::opcodeCB0x62() // BIT 4,D
{
	
}

void CPU::opcodeCB0x63() // BIT 4,E
{
	
}

void CPU::opcodeCB0x64() // BIT 4,H
{
	
}

void CPU::opcodeCB0x65() // BIT 4,L
{
	
}

void CPU::opcodeCB0x66() // BIT 4,(HL)
{
	
}

void CPU::opcodeCB0x67() // BIT 4,A
{
	
}

void CPU::opcodeCB0x68() // BIT 5,B
{
	
}

void CPU::opcodeCB0x69() // BIT 5,C
{
	
}

void CPU::opcodeCB0x6A() // BIT 5,D
{
	
}

void CPU::opcodeCB0x6B() // BIT 5,E
{
	
}

void CPU::opcodeCB0x6C() // BIT 5,H
{
	
}

void CPU::opcodeCB0x6D() // BIT 5,L
{
	
}

void CPU::opcodeCB0x6E() // BIT 5,(HL)
{
	
}

void CPU::opcodeCB0x6F() // BIT 5,A
{
	
}

//0x70

void CPU::opcodeCB0x70() // BIT 6,B
{
	
}

void CPU::opcodeCB0x71() // BIT 6,C
{
	
}

void CPU::opcodeCB0x72() // BIT 6,D
{
	
}

void CPU::opcodeCB0x73() // BIT 6,E
{
	
}

void CPU::opcodeCB0x74() // BIT 6,H
{
	
}

void CPU::opcodeCB0x75() // BIT 6,L
{
	
}

void CPU::opcodeCB0x76() // BIT 6,(HL)
{
	
}

void CPU::opcodeCB0x77() // BIT 6,A
{
	
}

void CPU::opcodeCB0x78() // BIT 7,B
{
	
}

void CPU::opcodeCB0x79() // BIT 7,C
{
	
}

void CPU::opcodeCB0x7A() // BIT 7,D
{
	
}

void CPU::opcodeCB0x7B() // BIT 7,E
{
	
}

void CPU::opcodeCB0x7C() // BIT 7,H
{
	
}

void CPU::opcodeCB0x7D() // BIT 7,L
{
	
}

void CPU::opcodeCB0x7E() // BIT 7,(HL)
{
	
}

void CPU::opcodeCB0x7F() // BIT 7,A
{
	
}

//0x80

void CPU::opcodeCB0x80() // RES 0,B
{
	
}

void CPU::opcodeCB0x81() // RES 0,C
{
	
}

void CPU::opcodeCB0x82() // RES 0,D
{
	
}

void CPU::opcodeCB0x83() // RES 0,E
{
	
}

void CPU::opcodeCB0x84() // RES 0,H
{
	
}

void CPU::opcodeCB0x85() // RES 0,L
{
	
}

void CPU::opcodeCB0x86() // RES 0,(HL)
{
	
}

void CPU::opcodeCB0x87() // RES 0,A
{
	
}

void CPU::opcodeCB0x88() // RES 1,B
{
	
}

void CPU::opcodeCB0x89() // RES 1,C
{
	
}

void CPU::opcodeCB0x8A() // RES 1,D
{
	
}

void CPU::opcodeCB0x8B() // RES 1,E
{
	
}

void CPU::opcodeCB0x8C() // RES 1,H
{
	
}

void CPU::opcodeCB0x8D() // RES 1,L
{
	
}

void CPU::opcodeCB0x8E() // RES 1,(HL)
{
	
}

void CPU::opcodeCB0x8F() // RES 1,A
{
	
}

//0x90

void CPU::opcodeCB0x90() // RES 2,B
{
	
}

void CPU::opcodeCB0x91() // RES 2,C
{
	
}

void CPU::opcodeCB0x92() // RES 2,D
{
	
}

void CPU::opcodeCB0x93() // RES 2,E
{
	
}

void CPU::opcodeCB0x94() // RES 2,H
{
	
}

void CPU::opcodeCB0x95() // RES 2,L
{
	
}

void CPU::opcodeCB0x96() // RES 2,(HL)
{
	
}

void CPU::opcodeCB0x97() // RES 2,A
{
	
}

void CPU::opcodeCB0x98() // RES 3,B
{
	
}

void CPU::opcodeCB0x99() // RES 3,C
{
	
}

void CPU::opcodeCB0x9A() // RES 3,D
{
	
}

void CPU::opcodeCB0x9B() // RES 3,E
{
	
}

void CPU::opcodeCB0x9C() // RES 3,H
{
	
}

void CPU::opcodeCB0x9D() // RES 3,L
{
	
}

void CPU::opcodeCB0x9E() // RES 3,(HL)
{
	
}

void CPU::opcodeCB0x9F() // RES 3,A
{
	
}

//0xA0

void CPU::opcodeCB0xA0() // RES 4,B
{
	
}

void CPU::opcodeCB0xA1() // RES 4,C
{
	
}

void CPU::opcodeCB0xA2() // RES 4,D
{
	
}

void CPU::opcodeCB0xA3() // RES 4,E
{
	
}

void CPU::opcodeCB0xA4() // RES 4,H
{
	
}

void CPU::opcodeCB0xA5() // RES 4,L
{
	
}

void CPU::opcodeCB0xA6() // RES 4,(HL)
{
	
}

void CPU::opcodeCB0xA7() // RES 4,A
{
	
}

void CPU::opcodeCB0xA8() // RES 5,B
{
	
}

void CPU::opcodeCB0xA9() // RES 5,C
{
	
}

void CPU::opcodeCB0xAA() // RES 5,D
{
	
}

void CPU::opcodeCB0xAB() // RES 5,E
{
	
}

void CPU::opcodeCB0xAC() // RES 5,H
{
	
}

void CPU::opcodeCB0xAD() // RES 5,L
{
	
}

void CPU::opcodeCB0xAE() // RES 5,(HL)
{
	
}

void CPU::opcodeCB0xAF() // RES 5,A
{
	
}

//0xB0

void CPU::opcodeCB0xB0() // RES 6,B
{
	
}

void CPU::opcodeCB0xB1() // RES 6,C
{
	
}

void CPU::opcodeCB0xB2() // RES 6,D
{
	
}

void CPU::opcodeCB0xB3() // RES 6,E
{
	
}

void CPU::opcodeCB0xB4() // RES 6,H
{
	
}

void CPU::opcodeCB0xB5() // RES 6,L
{
	
}

void CPU::opcodeCB0xB6() // RES 6,(HL)
{
	
}

void CPU::opcodeCB0xB7() // RES 6,A
{
	
}

void CPU::opcodeCB0xB8() // RES 7,B
{
	
}

void CPU::opcodeCB0xB9() // RES 7,C
{
	
}

void CPU::opcodeCB0xBA() // RES 7,D
{
	
}

void CPU::opcodeCB0xBB() // RES 7,E
{
	
}

void CPU::opcodeCB0xBC() // RES 7,H
{
	
}

void CPU::opcodeCB0xBD() // RES 7,L
{
	
}

void CPU::opcodeCB0xBE() // RES 7,(HL)
{
	
}

void CPU::opcodeCB0xBF() // RES 7,A
{
	
}

//0xC0

void CPU::opcodeCB0xC0() // SET 0,B
{
	
}

void CPU::opcodeCB0xC1() // SET 0,C
{
	
}

void CPU::opcodeCB0xC2() // SET 0,D
{
	
}

void CPU::opcodeCB0xC3() // SET 0,E
{
	
}

void CPU::opcodeCB0xC4() // SET 0,H
{
	
}

void CPU::opcodeCB0xC5() // SET 0,L
{
	
}

void CPU::opcodeCB0xC6() // SET 0,(HL)
{
	
}

void CPU::opcodeCB0xC7() // SET 0,A
{
	
}

void CPU::opcodeCB0xC8() // SET 1,B
{
	
}

void CPU::opcodeCB0xC9() // SET 1,C
{
	
}

void CPU::opcodeCB0xCA() // SET 1,D
{
	
}

void CPU::opcodeCB0xCB() // SET 1,E
{
	
}

void CPU::opcodeCB0xCC() // SET 1,H
{
	
}

void CPU::opcodeCB0xCD() // SET 1,L
{
	
}

void CPU::opcodeCB0xCE() // SET 1,(HL)
{
	
}

void CPU::opcodeCB0xCF() // SET 1,A
{
	
}

//0xD0

void CPU::opcodeCB0xD0() // SET 2,B
{
	
}

void CPU::opcodeCB0xD1() // SET 2,C
{
	
}

void CPU::opcodeCB0xD2() // SET 2,D
{
	
}

void CPU::opcodeCB0xD3() // SET 2,E
{
	
}

void CPU::opcodeCB0xD4() // SET 2,H
{
	
}

void CPU::opcodeCB0xD5() // SET 2,L
{
	
}

void CPU::opcodeCB0xD6() // SET 2,(HL)
{
	
}

void CPU::opcodeCB0xD7() // SET 2,A
{
	
}

void CPU::opcodeCB0xD8() // SET 3,B
{
	
}

void CPU::opcodeCB0xD9() // SET 3,C
{
	
}

void CPU::opcodeCB0xDA() // SET 3,D
{
	
}

void CPU::opcodeCB0xDB() // SET 3,E
{
	
}

void CPU::opcodeCB0xDC() // SET 3,H
{
	
}

void CPU::opcodeCB0xDD() // SET 3,L
{
	
}

void CPU::opcodeCB0xDE() // SET 3,(HL)
{
	
}

void CPU::opcodeCB0xDF() // SET 3,A
{
	
}

//0xE0

void CPU::opcodeCB0xE0() // SET 4,B
{
	
}

void CPU::opcodeCB0xE1() // SET 4,C
{
	
}

void CPU::opcodeCB0xE2() // SET 4,D
{
	
}

void CPU::opcodeCB0xE3() // SET 4,E
{
	
}

void CPU::opcodeCB0xE4() // SET 4,H
{
	
}

void CPU::opcodeCB0xE5() // SET 4,L
{
	
}

void CPU::opcodeCB0xE6() // SET 4,(HL)
{
	
}

void CPU::opcodeCB0xE7() // SET 4,A
{
	
}

void CPU::opcodeCB0xE8() // SET 5,B
{
	
}

void CPU::opcodeCB0xE9() // SET 5,C
{
	
}

void CPU::opcodeCB0xEA() // SET 5,D
{
	
}

void CPU::opcodeCB0xEB() // SET 5,E
{
	
}

void CPU::opcodeCB0xEC() // SET 5,H
{
	
}

void CPU::opcodeCB0xED() // SET 5,L
{
	
}

void CPU::opcodeCB0xEE() // SET 5,(HL)
{
	
}

void CPU::opcodeCB0xEF() // SET 5,A
{
	
}

//0xF0

void CPU::opcodeCB0xF0() // SET 6,B
{
	
}

void CPU::opcodeCB0xF1() // SET 6,C
{
	
}

void CPU::opcodeCB0xF2() // SET 6,D
{
	
}

void CPU::opcodeCB0xF3() // SET 6,E
{
	
}

void CPU::opcodeCB0xF4() // SET 6,H
{
	
}

void CPU::opcodeCB0xF5() // SET 6,L
{
	
}

void CPU::opcodeCB0xF6() // SET 6,(HL)
{
	
}

void CPU::opcodeCB0xF7() // SET 6,A
{
	
}

void CPU::opcodeCB0xF8() // SET 7,B
{
	
}

void CPU::opcodeCB0xF9() // SET 7,C
{
	
}

void CPU::opcodeCB0xFA() // SET 7,D
{
	
}

void CPU::opcodeCB0xFB() // SET 7,E
{
	
}

void CPU::opcodeCB0xFC() // SET 7,H
{
	
}

void CPU::opcodeCB0xFD() // SET 7,L
{
	
}

void CPU::opcodeCB0xFE() // SET 7,(HL)
{
	
}

void CPU::opcodeCB0xFF() // SET 7,A
{
	
}
