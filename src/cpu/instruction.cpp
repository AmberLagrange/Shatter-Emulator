#include "core.hpp"

#include "instruction.hpp"
#include "cpu.hpp"

#include "gameboy.hpp"

#include "logging/opcode_log.hpp"

//--------------------------------------Opcode Helpers--------------------------------------//

void CPU::pushStack(u16 val)
{
    m_Registers.SP()--;
    m_Gameboy.write(m_Registers.SP(), static_cast<u8>(val >> CHAR_BIT));

    m_Registers.SP()--;
    m_Gameboy.write(m_Registers.SP(), static_cast<u8>(val & UINT8_MAX));

    LOG_PUSH();
}

void CPU::popStack(u16& reg)
{
    u8 low  = m_Gameboy.read(m_Registers.SP());
    m_Registers.SP()++;

    u8 high = m_Gameboy.read(m_Registers.SP());
    m_Registers.SP()++;

    reg = (static_cast<u16>(high) << 8) | low;

    LOG_POP();
}

void CPU::opcodeINC(u8& reg)
{
    reg++;

    clearFlag(Flags::Register::Zero | Flags::Register::Negative | Flags::Register::HalfCarry);
    if((reg & 0x0F) == 0x00) setFlag(Flags::Register::HalfCarry);
    setZeroFromVal(reg);

    LOG_FLAGS();
}

void CPU::opcodeDEC(u8& reg)
{
    reg--;

    clearFlag(Flags::Register::Zero | Flags::Register::Negative | Flags::Register::HalfCarry);

    setFlag(Flags::Register::Negative);
    if((reg & 0x0F) == 0x0F) setFlag(Flags::Register::HalfCarry);
    setZeroFromVal(reg);

    LOG_FLAGS();
}

void CPU::opcodeADD(u8 val)
{
    clearAllFlags();

    if(m_Registers.A() > 0xFF - val) setFlag(Flags::Register::Carry);
    if((m_Registers.A() & 0x0F) + (val & 0x0F) > 0x0F) setFlag(Flags::Register::HalfCarry);

    m_Registers.A() += val;

    setZeroFromVal(m_Registers.A());

    LOG_FLAGS();
    LOG_A_REG();
}

void CPU::opcodeADC(u8 val)
{
    u16 a = static_cast<u16>(m_Registers.A());
    u16 carry = isFlagSet(Flags::Register::Carry);

    clearAllFlags();

    if(a + val + carry > 0x00FF) setFlag(Flags::Register::Carry);
    if((a & 0x0F) + (val & 0x0F) + carry > 0x0F) setFlag(Flags::Register::HalfCarry);

    m_Registers.A() = static_cast<u8>(a + val + carry);
    
    setZeroFromVal(m_Registers.A());

    LOG_FLAGS();
    LOG_A_REG();
}

void CPU::opcodeSUB(u8 val)
{
    clearAllFlags();

    setFlag(Flags::Register::Negative);
    if(m_Registers.A() < val) setFlag(Flags::Register::Carry);
    if(((m_Registers.A() & 0x0F) < (val & 0x0F))) setFlag(Flags::Register::HalfCarry);

    m_Registers.A() -= val;
    
    setZeroFromVal(m_Registers.A());

    LOG_FLAGS();
    LOG_A_REG();
}

void CPU::opcodeSBC(u8 val)
{
    u16 a = static_cast<u16>(m_Registers.A());
    u16 carry = isFlagSet(Flags::Register::Carry);

    clearAllFlags();
    setFlag(Flags::Register::Negative);

    if(a < static_cast<u16>(val) + carry) setFlag(Flags::Register::Carry);
    if((a & 0x000F) < (val & 0x0F) + carry) setFlag(Flags::Register::HalfCarry);

    m_Registers.A() = static_cast<u8>((a - val - carry) & 0x00FF);
    
    setZeroFromVal(m_Registers.A());
}

void CPU::opcodeAND(u8 val)
{   
    m_Registers.A() &= val;

    clearAllFlags();
    setFlag(Flags::Register::HalfCarry);
    setZeroFromVal(m_Registers.A());

    LOG_FLAGS();
    LOG_A_REG();
}

void CPU::opcodeXOR(u8 val)
{
    m_Registers.A() ^= val;

    clearAllFlags();
    setZeroFromVal(m_Registers.A());

    LOG_FLAGS();
    LOG_A_REG();
}

void CPU::opcodeOR(u8 val)
{
    m_Registers.A() |= val;

    clearAllFlags();
    setZeroFromVal(m_Registers.A());

    LOG_FLAGS();
    LOG_A_REG();
}

void CPU::opcodeCP(u8 val)
{
    clearAllFlags();
    setFlag(Flags::Register::Negative);

    if(m_Registers.A() < val) setFlag(Flags::Register::Carry);
    if((m_Registers.A() & 0x0F) < (val & 0x0F)) setFlag(Flags::Register::HalfCarry);
    if(m_Registers.A() == val) setFlag(Flags::Register::Zero);

    LOG_FLAGS();
    LOG_A_REG();
}

void CPU::opcodeJP(bool condition)
{
    u8 low  = m_Gameboy.read(m_Registers.PC()++);
    u8 high = m_Gameboy.read(m_Registers.PC()++);

    if(condition)
    {
        m_Registers.PC() = (static_cast<u16>(high) << 8) | low;
        m_Branched = true;

        LOG_JP();
    }
    else
    {
        LOG_NJP();
    }
}

void CPU::opcodeJR(bool condition)
{
    i8 offset = static_cast<i8>(m_Gameboy.read(m_Registers.PC()++));

    if(condition)
    {
        m_Registers.PC() += offset;
        m_Branched = true;

        LOG_JP();
    }
    else
    {
        LOG_NJP();
    }
}

void CPU::opcodeCALL(bool condition)
{
    u8 low  = m_Gameboy.read(m_Registers.PC()++);
    u8 high = m_Gameboy.read(m_Registers.PC()++);

    if(condition)
    {
        pushStack(m_Registers.PC());
        m_Registers.PC() = (static_cast<u16>(high) << 8) | low;
        m_Branched = true;

        LOG_JP();
    }
    else
    {
        LOG_NJP();
    }
}

void CPU::opcodeRET(bool condiiton)
{
    if(condiiton)
    {
        popStack(m_Registers.PC());
        m_Branched = true;

        LOG_RET();
    }
    else
    {
        LOG_NRET();
    }
}

void CPU::opcodeRST(u16 val)
{
    pushStack(m_Registers.PC());
    m_Registers.PC() = val;

    LOG_JP();
}

// 16 bit variants

void CPU::opcodeADD_HL(u16 val)
{
    clearFlag(Flags::Register::Negative | Flags::Register::HalfCarry | Flags::Register::Carry);
    if(m_Registers.HL() > 0xFFFF - val) setFlag(Flags::Register::Carry);
    if((m_Registers.HL() & 0x0FFF) + (val & 0x0FFF) > 0x0FFF) setFlag(Flags::Register::HalfCarry);

    m_Registers.HL() += val;

    LOG_FLAGS();
    LOG_HL_REG();
}

auto CPU::opcodeADD_SP() -> u16
{
    clearAllFlags();

    i8 offset = static_cast<i8>(m_Gameboy.read(m_Registers.PC()++));

    if((offset & 0xFF) + (m_Registers.SP() & 0x00FF) > 0x00FF) setFlag(Flags::Register::Carry);
    if((offset & 0x0F) + (m_Registers.SP() & 0x000F) > 0x000F) setFlag(Flags::Register::HalfCarry);

    LOG_FLAGS();

    return m_Registers.SP() + offset;
}

//--------------------------------------Opcodes--------------------------------------//

//0x00

void CPU::opcode0x00() // NOP
{

}

void CPU::opcode0x01() // LD BC,u16
{
    m_Registers.C() = m_Gameboy.read(m_Registers.PC()++);
    m_Registers.B() = m_Gameboy.read(m_Registers.PC()++);

    LOG_BC_REG();
}

void CPU::opcode0x02() // LD (BC),A
{
    m_Gameboy.write(m_Registers.BC(), m_Registers.A());

    LOG_WRITE(m_Registers.A());
}

void CPU::opcode0x03() // INC BC
{
    m_Registers.BC()++;

    LOG_BC_REG();
}

void CPU::opcode0x04() // INC B
{
    opcodeINC(m_Registers.B());

    LOG_B_REG();
}

void CPU::opcode0x05() // DEC B
{
    opcodeDEC(m_Registers.B());

    LOG_B_REG();
}

void CPU::opcode0x06() // LD B,u8
{
    m_Registers.B() = m_Gameboy.read(m_Registers.PC()++);

    LOG_B_REG();
}

void CPU::opcode0x07() // RLCA
{
    // RLC A, except Zero Flag isn't set

    clearAllFlags();

    u8 carry = bit_functions::get_bit(m_Registers.A(), 7);
    m_Registers.A() = (m_Registers.A() << 1) | carry;

    if(carry) setFlag(Flags::Register::Carry);
}

void CPU::opcode0x08() // LD (u16),SP
{
    u8 low  = m_Gameboy.read(m_Registers.PC()++);
    u8 high = m_Gameboy.read(m_Registers.PC()++);
    u16 addr = (static_cast<u16>(high) << 8) | low;

    m_Gameboy.write(addr    , static_cast<u8>(m_Registers.SP()            ));
    m_Gameboy.write(addr + 1, static_cast<u8>(m_Registers.SP() >> CHAR_BIT));

    LOG_WRITE(addr);
    LOG_WRITE(addr + 1);
}

void CPU::opcode0x09() // ADD HL,BC
{
    opcodeADD_HL(m_Registers.BC());
}

void CPU::opcode0x0A() // LD A,(BC)
{
    m_Registers.A() = m_Gameboy.read(m_Registers.BC());

    LOG_READ(m_Registers.BC());
    LOG_A_REG();
}

void CPU::opcode0x0B() // DEC BC
{
    m_Registers.BC()--;

    LOG_BC_REG();
}

void CPU::opcode0x0C() // INC C
{
    opcodeINC(m_Registers.C());

    LOG_C_REG();
}

void CPU::opcode0x0D() // DEC C
{
    opcodeDEC(m_Registers.C());

    LOG_C_REG();
}

void CPU::opcode0x0E() // LD C,u8
{
    m_Registers.C() = m_Gameboy.read(m_Registers.PC()++);

    LOG_C_REG();
}

void CPU::opcode0x0F() // RRCA
{
    // RRCA, except Zero Flag isn't set

    clearAllFlags();

    u8 carry = bit_functions::get_bit(m_Registers.A(), 0);
    m_Registers.A() = (carry << 7) | (m_Registers.A() >> 1);

    if(carry) setFlag(Flags::Register::Carry);
}

//0x10

void CPU::opcode0x10() // STOP
{
    m_Registers.PC()++; //Skip next opcode

    OPCODE("Stopped!");
}

void CPU::opcode0x11() // LD DE,u16
{
    m_Registers.E() = m_Gameboy.read(m_Registers.PC()++);
    m_Registers.D() = m_Gameboy.read(m_Registers.PC()++);

    LOG_DE_REG();
}

void CPU::opcode0x12() // LD (DE),A
{
    m_Gameboy.write(m_Registers.DE(), m_Registers.A());

    LOG_WRITE(m_Registers.DE());
}

void CPU::opcode0x13() // INC DE
{
    m_Registers.DE()++;

    LOG_DE_REG();
}

void CPU::opcode0x14() // INC D
{
    opcodeINC(m_Registers.D());

    LOG_D_REG();
}

void CPU::opcode0x15() // DEC D
{
    opcodeDEC(m_Registers.D());

    LOG_D_REG();
}

void CPU::opcode0x16() // LD D,u8
{
    m_Registers.D() = m_Gameboy.read(m_Registers.PC()++);

    LOG_D_REG();
}

void CPU::opcode0x17() // RLA
{
    // RL A, except Zero Flag isn't set

    u8 carry = isFlagSet(Flags::Register::Carry);

    clearAllFlags();

    if(bit_functions::get_bit(m_Registers.A(), 7)) setFlag(Flags::Register::Carry);

    m_Registers.A() = (m_Registers.A() << 1) | carry;

    LOG_FLAGS();
    LOG_A_REG();
}

void CPU::opcode0x18() // JR i8
{
    opcodeJR(true);
}

void CPU::opcode0x19() // ADD HL,DE
{
    opcodeADD_HL(m_Registers.DE());
}

void CPU::opcode0x1A() // LD A,(DE)
{
    m_Registers.A() = m_Gameboy.read(m_Registers.DE());

    LOG_READ(m_Registers.DE());
    LOG_A_REG();
}

void CPU::opcode0x1B() // DEC DE
{
    m_Registers.DE()--;

    LOG_DE_REG();
}

void CPU::opcode0x1C() // INC E
{
    opcodeINC(m_Registers.E());

    LOG_E_REG();
}

void CPU::opcode0x1D() // DEC E
{
    opcodeDEC(m_Registers.E());

    LOG_E_REG();
}

void CPU::opcode0x1E() // LD E,u8
{
    m_Registers.E() = m_Gameboy.read(m_Registers.PC()++);

    LOG_E_REG();
}

void CPU::opcode0x1F() // RRA
{
    // RR A, except Zero Flag isn't set

    u8 carry = isFlagSet(Flags::Register::Carry);

    clearAllFlags();

    if(bit_functions::get_bit(m_Registers.A(), 0)) setFlag(Flags::Register::Carry);

    m_Registers.A() = (carry << 7) | (m_Registers.A() >> 1);
}

//0x20

void CPU::opcode0x20() // JR NZ,i8
{
    opcodeJR(!isFlagSet(Flags::Register::Zero));
}

void CPU::opcode0x21() // LD HL,u16
{
    u8 low  = m_Gameboy.read(m_Registers.PC()++);
    u8 high = m_Gameboy.read(m_Registers.PC()++);
    m_Registers.HL() = (static_cast<u16>(high) << 8) | low;

    LOG_HL_REG();
}

void CPU::opcode0x22() // LD (HL+),A
{
    m_Gameboy.write(m_Registers.HL()++, m_Registers.A());

    LOG_WRITE(m_Registers.HL() - 1);
    LOG_HL_REG();
}

void CPU::opcode0x23() // INC HL
{
    m_Registers.HL()++;

    LOG_HL_REG();
}

void CPU::opcode0x24() // INC H
{
    opcodeINC(m_Registers.H());

    LOG_H_REG();
}

void CPU::opcode0x25() // DEC H
{
    opcodeDEC(m_Registers.H());

    LOG_H_REG();
}

void CPU::opcode0x26() // LD H,u8
{
    m_Registers.H() = m_Gameboy.read(m_Registers.PC()++);

    LOG_H_REG();
}

void CPU::opcode0x27() // DAA
{
    u16 a = static_cast<u16>(m_Registers.A());

    if (!isFlagSet(Flags::Register::Negative))
    {
        if(isFlagSet(Flags::Register::HalfCarry) || (a & 0x0F) > 0x09 )
        {
            a += 0x06;
        }

        if (isFlagSet(Flags::Register::Carry) || a > 0x9F )
        {
            a += 0x60;
        }
    }
    else
    {
        if(isFlagSet(Flags::Register::HalfCarry))
        {
            a -= 0x06;
            if(!isFlagSet(Flags::Register::Carry)) a &= 0xFF;
        }

        if(isFlagSet(Flags::Register::Carry))
        {
            a -= 0x60;
        }
    }

    clearFlag(Flags::Register::Zero | Flags::Register::HalfCarry);

    if(a & 0x0100) setFlag(Flags::Register::Carry);

    m_Registers.A() = static_cast<u16>(a);

    setZeroFromVal(m_Registers.A());

    OPCODE("DAA.");
    LOG_A_REG();
}

void CPU::opcode0x28() // JR Z,i8
{
    opcodeJR(isFlagSet(Flags::Register::Zero));
}

void CPU::opcode0x29() // ADD HL,HL
{
    opcodeADD_HL(m_Registers.HL());
}

void CPU::opcode0x2A() // LD A,(HL+)
{
    m_Registers.A() = m_Gameboy.read(m_Registers.HL()++);

    LOG_READ(m_Registers.HL() - 1);
    LOG_HL_REG();
    LOG_A_REG();
}

void CPU::opcode0x2B() // DEC HL
{
    m_Registers.HL()--;

    LOG_HL_REG();
}

void CPU::opcode0x2C() // INC L
{
    opcodeINC(m_Registers.L());

    LOG_L_REG();
}

void CPU::opcode0x2D() // DEC L
{
    opcodeDEC(m_Registers.L());

    LOG_L_REG();
}

void CPU::opcode0x2E() // LD L,u8
{
    m_Registers.L() = m_Gameboy.read(m_Registers.PC()++);

    LOG_L_REG();
}

void CPU::opcode0x2F() // CPL
{
    m_Registers.A() = ~m_Registers.A();
    setFlag(Flags::Register::Negative | Flags::Register::HalfCarry);

    OPCODE("Compliment of A.");
    LOG_A_REG();
}

//0x30

void CPU::opcode0x30() // JR NC,i8
{
    opcodeJR(!isFlagSet(Flags::Register::Carry));
}

void CPU::opcode0x31() // LD SP,u16
{
    u8 low  = m_Gameboy.read(m_Registers.PC()++);
    u8 high = m_Gameboy.read(m_Registers.PC()++);
    m_Registers.SP() = (static_cast<u16>(high) << 8) | low;

    LOG_SP_REG();
}

void CPU::opcode0x32() // LD (HL-),A
{
    m_Gameboy.write(m_Registers.HL()--, m_Registers.A());

    LOG_WRITE(m_Registers.HL() + 1);
    LOG_HL_REG();
}

void CPU::opcode0x33() // INC SP
{
    m_Registers.SP()++;

    LOG_SP_REG();
}

void CPU::opcode0x34() // INC (HL)
{
    clearFlag(Flags::Register::Zero | Flags::Register::Negative | Flags::Register::HalfCarry);
    u8 val = m_Gameboy.read(m_Registers.HL()) + 1;
    if((val & 0x0F) == 0x00) setFlag(Flags::Register::HalfCarry);
    setZeroFromVal(val);
    m_Gameboy.write(m_Registers.HL(), val);

    LOG_WRITE(m_Registers.HL());
    LOG_FLAGS();
}

void CPU::opcode0x35() // DEC (HL)
{
    clearFlag(Flags::Register::Zero | Flags::Register::Negative | Flags::Register::HalfCarry);
    u8 val = m_Gameboy.read(m_Registers.HL()) - 1;
    setFlag(Flags::Register::Negative);
    if((val & 0x0F) == 0x0F) setFlag(Flags::Register::HalfCarry);
    setZeroFromVal(val);
    m_Gameboy.write(m_Registers.HL(), val);

    LOG_WRITE(m_Registers.HL());
    LOG_FLAGS();
}

void CPU::opcode0x36() // LD (HL),u8
{
    m_Gameboy.write(m_Registers.HL(), m_Gameboy.read(m_Registers.PC()++));

    LOG_WRITE(m_Registers.HL());
}

void CPU::opcode0x37() // SCF
{
    clearFlag(Flags::Register::Negative | Flags::Register::HalfCarry);
    setFlag(Flags::Register::Carry);

    LOG_FLAGS();
}

void CPU::opcode0x38() // JR C,i8
{
    opcodeJR(isFlagSet(Flags::Register::Carry));
}

void CPU::opcode0x39() // ADD HL,SP
{
    opcodeADD_HL(m_Registers.SP());
}

void CPU::opcode0x3A() // LD A,(HL-)
{
    m_Registers.A() = m_Gameboy.read(m_Registers.HL()--);

    LOG_READ(m_Registers.HL() + 1);
    LOG_HL_REG();
    LOG_A_REG();
}

void CPU::opcode0x3B() // DEC SP
{
    m_Registers.SP()--;

    LOG_SP_REG();
}

void CPU::opcode0x3C() // INC A
{
    opcodeINC(m_Registers.A());

    LOG_A_REG();
}

void CPU::opcode0x3D() // DEC A
{
    opcodeDEC(m_Registers.A());

    LOG_A_REG();
}

void CPU::opcode0x3E() // LD A,u8
{
    m_Registers.A() = m_Gameboy.read(m_Registers.PC()++);

    LOG_A_REG();
}

void CPU::opcode0x3F() // CCF
{
    clearFlag(Flags::Register::Negative | Flags::Register::HalfCarry);
    flipFlag(Flags::Register::Carry);

    LOG_FLAGS();
}

//0x40

void CPU::opcode0x40() // LD B,B
{
    m_Registers.B() = m_Registers.B();

    LOG_B_REG();
}

void CPU::opcode0x41() // LD B,C
{
    m_Registers.B() = m_Registers.C();

    LOG_B_REG();
}

void CPU::opcode0x42() // LD B,D
{
    m_Registers.B() = m_Registers.D();

    LOG_B_REG();
}

void CPU::opcode0x43() // LD B,E
{
    m_Registers.B() = m_Registers.E();

    LOG_B_REG();
}

void CPU::opcode0x44() // LD B,H
{
    m_Registers.B() = m_Registers.H();

    LOG_B_REG();
}

void CPU::opcode0x45() // LD B,L
{
    m_Registers.B() = m_Registers.L();

    LOG_B_REG();
}

void CPU::opcode0x46() // LD B,(HL)
{
    m_Registers.B() = m_Gameboy.read(m_Registers.HL());

    LOG_READ(m_Registers.HL());
    LOG_B_REG();
}

void CPU::opcode0x47() // LD B,A
{
    m_Registers.B() = m_Registers.A();

    LOG_B_REG();
}

void CPU::opcode0x48() // LD C,B
{
    m_Registers.C() = m_Registers.B();

    LOG_C_REG();
}

void CPU::opcode0x49() // LD C,C
{
    m_Registers.C() = m_Registers.C();

    LOG_C_REG();
}

void CPU::opcode0x4A() // LD C,D
{
    m_Registers.C() = m_Registers.D();

    LOG_C_REG();
}

void CPU::opcode0x4B() // LD C,E
{
    m_Registers.C() = m_Registers.E();

    LOG_C_REG();
}

void CPU::opcode0x4C() // LD C,H
{
    m_Registers.C() = m_Registers.H();

    LOG_C_REG();
}

void CPU::opcode0x4D() // LD C,L
{
    m_Registers.C() = m_Registers.L();

    LOG_C_REG();
}

void CPU::opcode0x4E() // LD C,(HL)
{
    m_Registers.C() = m_Gameboy.read(m_Registers.HL());

    LOG_READ(m_Registers.HL());
    LOG_C_REG();
}

void CPU::opcode0x4F() // LD C,A
{
    m_Registers.C() = m_Registers.A();

    LOG_C_REG();
}

//0x50

void CPU::opcode0x50() // LD D,B
{
    m_Registers.D() = m_Registers.B();

    LOG_D_REG();
}

void CPU::opcode0x51() // LD D,C
{
    m_Registers.D() = m_Registers.C();

    LOG_D_REG();
}

void CPU::opcode0x52() // LD D,D
{
    m_Registers.D() = m_Registers.D();

    LOG_D_REG();
}

void CPU::opcode0x53() // LD D,E
{
    m_Registers.D() = m_Registers.E();

    LOG_D_REG();
}

void CPU::opcode0x54() // LD D,H
{
    m_Registers.D() = m_Registers.H();

    LOG_D_REG();
}

void CPU::opcode0x55() // LD D,L
{
    m_Registers.D() = m_Registers.L();

    LOG_D_REG();
}

void CPU::opcode0x56() // LD D,(HL)
{
    m_Registers.D() = m_Gameboy.read(m_Registers.HL());

    LOG_READ(m_Registers.HL());
    LOG_D_REG();
}

void CPU::opcode0x57() // LD D,A
{
    m_Registers.D() = m_Registers.A();

    LOG_D_REG();
}

void CPU::opcode0x58() // LD E,B
{
    m_Registers.E() = m_Registers.B();

    LOG_E_REG();
}

void CPU::opcode0x59() // LD E,C
{
    m_Registers.E() = m_Registers.C();

    LOG_E_REG();
}

void CPU::opcode0x5A() // LD E,D
{
    m_Registers.E() = m_Registers.D();

    LOG_E_REG();
}

void CPU::opcode0x5B() // LD E,E
{
    m_Registers.E() = m_Registers.E();

    LOG_E_REG();
}

void CPU::opcode0x5C() // LD E,H
{
    m_Registers.E() = m_Registers.H();

    LOG_E_REG();
}

void CPU::opcode0x5D() // LD E,L
{
    m_Registers.E() = m_Registers.L();

    LOG_E_REG();
}

void CPU::opcode0x5E() // LD E,(HL)
{
    m_Registers.E() = m_Gameboy.read(m_Registers.HL());

    LOG_READ(m_Registers.HL());
    LOG_E_REG();
}

void CPU::opcode0x5F() // LD E,A
{
    m_Registers.E() = m_Registers.A();

    LOG_E_REG();
}

//0x60

void CPU::opcode0x60() // LD H,B
{
    m_Registers.H() = m_Registers.B();

    LOG_H_REG();
}

void CPU::opcode0x61() // LD H,C
{
    m_Registers.H() = m_Registers.C();

    LOG_H_REG();
}

void CPU::opcode0x62() // LD H,D
{
    m_Registers.H() = m_Registers.D();

    LOG_H_REG();
}

void CPU::opcode0x63() // LD H,E
{
    m_Registers.H() = m_Registers.E();

    LOG_H_REG();
}

void CPU::opcode0x64() // LD H,H
{
    m_Registers.H() = m_Registers.H();

    LOG_H_REG();
}

void CPU::opcode0x65() // LD H,L
{
    m_Registers.H() = m_Registers.L();

    LOG_H_REG();
}

void CPU::opcode0x66() // LD H,(HL)
{
    m_Registers.H() = m_Gameboy.read(m_Registers.HL());

    LOG_READ(m_Registers.HL());
    LOG_H_REG();
}

void CPU::opcode0x67() // LD H,A
{
    m_Registers.H() = m_Registers.A();

    LOG_H_REG();
}

void CPU::opcode0x68() // LD L,B
{
    m_Registers.L() = m_Registers.B();

    LOG_L_REG();
}

void CPU::opcode0x69() // LD L,C
{
    m_Registers.L() = m_Registers.C();

    LOG_L_REG();
}

void CPU::opcode0x6A() // LD L,D
{
    m_Registers.L() = m_Registers.D();

    LOG_L_REG();
}

void CPU::opcode0x6B() // LD L,E
{
    m_Registers.L() = m_Registers.E();

    LOG_L_REG();
}

void CPU::opcode0x6C() // LD L,H
{
    m_Registers.L() = m_Registers.H();

    LOG_L_REG();
}

void CPU::opcode0x6D() // LD L,L
{
    m_Registers.L() = m_Registers.L();

    LOG_L_REG();
}

void CPU::opcode0x6E() // LD L,(HL)
{
    m_Registers.L() = m_Gameboy.read(m_Registers.HL());

    LOG_READ(m_Registers.HL());
    LOG_L_REG();
}

void CPU::opcode0x6F() // LD L,A
{
    m_Registers.L() = m_Registers.A();

    LOG_L_REG();
}

//0x70

void CPU::opcode0x70() // LD (HL),B
{
    m_Gameboy.write(m_Registers.HL(), m_Registers.B());

    LOG_WRITE(m_Registers.HL());
}

void CPU::opcode0x71() // LD (HL),C
{
    m_Gameboy.write(m_Registers.HL(), m_Registers.C());

    LOG_WRITE(m_Registers.HL());
}

void CPU::opcode0x72() // LD (HL),D
{
    m_Gameboy.write(m_Registers.HL(), m_Registers.D());

    LOG_WRITE(m_Registers.HL());
}

void CPU::opcode0x73() // LD (HL),E
{
    m_Gameboy.write(m_Registers.HL(), m_Registers.E());

    LOG_WRITE(m_Registers.HL());
}

void CPU::opcode0x74() // LD (HL),H
{
    m_Gameboy.write(m_Registers.HL(), m_Registers.H());

    LOG_WRITE(m_Registers.HL());
}

void CPU::opcode0x75() // LD (HL),L
{
    m_Gameboy.write(m_Registers.HL(), m_Registers.L());

    LOG_WRITE(m_Registers.HL());
}

void CPU::opcode0x76() // HALT
{
    u8 flags = m_Gameboy.read(IF_REGISTER);
    u8 enabledFlags = (flags & m_Gameboy.read(IE_REGISTER));

    if(m_IME)
    {
        m_Halted = false;
    }
    else if (flags & enabledFlags & 0x1f != 0)
    {
        m_Halted = true;
    }
    else
    {
        m_Halted = false;
        m_HaltBug = true;
    }
    
    OPCODE("Halt!");
}

void CPU::opcode0x77() // LD (HL),A
{
    m_Gameboy.write(m_Registers.HL(), m_Registers.A());

    LOG_WRITE(m_Registers.HL());
}

void CPU::opcode0x78() // LD A,B
{
    m_Registers.A() = m_Registers.B();

    LOG_A_REG();
}

void CPU::opcode0x79() // LD A,C
{
    m_Registers.A() = m_Registers.C();

    LOG_A_REG();
}

void CPU::opcode0x7A() // LD A,D
{
    m_Registers.A() = m_Registers.D();

    LOG_A_REG();
}

void CPU::opcode0x7B() // LD A,E
{
    m_Registers.A() = m_Registers.E();

    LOG_A_REG();
}

void CPU::opcode0x7C() // LD A,H
{
    m_Registers.A() = m_Registers.H();

    LOG_A_REG();
}

void CPU::opcode0x7D() // LD A,L
{
    m_Registers.A() = m_Registers.L();

    LOG_A_REG();
}

void CPU::opcode0x7E() // LD A,(HL)
{
    m_Registers.A() = m_Gameboy.read(m_Registers.HL());

    LOG_READ(m_Registers.HL());
    LOG_A_REG();
}

void CPU::opcode0x7F() // LD A,A
{
    m_Registers.A() = m_Registers.A();

    LOG_A_REG();
}

//0x80

void CPU::opcode0x80() // Add A,B
{
    opcodeADD(m_Registers.B());
}

void CPU::opcode0x81() // Add A,C
{
    opcodeADD(m_Registers.C());
}

void CPU::opcode0x82() // Add A,D
{
    opcodeADD(m_Registers.D());
}

void CPU::opcode0x83() // Add A,E
{
    opcodeADD(m_Registers.E());
}

void CPU::opcode0x84() // Add A,H
{
    opcodeADD(m_Registers.H());
}

void CPU::opcode0x85() // Add A,L
{
    opcodeADD(m_Registers.L());
}

void CPU::opcode0x86() // ADD A,(HL)
{
    opcodeADD(m_Gameboy.read(m_Registers.HL()));
}

void CPU::opcode0x87() // ADD A,A
{
    opcodeADD(m_Registers.A());
}

void CPU::opcode0x88() // ADC A,B
{
    opcodeADC(m_Registers.B());
}

void CPU::opcode0x89() // ADC A,C
{
    opcodeADC(m_Registers.C());
}

void CPU::opcode0x8A() // ADC A,D
{
    opcodeADC(m_Registers.D());
}

void CPU::opcode0x8B() // ADC A,E
{
    opcodeADC(m_Registers.E());
}

void CPU::opcode0x8C() // ADC A,H
{
    opcodeADC(m_Registers.H());
}

void CPU::opcode0x8D() // ADC A,L
{
    opcodeADC(m_Registers.L());
}

void CPU::opcode0x8E() // ADC A,(HL)
{
    opcodeADC(m_Gameboy.read(m_Registers.HL()));
}

void CPU::opcode0x8F() // ADC A,A
{
    opcodeADC(m_Registers.A());
}

//0x90

void CPU::opcode0x90() // SUB A,B
{
    opcodeSUB(m_Registers.B());
}

void CPU::opcode0x91() // SUB A,C
{
    opcodeSUB(m_Registers.C());
}

void CPU::opcode0x92() // SUB A,D
{
    opcodeSUB(m_Registers.D());
}

void CPU::opcode0x93() // SUB A,E
{
    opcodeSUB(m_Registers.E());
}

void CPU::opcode0x94() // SUB A,H
{
    opcodeSUB(m_Registers.H());
}

void CPU::opcode0x95() // SUB A,L
{
    opcodeSUB(m_Registers.L());
}

void CPU::opcode0x96() // SUB A,(HL)
{
    opcodeSUB(m_Gameboy.read(m_Registers.HL()));
}

void CPU::opcode0x97() // SUB A,A
{
    opcodeSUB(m_Registers.A());
}

void CPU::opcode0x98() // SBC A,B
{
    opcodeSBC(m_Registers.B());
}

void CPU::opcode0x99() // SBC A,C
{
    opcodeSBC(m_Registers.C());
}

void CPU::opcode0x9A() // SBC A,D
{
    opcodeSBC(m_Registers.D());
}

void CPU::opcode0x9B() // SBC A,E
{
    opcodeSBC(m_Registers.E());
}

void CPU::opcode0x9C() // SBC A,H
{
    opcodeSBC(m_Registers.H());
}

void CPU::opcode0x9D() // SBC A,L
{
    opcodeSBC(m_Registers.L());
}

void CPU::opcode0x9E() // SBC A,(HL)
{
    opcodeSBC(m_Gameboy.read(m_Registers.HL()));
}

void CPU::opcode0x9F() // SBC A,A
{
    opcodeSBC(m_Registers.A());
}

//0xA0

void CPU::opcode0xA0() // AND A,B
{
    opcodeAND(m_Registers.B());
}

void CPU::opcode0xA1() // AND A,C
{
    opcodeAND(m_Registers.C());
}

void CPU::opcode0xA2() // AND A,D
{
    opcodeAND(m_Registers.D());
}

void CPU::opcode0xA3() // AND A,E
{
    opcodeAND(m_Registers.E());
}

void CPU::opcode0xA4() // AND A,H
{
    opcodeAND(m_Registers.H());
}

void CPU::opcode0xA5() // AND A,L
{
    opcodeAND(m_Registers.L());
}

void CPU::opcode0xA6() // AND A,(HL)
{
    opcodeAND(m_Gameboy.read(m_Registers.HL()));
}

void CPU::opcode0xA7() // AND A,A
{
    opcodeAND(m_Registers.A());
}

void CPU::opcode0xA8() // XOR A,B
{
    opcodeXOR(m_Registers.B());
}

void CPU::opcode0xA9() // XOR A,C
{
    opcodeXOR(m_Registers.C());
}

void CPU::opcode0xAA() // XOR A,D
{
    opcodeXOR(m_Registers.D());
}

void CPU::opcode0xAB() // XOR A,E
{
    opcodeXOR(m_Registers.E());
}

void CPU::opcode0xAC() // XOR A,H
{
    opcodeXOR(m_Registers.H());
}

void CPU::opcode0xAD() // XOR A,L
{
    opcodeXOR(m_Registers.L());
}

void CPU::opcode0xAE() // XOR A,(HL)
{
    opcodeXOR(m_Gameboy.read(m_Registers.HL()));
}

void CPU::opcode0xAF() // XOR A,A
{
    opcodeXOR(m_Registers.A());
}

//0xB0

void CPU::opcode0xB0() // OR A,B
{
    opcodeOR(m_Registers.B());
}

void CPU::opcode0xB1() // OR A,C
{
    opcodeOR(m_Registers.C());
}

void CPU::opcode0xB2() // OR A,D
{
    opcodeOR(m_Registers.D());
}

void CPU::opcode0xB3() // OR A,E
{
    opcodeOR(m_Registers.E());
}

void CPU::opcode0xB4() // OR A,H
{
    opcodeOR(m_Registers.H());
}

void CPU::opcode0xB5() // OR A,L
{
    opcodeOR(m_Registers.L());
}

void CPU::opcode0xB6() // OR A,(HL)
{
    opcodeOR(m_Gameboy.read(m_Registers.HL()));
}

void CPU::opcode0xB7() // OR A,A
{
    opcodeOR(m_Registers.A());
}

void CPU::opcode0xB8() // CP A,B
{
    opcodeCP(m_Registers.B());
}

void CPU::opcode0xB9() // CP A,C
{
    opcodeCP(m_Registers.C());
}

void CPU::opcode0xBA() // CP A,D
{
    opcodeCP(m_Registers.D());
}

void CPU::opcode0xBB() // CP A,E
{
    opcodeCP(m_Registers.E());
}

void CPU::opcode0xBC() // CP A,H
{
    opcodeCP(m_Registers.H());
}

void CPU::opcode0xBD() // CP A,L
{
    opcodeCP(m_Registers.L());
}

void CPU::opcode0xBE() // CP A,(HL)
{
    opcodeCP(m_Gameboy.read(m_Registers.HL()));
}

void CPU::opcode0xBF() // CP A,A
{
    opcodeCP(m_Registers.A());
}

//0xC0

void CPU::opcode0xC0() // RET NZ
{
    opcodeRET(!isFlagSet(Flags::Register::Zero));
}

void CPU::opcode0xC1() // POP BC
{
    popStack(m_Registers.BC());

    LOG_BC_REG();
}

void CPU::opcode0xC2() // JP NZ,u16
{
    opcodeJP(!isFlagSet(Flags::Register::Zero));
}

void CPU::opcode0xC3() // JP u16
{
    opcodeJP(true);
}

void CPU::opcode0xC4() // CALL NZ,u16
{
    opcodeCALL(!isFlagSet(Flags::Register::Zero));
}

void CPU::opcode0xC5() // PUSH BC
{
    pushStack(m_Registers.BC());
}

void CPU::opcode0xC6() // ADD A,u8
{
    opcodeADD(m_Gameboy.read(m_Registers.PC()++));
}

void CPU::opcode0xC7() // RST 00h
{
    opcodeRST(RST_0x00);
}

void CPU::opcode0xC8() // RET Z
{
    opcodeRET(isFlagSet(Flags::Register::Zero));
}

void CPU::opcode0xC9() // RET
{
    opcodeRET(true);
}

void CPU::opcode0xCA() // JP Z,u16
{
    opcodeJP(isFlagSet(Flags::Register::Zero));
}

/**

void CPU::opcode0xCB() // PREFIX CB
{

}

**/

void CPU::opcode0xCC() // CALL Z,u16
{
    opcodeCALL(isFlagSet(Flags::Register::Zero));
}

void CPU::opcode0xCD() // CALL u16
{
    opcodeCALL(true);
}

void CPU::opcode0xCE() // ADC A,u8
{
    opcodeADC(m_Gameboy.read(m_Registers.PC()++));
}

void CPU::opcode0xCF() // RST 08h
{
    opcodeRST(RST_0x08);
}

//0xD0

void CPU::opcode0xD0() // RET NC
{
    opcodeRET(!isFlagSet(Flags::Register::Carry));
}

void CPU::opcode0xD1() // POP DE
{
    popStack(m_Registers.DE());

    LOG_DE_REG();
}

void CPU::opcode0xD2() // JP NC,u16
{
    opcodeJP(!isFlagSet(Flags::Register::Carry));
}

void CPU::opcode0xD3() // UNUSED
{

}

void CPU::opcode0xD4() // CALL NC,u16
{
    opcodeCALL(!isFlagSet(Flags::Register::Carry));
}

void CPU::opcode0xD5() // PUSH DE
{
    pushStack(m_Registers.DE());
}

void CPU::opcode0xD6() // SUB A,u8
{
    opcodeSUB(m_Gameboy.read(m_Registers.PC()++));
}

void CPU::opcode0xD7() // RST 10h
{
    opcodeRST(RST_0x10);
}

void CPU::opcode0xD8() // RET C
{
    opcodeRET(isFlagSet(Flags::Register::Carry));
}

void CPU::opcode0xD9() // RETI
{
    opcodeRET(true);
    m_IME = true;
}

void CPU::opcode0xDA() // JP C,u16
{
    opcodeJP(isFlagSet(Flags::Register::Carry));
}

void CPU::opcode0xDB() // UNUSED
{

}

void CPU::opcode0xDC() // CALL C,u16
{
    opcodeCALL(isFlagSet(Flags::Register::Carry));
}

void CPU::opcode0xDD() // UNUSED
{

}

void CPU::opcode0xDE() // SBC A,u8
{
    opcodeSBC(m_Gameboy.read(m_Registers.PC()++));
}

void CPU::opcode0xDF() // RST 18h
{
    opcodeRST(RST_0x18);
}

//0xE0

void CPU::opcode0xE0() // LD (FF00+u8),A
{
    u8 offset = m_Gameboy.read(m_Registers.PC()++);
    u16 addr = 0xFF00 | offset;
    m_Gameboy.write(addr, m_Registers.A());

    LOG_WRITE(addr);
}

void CPU::opcode0xE1() // POP HL
{
    popStack(m_Registers.HL());

    LOG_HL_REG();
}

void CPU::opcode0xE2() // LD (FF00+C),A
{
    u8 offset = m_Registers.C();
    u16 addr = 0xFF00 | offset;
    m_Gameboy.write(addr, m_Registers.A());

    LOG_WRITE(addr);
}

void CPU::opcode0xE3() // UNUSED
{

}

void CPU::opcode0xE4() // UNUSED
{

}

void CPU::opcode0xE5() // PUSH HL
{
    pushStack(m_Registers.HL());
}

void CPU::opcode0xE6() // AND A,u8
{
    opcodeAND(m_Gameboy.read(m_Registers.PC()++));
}

void CPU::opcode0xE7() // RST 20h
{
    opcodeRST(RST_0x20);
}

void CPU::opcode0xE8() // ADD SP,i8
{
    m_Registers.SP() = opcodeADD_SP();

    LOG_SP_REG();
}

void CPU::opcode0xE9() // JP HL
{
    m_Registers.PC() = m_Registers.HL();
    m_Branched = true;

    LOG_JP();
}

void CPU::opcode0xEA() // LD (u16),A
{
    u8 low  = m_Gameboy.read(m_Registers.PC()++);
    u8 high = m_Gameboy.read(m_Registers.PC()++);
    u16 addr = (static_cast<u16>(high) << 8) | low;
    m_Gameboy.write(addr, m_Registers.A());

    LOG_WRITE(addr);
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
    opcodeXOR(m_Gameboy.read(m_Registers.PC()++));
}

void CPU::opcode0xEF() // RST 28h
{
    opcodeRST(RST_0x28);
}

//0xF0

void CPU::opcode0xF0() // LD A,(FF00+u8)
{
    u8 offset = m_Gameboy.read(m_Registers.PC()++);
    u16 addr = 0xFF00 | offset;
    m_Registers.A() = m_Gameboy.read(addr);

    LOG_READ(addr);
    LOG_A_REG();
}

void CPU::opcode0xF1() // POP AF
{
    popStack(m_Registers.AF());
    m_Registers.F() &= 0xF0; // Correct for lower nibble to always be zero

    LOG_AF_REG();
}

void CPU::opcode0xF2() // LD A,(FF00+C)
{
    u16 addr = 0xFF00 | m_Registers.C();
    m_Registers.A() = m_Gameboy.read(addr);

    LOG_READ(addr);
    LOG_A_REG();
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
    pushStack(m_Registers.AF());
}

void CPU::opcode0xF6() // OR A,u8
{
    opcodeOR(m_Gameboy.read(m_Registers.PC()++));
}

void CPU::opcode0xF7() // RST 30h
{
    opcodeRST(RST_0x30);
}

void CPU::opcode0xF8() // LD HL,SP+i8
{
    m_Registers.HL() = opcodeADD_SP();

    LOG_HL_REG();
}

void CPU::opcode0xF9() // LD SP,HL
{
    m_Registers.SP() = m_Registers.HL();

    LOG_SP_REG();
}

void CPU::opcode0xFA() // LD A,(u16)
{
    u8 low  = m_Gameboy.read(m_Registers.PC()++);
    u8 high = m_Gameboy.read(m_Registers.PC()++);
    u16 addr = (static_cast<u16>(high) << 8) | low;

    m_Registers.A() = m_Gameboy.read(addr);

    LOG_READ(addr);
    LOG_A_REG();
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
    opcodeCP(m_Gameboy.read(m_Registers.PC()++));
}

void CPU::opcode0xFF() // RST 38h
{
    opcodeRST(RST_0x38);
}
