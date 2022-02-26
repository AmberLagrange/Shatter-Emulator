#include "instruction.h"
#include "cpu.h"
#include "gameboy.h"

#include "logging/opcode_log.h"

//--------------------------------------CB Opcode Helpers--------------------------------------//

void CPU::opcodeRLC(u8& reg)
{
    clearAllFlags();

    u8 carry = GET_BIT(reg, 7);

    reg = (reg << 1) | carry;

    if(carry) setFlag(Flags::Register::Carry);
    setZeroFromVal(reg);
}

void CPU::opcodeRRC(u8& reg)
{
    clearAllFlags();

    u8 carry = GET_BIT(reg, 0);

    reg = (carry << 7) | (reg >> 1);

    if(carry) setFlag(Flags::Register::Carry);
    setZeroFromVal(reg);
}

void CPU::opcodeRL(u8& reg)
{
    u8 carry = isFlagSet(Flags::Register::Carry);

    clearAllFlags();

    if(GET_BIT(reg, 7)) setFlag(Flags::Register::Carry);

    reg = (reg << 1) | carry;

    setZeroFromVal(reg);
}

void CPU::opcodeRR(u8& reg)
{
    u8 carry = isFlagSet(Flags::Register::Carry);

    clearAllFlags();

    if(GET_BIT(reg, 0)) setFlag(Flags::Register::Carry);

    reg = (carry << 7) | (reg >> 1);

    setZeroFromVal(reg);
}

void CPU::opcodeSLA(u8& reg)
{
    clearAllFlags();

    if(GET_BIT(reg, 7)) setFlag(Flags::Register::Carry);

    reg <<= 1;

    setZeroFromVal(reg);
}

void CPU::opcodeSRA(u8& reg)
{
    clearAllFlags();

    if(GET_BIT(reg, 0)) setFlag(Flags::Register::Carry);
    
    reg >>= 1;

    if(GET_BIT(reg, 6)) reg |= 0x80;

    setZeroFromVal(reg);
}

void CPU::opcodeSWAP(u8& reg)
{
    clearAllFlags();

    reg = (reg << 4) | (reg >> 4);

    setZeroFromVal(reg);
}

void CPU::opcodeSRL(u8& reg)
{
    u8 carry = GET_BIT(reg, 0);

    clearAllFlags();

    reg >>= 1;

    if(carry) setFlag(Flags::Register::Carry);
    setZeroFromVal(reg);
}

void CPU::opcodeBIT(const u8& bit, u8& reg)
{
    if(GET_BIT(reg, bit)) clearFlag(Flags::Register::Zero);
    else
        setFlag(Flags::Register::Zero);

    clearFlag(Flags::Register::Negative);
    setFlag(Flags::Register::HalfCarry);
}

void CPU::opcodeRES(const u8& bit, u8& reg)
{
    CLEAR_BIT(reg, bit);
}

void CPU::opcodeSET(const u8& bit, u8& reg)
{
    SET_BIT(reg, bit);
}

//HL variants

void CPU::opcodeBIT_HL(const u8& bit)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    if(GET_BIT(val, bit)) clearFlag(Flags::Register::Zero);
    else
        setFlag(Flags::Register::Zero);

    clearFlag(Flags::Register::Negative);
    setFlag(Flags::Register::HalfCarry);
}

void CPU::opcodeRES_HL(const u8& bit)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    val &= ~(0x01 << bit);

    m_Gameboy.write(m_Registers.HL, val);
}

void CPU::opcodeSET_HL([[maybe_unused]] const u8& bit)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    val |= (0x01 << bit);

    m_Gameboy.write(m_Registers.HL, val);
}

//--------------------------------------CB Opcodes--------------------------------------//

//0x00

void CPU::opcodeCB0x00() // RLC B
{
    opcodeRLC(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x01() // RLC C
{
    opcodeRLC(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x02() // RLC D
{
    opcodeRLC(m_Registers.D);

    LOG_D_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x03() // RLC E
{
    opcodeRLC(m_Registers.E);

    LOG_E_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x04() // RLC H
{
    opcodeRLC(m_Registers.H);

    LOG_H_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x05() // RLC L
{
    opcodeRLC(m_Registers.L);

    LOG_L_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x06() // RLC (HL)
{
    u8 val = m_Gameboy.read(m_Registers.HL);
    
    clearAllFlags();

    u8 carry = GET_BIT(val, 7);

    val = (val << 1) | carry;

    if(carry) setFlag(Flags::Register::Carry);
    setZeroFromVal(val);

    m_Gameboy.write(m_Registers.HL, val);

    LOG_WRITE(m_Registers.HL);
    LOG_FLAGS();
}

void CPU::opcodeCB0x07() // RLC A
{
    opcodeRLC(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x08() // RRC B
{
    opcodeRRC(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x09() // RRC C
{
    opcodeRRC(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x0A() // RRC D
{
    opcodeRRC(m_Registers.D);

    LOG_D_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x0B() // RRC E
{
    opcodeRRC(m_Registers.E);

    LOG_E_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x0C() // RRC H
{
    opcodeRRC(m_Registers.H);

    LOG_H_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x0D() // RRC L
{
    opcodeRRC(m_Registers.L);

    LOG_L_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x0E() // RRC (HL)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    clearAllFlags();

    u8 carry = GET_BIT(val, 0);

    val = (carry << 7) | (val >> 1);

    if(carry) setFlag(Flags::Register::Carry);
    setZeroFromVal(val);

    m_Gameboy.write(m_Registers.HL, val);

    LOG_WRITE(m_Registers.HL);
    LOG_FLAGS();
}

void CPU::opcodeCB0x0F() // RRC A
{
    opcodeRRC(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

//0x10

void CPU::opcodeCB0x10() // RL B
{
    opcodeRL(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x11() // RL C
{
    opcodeRL(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x12() // RL D
{
    opcodeRL(m_Registers.D);

    LOG_D_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x13() // RL E
{
    opcodeRL(m_Registers.E);

    LOG_E_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x14() // RL H
{
    opcodeRL(m_Registers.H);

    LOG_H_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x15() // RL L
{
    opcodeRL(m_Registers.L);

    LOG_L_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x16() // RL (HL)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    u8 carry = isFlagSet(Flags::Register::Carry);

    clearAllFlags();

    if(GET_BIT(val, 7)) setFlag(Flags::Register::Carry);

    val = (val << 1) | carry;

    setZeroFromVal(val);

    m_Gameboy.write(m_Registers.HL, val);

    LOG_WRITE(m_Registers.HL);
    LOG_FLAGS();
}

void CPU::opcodeCB0x17() // RL A
{
    opcodeRL(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x18() // RR B
{
    opcodeRR(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x19() // RR C
{
    opcodeRR(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x1A() // RR D
{
    opcodeRR(m_Registers.D);

    LOG_D_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x1B() // RR E
{
    opcodeRR(m_Registers.E);

    LOG_E_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x1C() // RR H
{
    opcodeRR(m_Registers.H);

    LOG_H_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x1D() // RR L
{
    opcodeRR(m_Registers.L);

    LOG_L_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x1E() // RR (HL)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    u8 carry = isFlagSet(Flags::Register::Carry);

    clearAllFlags();

    if(GET_BIT(val, 0)) setFlag(Flags::Register::Carry);

    val = (carry << 7) | (val >> 1);

    setZeroFromVal(val);

    m_Gameboy.write(m_Registers.HL, val);

    LOG_WRITE(m_Registers.HL);
    LOG_FLAGS();
}

void CPU::opcodeCB0x1F() // RR A
{
    opcodeRR(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

//0x20

void CPU::opcodeCB0x20() // SLA B
{
    opcodeSLA(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x21() // SLA C
{
    opcodeSLA(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x22() // SLA D
{
    opcodeSLA(m_Registers.D);

    LOG_D_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x23() // SLA E
{
    opcodeSLA(m_Registers.E);

    LOG_E_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x24() // SLA H
{
    opcodeSLA(m_Registers.H);

    LOG_H_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x25() // SLA L
{
    opcodeSLA(m_Registers.L);

    LOG_L_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x26() // SLA (HL)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    clearAllFlags();

    if(GET_BIT(val, 7)) setFlag(Flags::Register::Carry);

    val <<= 1;

    setZeroFromVal(val);

    m_Gameboy.write(m_Registers.HL, val);

    LOG_WRITE(m_Registers.HL);
    LOG_FLAGS();
}

void CPU::opcodeCB0x27() // SLA A
{
    opcodeSLA(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x28() // SRA B
{
    opcodeSRA(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x29() // SRA C
{
    opcodeSRA(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x2A() // SRA D
{
    opcodeSRA(m_Registers.D);

    LOG_D_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x2B() // SRA E
{
    opcodeSRA(m_Registers.E);

    LOG_E_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x2C() // SRA H
{
    opcodeSRA(m_Registers.H);

    LOG_H_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x2D() // SRA L
{
    opcodeSRA(m_Registers.L);

    LOG_L_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x2E() // SRA (HL)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    clearAllFlags();

    if(GET_BIT(val, 0)) setFlag(Flags::Register::Carry);
    val >>= 1;

    if(GET_BIT(val, 6)) val |= 0x80;

    setZeroFromVal(val);

    m_Gameboy.write(m_Registers.HL, val);

    LOG_WRITE(m_Registers.HL);
    LOG_FLAGS();
}

void CPU::opcodeCB0x2F() // SRA A
{
    opcodeSRA(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

//0x30

void CPU::opcodeCB0x30() // SWAP B
{
    opcodeSWAP(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x31() // SWAP C
{
    opcodeSWAP(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x32() // SWAP D
{
    opcodeSWAP(m_Registers.D);

    LOG_D_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x33() // SWAP E
{
    opcodeSWAP(m_Registers.E);

    LOG_E_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x34() // SWAP H
{
    opcodeSWAP(m_Registers.H);

    LOG_H_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x35() // SWAP L
{
    opcodeSWAP(m_Registers.L);

    LOG_L_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x36() // SWAP (HL)
{
    clearAllFlags();

    u8 val = m_Gameboy.read(m_Registers.HL);

    u8 low  = val & 0x0F;
    u8 high = val & 0xF0;

    m_Gameboy.write(m_Registers.HL, (low << 4) | (high >> 4));

    setZeroFromVal((low << 4) | (high >> 4));

    LOG_WRITE(m_Registers.HL);
    LOG_FLAGS();
}

void CPU::opcodeCB0x37() // SWAP A
{
    opcodeSWAP(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x38() // SRL B
{
    opcodeSRL(m_Registers.B);

    LOG_B_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x39() // SRL C
{
    opcodeSRL(m_Registers.C);

    LOG_C_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x3A() // SRL D
{
    opcodeSRL(m_Registers.D);

    LOG_D_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x3B() // SRL E
{
    opcodeSRL(m_Registers.E);

    LOG_E_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x3C() // SRL H
{
    opcodeSRL(m_Registers.H);

    LOG_H_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x3D() // SRL L
{
    opcodeSRL(m_Registers.L);

    LOG_L_REG();
    LOG_FLAGS();
}

void CPU::opcodeCB0x3E() // SRL (HL)
{
    u8 val = m_Gameboy.read(m_Registers.HL);

    u8 carry = GET_BIT(val, 0);

    clearAllFlags();

    val >>= 1;

    if(carry) setFlag(Flags::Register::Carry);
    setZeroFromVal(val);

    m_Gameboy.write(m_Registers.HL, val);

    LOG_WRITE(m_Registers.HL);
    LOG_FLAGS();
}

void CPU::opcodeCB0x3F() // SRL A
{
    opcodeSRL(m_Registers.A);

    LOG_A_REG();
    LOG_FLAGS();
}

//0x40

void CPU::opcodeCB0x40() // BIT 0,B
{
    opcodeBIT(0, m_Registers.B);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x41() // BIT 0,C
{
    opcodeBIT(0, m_Registers.C);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x42() // BIT 0,D
{
    opcodeBIT(0, m_Registers.D);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x43() // BIT 0,E
{
    opcodeBIT(0, m_Registers.E);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x44() // BIT 0,H
{
    opcodeBIT(0, m_Registers.H);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x45() // BIT 0,L
{
    opcodeBIT(0, m_Registers.L);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x46() // BIT 0,(HL)
{
    opcodeBIT_HL(0);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x47() // BIT 0,A
{
    opcodeBIT(0, m_Registers.A);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x48() // BIT 1,B
{
    opcodeBIT(1, m_Registers.B);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x49() // BIT 1,C
{
    opcodeBIT(1, m_Registers.C);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x4A() // BIT 1,D
{
    opcodeBIT(1, m_Registers.D);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x4B() // BIT 1,E
{
    opcodeBIT(1, m_Registers.E);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x4C() // BIT 1,H
{
    opcodeBIT(1, m_Registers.H);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x4D() // BIT 1,L
{
    opcodeBIT(1, m_Registers.L);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x4E() // BIT 1,(HL)
{
    opcodeBIT_HL(1);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x4F() // BIT 1,A
{
    opcodeBIT(1, m_Registers.A);
    
    LOG_FLAGS();
}

//0x50

void CPU::opcodeCB0x50() // BIT 2,B
{
    opcodeBIT(2, m_Registers.B);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x51() // BIT 2,C
{
    opcodeBIT(2, m_Registers.C);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x52() // BIT 2,D
{
    opcodeBIT(2, m_Registers.D);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x53() // BIT 2,E
{
    opcodeBIT(2, m_Registers.E);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x54() // BIT 2,H
{
    opcodeBIT(2, m_Registers.H);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x55() // BIT 2,L
{
    opcodeBIT(2, m_Registers.L);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x56() // BIT 2,(HL)
{
    opcodeBIT_HL(2);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x57() // BIT 2,A
{
    opcodeBIT(2, m_Registers.A);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x58() // BIT 3,B
{
    opcodeBIT(3, m_Registers.B);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x59() // BIT 3,C
{
    opcodeBIT(3, m_Registers.C);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x5A() // BIT 3,D
{
    opcodeBIT(3, m_Registers.D);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x5B() // BIT 3,E
{
    opcodeBIT(3, m_Registers.E);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x5C() // BIT 3,H
{
    opcodeBIT(3, m_Registers.H);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x5D() // BIT 3,L
{
    opcodeBIT(3, m_Registers.L);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x5E() // BIT 3,(HL)
{
    opcodeBIT_HL(3);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x5F() // BIT 3,A
{
    opcodeBIT(3, m_Registers.A);
    
    LOG_FLAGS();
}

//0x60

void CPU::opcodeCB0x60() // BIT 4,B
{
    opcodeBIT(4, m_Registers.B);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x61() // BIT 4,C
{
    opcodeBIT(4, m_Registers.C);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x62() // BIT 4,D
{
    opcodeBIT(4, m_Registers.D);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x63() // BIT 4,E
{
    opcodeBIT(4, m_Registers.E);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x64() // BIT 4,H
{
    opcodeBIT(4, m_Registers.H);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x65() // BIT 4,L
{
    opcodeBIT(4, m_Registers.L);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x66() // BIT 4,(HL)
{
    opcodeBIT_HL(4);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x67() // BIT 4,A
{
    opcodeBIT(4, m_Registers.A);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x68() // BIT 5,B
{
    opcodeBIT(5, m_Registers.B);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x69() // BIT 5,C
{
    opcodeBIT(5, m_Registers.C);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x6A() // BIT 5,D
{
    opcodeBIT(5, m_Registers.D);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x6B() // BIT 5,E
{
    opcodeBIT(5, m_Registers.E);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x6C() // BIT 5,H
{
    opcodeBIT(5, m_Registers.H);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x6D() // BIT 5,L
{
    opcodeBIT(5, m_Registers.L);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x6E() // BIT 5,(HL)
{
    opcodeBIT_HL(5);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x6F() // BIT 5,A
{
    opcodeBIT(5, m_Registers.A);
    
    LOG_FLAGS();
}

//0x70

void CPU::opcodeCB0x70() // BIT 6,B
{
    opcodeBIT(6, m_Registers.B);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x71() // BIT 6,C
{
    opcodeBIT(6, m_Registers.C);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x72() // BIT 6,D
{
    opcodeBIT(6, m_Registers.D);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x73() // BIT 6,E
{
    opcodeBIT(6, m_Registers.E);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x74() // BIT 6,H
{
    opcodeBIT(6, m_Registers.H);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x75() // BIT 6,L
{
    opcodeBIT(6, m_Registers.L);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x76() // BIT 6,(HL)
{
    opcodeBIT_HL(6);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x77() // BIT 6,A
{
    opcodeBIT(6, m_Registers.A);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x78() // BIT 7,B
{
    opcodeBIT(7, m_Registers.B);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x79() // BIT 7,C
{
    opcodeBIT(7, m_Registers.C);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x7A() // BIT 7,D
{
    opcodeBIT(7, m_Registers.D);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x7B() // BIT 7,E
{
    opcodeBIT(7, m_Registers.E);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x7C() // BIT 7,H
{
    opcodeBIT(7, m_Registers.H);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x7D() // BIT 7,L
{
    opcodeBIT(7, m_Registers.L);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x7E() // BIT 7,(HL)
{
    opcodeBIT_HL(7);
    
    LOG_FLAGS();
}

void CPU::opcodeCB0x7F() // BIT 7,A
{
    opcodeBIT(7, m_Registers.A);
    
    LOG_FLAGS();
}

//0x80

void CPU::opcodeCB0x80() // RES 0,B
{
    opcodeRES(0, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0x81() // RES 0,C
{
    opcodeRES(0, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0x82() // RES 0,D
{
    opcodeRES(0, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0x83() // RES 0,E
{
    opcodeRES(0, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0x84() // RES 0,H
{
    opcodeRES(0, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0x85() // RES 0,L
{
    opcodeRES(0, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0x86() // RES 0,(HL)
{
    opcodeRES_HL(0);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0x87() // RES 0,A
{
    opcodeRES(0, m_Registers.A);
    
    LOG_A_REG();
}

void CPU::opcodeCB0x88() // RES 1,B
{
    opcodeRES(1, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0x89() // RES 1,C
{
    opcodeRES(1, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0x8A() // RES 1,D
{
    opcodeRES(1, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0x8B() // RES 1,E
{
    opcodeRES(1, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0x8C() // RES 1,H
{
    opcodeRES(1, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0x8D() // RES 1,L
{
    opcodeRES(1, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0x8E() // RES 1,(HL)
{
    opcodeRES_HL(1);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0x8F() // RES 1,A
{
    opcodeRES(1, m_Registers.A);
    
    LOG_A_REG();
}

//0x90

void CPU::opcodeCB0x90() // RES 2,B
{
    opcodeRES(2, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0x91() // RES 2,C
{
    opcodeRES(2, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0x92() // RES 2,D
{
    opcodeRES(2, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0x93() // RES 2,E
{
    opcodeRES(2, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0x94() // RES 2,H
{
    opcodeRES(2, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0x95() // RES 2,L
{
    opcodeRES(2, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0x96() // RES 2,(HL)
{
    opcodeRES_HL(2);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0x97() // RES 2,A
{
    opcodeRES(2, m_Registers.A);
    
    LOG_A_REG();
}

void CPU::opcodeCB0x98() // RES 3,B
{
    opcodeRES(3, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0x99() // RES 3,C
{
    opcodeRES(3, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0x9A() // RES 3,D
{
    opcodeRES(3, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0x9B() // RES 3,E
{
    opcodeRES(3, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0x9C() // RES 3,H
{
    opcodeRES(3, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0x9D() // RES 3,L
{
    opcodeRES(3, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0x9E() // RES 3,(HL)
{
    opcodeRES_HL(3);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0x9F() // RES 3,A
{
    opcodeRES(3, m_Registers.A);
    
    LOG_A_REG();
}

//0xA0

void CPU::opcodeCB0xA0() // RES 4,B
{
    opcodeRES(4, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xA1() // RES 4,C
{
    opcodeRES(4, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xA2() // RES 4,D
{
    opcodeRES(4, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xA3() // RES 4,E
{
    opcodeRES(4, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xA4() // RES 4,H
{
    opcodeRES(4, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xA5() // RES 4,L
{
    opcodeRES(4, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xA6() // RES 4,(HL)
{
    opcodeRES_HL(4);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xA7() // RES 4,A
{
    opcodeRES(4, m_Registers.A);
    
    LOG_A_REG();
}

void CPU::opcodeCB0xA8() // RES 5,B
{
    opcodeRES(5, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xA9() // RES 5,C
{
    opcodeRES(5, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xAA() // RES 5,D
{
    opcodeRES(5, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xAB() // RES 5,E
{
    opcodeRES(5, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xAC() // RES 5,H
{
    opcodeRES(5, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xAD() // RES 5,L
{
    opcodeRES(5, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xAE() // RES 5,(HL)
{
    opcodeRES_HL(5);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xAF() // RES 5,A
{
    opcodeRES(5, m_Registers.A);
    
    LOG_A_REG();
}

//0xB0

void CPU::opcodeCB0xB0() // RES 6,B
{
    opcodeRES(6, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xB1() // RES 6,C
{
    opcodeRES(6, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xB2() // RES 6,D
{
    opcodeRES(6, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xB3() // RES 6,E
{
    opcodeRES(6, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xB4() // RES 6,H
{
    opcodeRES(6, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xB5() // RES 6,L
{
    opcodeRES(6, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xB6() // RES 6,(HL)
{
    opcodeRES_HL(6);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xB7() // RES 6,A
{
    opcodeRES(6, m_Registers.A);
    
    LOG_A_REG();
}

void CPU::opcodeCB0xB8() // RES 7,B
{
    opcodeRES(7, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xB9() // RES 7,C
{
    opcodeRES(7, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xBA() // RES 7,D
{
    opcodeRES(7, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xBB() // RES 7,E
{
    opcodeRES(7, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xBC() // RES 7,H
{
    opcodeRES(7, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xBD() // RES 7,L
{
    opcodeRES(7, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xBE() // RES 7,(HL)
{
    opcodeRES_HL(7);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xBF() // RES 7,A
{
    opcodeRES(7, m_Registers.A);
    
    LOG_A_REG();
}

//0xC0

void CPU::opcodeCB0xC0() // SET 0,B
{
    opcodeSET(0, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xC1() // SET 0,C
{
    opcodeSET(0, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xC2() // SET 0,D
{
    opcodeSET(0, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xC3() // SET 0,E
{
    opcodeSET(0, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xC4() // SET 0,H
{
    opcodeSET(0, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xC5() // SET 0,L
{
    opcodeSET(0, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xC6() // SET 0,(HL)
{
    opcodeSET_HL(0);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xC7() // SET 0,A
{
    opcodeSET(0, m_Registers.A);
    
    LOG_A_REG();
}

void CPU::opcodeCB0xC8() // SET 1,B
{
    opcodeSET(1, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xC9() // SET 1,C
{
    opcodeSET(1, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xCA() // SET 1,D
{
    opcodeSET(1, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xCB() // SET 1,E
{
    opcodeSET(1, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xCC() // SET 1,H
{
    opcodeSET(1, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xCD() // SET 1,L
{
    opcodeSET(1, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xCE() // SET 1,(HL)
{
    opcodeSET_HL(1);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xCF() // SET 1,A
{
    opcodeSET(1, m_Registers.A);
    
    LOG_A_REG();
}

//0xD0

void CPU::opcodeCB0xD0() // SET 2,B
{
    opcodeSET(2, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xD1() // SET 2,C
{
    opcodeSET(2, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xD2() // SET 2,D
{
    opcodeSET(2, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xD3() // SET 2,E
{
    opcodeSET(2, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xD4() // SET 2,H
{
    opcodeSET(2, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xD5() // SET 2,L
{
    opcodeSET(2, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xD6() // SET 2,(HL)
{
    opcodeSET_HL(2);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xD7() // SET 2,A
{
    opcodeSET(2, m_Registers.A);
    
    LOG_A_REG();
}

void CPU::opcodeCB0xD8() // SET 3,B
{
    opcodeSET(3, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xD9() // SET 3,C
{
    opcodeSET(3, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xDA() // SET 3,D
{
    opcodeSET(3, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xDB() // SET 3,E
{
    opcodeSET(3, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xDC() // SET 3,H
{
    opcodeSET(3, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xDD() // SET 3,L
{
    opcodeSET(3, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xDE() // SET 3,(HL)
{
    opcodeSET_HL(3);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xDF() // SET 3,A
{
    opcodeSET(3, m_Registers.A);
    
    LOG_A_REG();
}

//0xE0

void CPU::opcodeCB0xE0() // SET 4,B
{
    opcodeSET(4, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xE1() // SET 4,C
{
    opcodeSET(4, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xE2() // SET 4,D
{
    opcodeSET(4, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xE3() // SET 4,E
{
    opcodeSET(4, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xE4() // SET 4,H
{
    opcodeSET(4, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xE5() // SET 4,L
{
    opcodeSET(4, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xE6() // SET 4,(HL)
{
    opcodeSET_HL(4);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xE7() // SET 4,A
{
    opcodeSET(4, m_Registers.A);
    
    LOG_A_REG();
}

void CPU::opcodeCB0xE8() // SET 5,B
{
    opcodeSET(5, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xE9() // SET 5,C
{
    opcodeSET(5, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xEA() // SET 5,D
{
    opcodeSET(5, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xEB() // SET 5,E
{
    opcodeSET(5, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xEC() // SET 5,H
{
    opcodeSET(5, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xED() // SET 5,L
{
    opcodeSET(5, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xEE() // SET 5,(HL)
{
    opcodeSET_HL(5);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xEF() // SET 5,A
{
    opcodeSET(5, m_Registers.A);
    
    LOG_A_REG();
}

//0xF0

void CPU::opcodeCB0xF0() // SET 6,B
{
    opcodeSET(6, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xF1() // SET 6,C
{
    opcodeSET(6, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xF2() // SET 6,D
{
    opcodeSET(6, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xF3() // SET 6,E
{
    opcodeSET(6, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xF4() // SET 6,H
{
    opcodeSET(6, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xF5() // SET 6,L
{
    opcodeSET(6, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xF6() // SET 6,(HL)
{
    opcodeSET_HL(6);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xF7() // SET 6,A
{
    opcodeSET(6, m_Registers.A);
    
    LOG_A_REG();
}

void CPU::opcodeCB0xF8() // SET 7,B
{
    opcodeSET(7, m_Registers.B);
    
    LOG_B_REG();
}

void CPU::opcodeCB0xF9() // SET 7,C
{
    opcodeSET(7, m_Registers.C);
    
    LOG_C_REG();
}

void CPU::opcodeCB0xFA() // SET 7,D
{
    opcodeSET(7, m_Registers.D);
    
    LOG_D_REG();
}

void CPU::opcodeCB0xFB() // SET 7,E
{
    opcodeSET(7, m_Registers.E);
    
    LOG_E_REG();
}

void CPU::opcodeCB0xFC() // SET 7,H
{
    opcodeSET(7, m_Registers.H);
    
    LOG_H_REG();
}

void CPU::opcodeCB0xFD() // SET 7,L
{
    opcodeSET(7, m_Registers.L);
    
    LOG_L_REG();
}

void CPU::opcodeCB0xFE() // SET 7,(HL)
{
    opcodeSET_HL(7);
    
    LOG_WRITE(m_Registers.HL);
}

void CPU::opcodeCB0xFF() // SET 7,A
{
    opcodeSET(7, m_Registers.A);
    
    LOG_A_REG();
}