#include "instruction.h"
#include "gameboy.h"

#ifndef NDEBUG
    #include <string>
    #define INSTRUCTION(mnemonic, op, length, cyclesBranch, cyclesNoBranch) { mnemonic, op, length, cyclesBranch, cyclesNoBranch }
#else
    #define INSTRUCTION(mnemonic, op, length, cyclesBranch, cyclesNoBranch) { op, length, cyclesBranch, cyclesNoBranch }
#endif

//--------------------------------------Opcode Helpers--------------------------------------//

void Instruction::opcodeDec(Gameboy* gb, u8& reg)
{
    reg--;
    gb->m_CPU.isFlagSet(CPU::Flags::Carry) ? gb->m_CPU.setFlags(CPU::Flags::Carry) : gb->m_CPU.clearFlags();
    gb->m_CPU.toggleZeroFromVal(reg);
    gb->m_CPU.toggleFlag(CPU::Flags::Negative);
    if((reg & 0x0f) == 0x0f) gb->m_CPU.toggleFlag(CPU::Flags::HalfCarry);   
}

void Instruction::opcodeLoadu8(Gameboy* gb, u8& reg)
{
    reg = gb->read(gb->m_CPU.m_Registers.PC++);
}

void Instruction::opcodeLoadA(Gameboy* gb, const u8& val)
{
    gb->m_CPU.m_Registers.A = val;

    LOG("Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.A) << " into register A.");
}

void Instruction::opcodeXOR(Gameboy* gb, const u8& val)
{
    gb->m_CPU.m_Registers.A ^= val;
    gb->m_CPU.clearFlags();
    gb->m_CPU.toggleZeroFromVal(gb->m_CPU.m_Registers.A);
}

void Instruction::opcodeJP(Gameboy* gb, bool condition)
{
    u8 low  = gb->read(gb->m_CPU.m_Registers.PC++);
    u8 high = gb->read(gb->m_CPU.m_Registers.PC++);
    u16 address = static_cast<u16>(high) << 8 | static_cast<u16>(low);

    if(condition)
    {
        gb->m_CPU.m_Registers.PC = address;
        LOG("Jumped to 0x" << std::setw(4) << std::setfill('0') << std::hex << gb->m_CPU.m_Registers.PC << ".");
    }
    else
    {
        LOG("Did not jump.");
    }
}

void Instruction::opcodeJPOffset(Gameboy* gb, bool condition)
{
    i8 offset = gb->read(gb->m_CPU.m_Registers.PC++);
    u16 address = gb->m_CPU.m_Registers.PC + offset;

    if(condition)
    {
        gb->m_CPU.m_Registers.PC = address;
        LOG("Jumped to 0x" << std::setw(4) << std::setfill('0') << std::hex << gb->m_CPU.m_Registers.PC << ".");
    }
    else
    {
        LOG("Did not jump.");
    }
}

//--------------------------------------Opcodes--------------------------------------//

//0x00

void Instruction::op00([[maybe_unused]] Gameboy* gb)
{
    
}

void Instruction::op05(Gameboy* gb)
{
    opcodeDec(gb, gb->m_CPU.m_Registers.B);

    LOG("Decremented B to 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.B) << ".");
    LOG("Flags Updated to 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.F) << ".");
}

void Instruction::op06(Gameboy* gb)
{
    opcodeLoadu8(gb, gb->m_CPU.m_Registers.B);

    LOG("Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.B) << " into register B.");
}

void Instruction::op0d(Gameboy* gb)
{
    opcodeDec(gb, gb->m_CPU.m_Registers.C);

    LOG("Decremented C to 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.B) << ".");
    LOG("Flags Updated to 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.F) << ".");
}

void Instruction::op0E(Gameboy* gb)
{
    opcodeLoadu8(gb, gb->m_CPU.m_Registers.C);

    LOG("Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.C) << " into register C.");
}

//0x10

//0x20

void Instruction::op20(Gameboy* gb)
{
    opcodeJPOffset(gb, !gb->m_CPU.isFlagSet(CPU::Flags::Zero));
}

void Instruction::op21(Gameboy* gb)
{
    u8 low  = gb->read(gb->m_CPU.m_Registers.PC++);
    u8 high = gb->read(gb->m_CPU.m_Registers.PC++);
    gb->m_CPU.m_Registers.HL = static_cast<u16>(high) << 8 | static_cast<u16>(low);

    LOG("Loaded 0x" << std::setw(4) << std::setfill('0') << std::hex << gb->m_CPU.m_Registers.HL << " into HL.");
}

//0x30

void Instruction::op32(Gameboy* gb)
{
    gb->write(gb->m_CPU.m_Registers.HL--, gb->m_CPU.m_Registers.A);
    LOG("Wrote " << static_cast<u16>(gb->m_CPU.m_Registers.A) << " at memory address " << (gb->m_CPU.m_Registers.HL + 1));
}

void Instruction::op3E(Gameboy* gb)
{
    u8 val = gb->m_CPU.m_Registers.PC++;
    opcodeLoadA(gb, val);
}

//0x40

//0x50

//0x60

//0x70

//0x80

//0x90

//0xA0

void Instruction::opAF(Gameboy* gb)
{
    opcodeXOR(gb, gb->m_CPU.m_Registers.A);
    LOG("A XOR'd with A. Value 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.A) << " written.");
    LOG("Flags Updated to 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.F) << ".");
}

//0xB0

//0xC0

void Instruction::opC3(Gameboy* gb)
{
    opcodeJP(gb, true);
}

//0xD0

//0xE0

void Instruction::opE0(Gameboy* gb)
{
    u8 offset = gb->m_CPU.m_Registers.PC++;
    u16 address = 0xFF00 | offset;

    gb->write(address, gb->m_CPU.m_Registers.A);

    LOG("Wrote 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(gb->m_CPU.m_Registers.A) << " at address 0x" << address << ".");
}

//0xF0

void Instruction::opF0(Gameboy* gb)
{
    u8 offset = gb->m_CPU.m_Registers.PC++;
    u16 address = 0xFF00 | offset;
    u8 val = gb->read(address);

    opcodeLoadA(gb, val);
}

void Instruction::opF3(Gameboy* gb)
{
    gb->m_CPU.m_InterruptEnabled = false;

    LOG("Disabled Interrupts.");
}

void Instruction::opFB(Gameboy* gb)
{
    gb->m_CPU.m_InterruptEnabled = true;

    LOG("Enabled Interrupts.");
}

const Instruction Instruction::instructions[0x100] = 
{
    //0x0
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
    INSTRUCTION("DEC C",            Instruction::op0d, 1,  4,  4),
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
    INSTRUCTION("CP A,u8",          NULL, 2,  8,  8),
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