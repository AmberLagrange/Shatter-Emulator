#include "cpu.h"
#include "gameboy.h"

#ifndef NDEBUG
    #define LOG_OP() OPCODE(instruction.mnemonic);
#else
    #define LOG_OP() ((void)0)
#endif

CPU::CPU()
{
    DEBUG("Initializing CPU!");
    reset();
}

u8 CPU::tick()
{
    if(m_Halted) return 4;

    handleInterrupts();

    Instruction instruction;
    u8 cycles = 0;
    u8 opcode = m_Gameboy->read(m_Registers.PC++);

    if(opcode == 0xCB)
    {
        opcode = m_Gameboy->read(m_Registers.PC++);
        instruction = instructionsCB[opcode];
        cycles += 4; // Add 4 cycles due to the CB prefix

        ASSERT(instruction.op, "Opcode CB 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(opcode) << ": " << instruction.mnemonic);
    }
    else
    {
        instruction = instructions[opcode];
        ASSERT(instruction.op, "Opcode 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(opcode) << ": " << instruction.mnemonic);
    }
    
    LOG_OP();
    std::invoke(instruction.op);

    if(m_Branched)
    {
        m_Branched = false;
        cycles += instruction.cyclesBranch;
    }
    else
    {
        cycles += instruction.cyclesNoBranch;
    }

    return cycles;
}

void CPU::raiseInterrupt(const Flags::Interrupt& flag)
{
    m_Halted = false;

    m_Gameboy->write(IF_REGISTER, m_Gameboy->read(IF_REGISTER) | flag);
}

void CPU::handleInterrupts()
{
    u8 flags = m_Gameboy->read(IF_REGISTER);
    u8 enabledFlags = (flags & m_Gameboy->read(IE_REGISTER));

    if(m_IME && (enabledFlags & 0x1F))
    {
        pushStack(m_Registers.PC);

        if(enabledFlags & Flags::Interrupt::VBlank)
        {
            flags &= ~Flags::Interrupt::VBlank;
            m_Registers.PC = 0x0040;
        }
        else if(enabledFlags & Flags::Interrupt::LCD_STAT)
        {
            flags &= ~Flags::Interrupt::LCD_STAT;
            m_Registers.PC = 0x0048;
        }
        else if(enabledFlags & Flags::Interrupt::Timer)
        {
            flags &= ~Flags::Interrupt::Timer;
            m_Registers.PC = 0x0050;
        }
        else if(enabledFlags & Flags::Interrupt::Serial)
        {
            flags &= ~Flags::Interrupt::Serial;
            m_Registers.PC = 0x0058;
        }
        else if(enabledFlags & Flags::Interrupt::Joypad)
        {
            flags &= ~Flags::Interrupt::Joypad;
            m_Registers.PC = 0x0060;
        }
        
        m_IME = false;
        m_Gameboy->write(IF_REGISTER, flags);
    }
}

void CPU::reset()
{
    DEBUG("CPU Reset sequence:");

    m_Registers.AF = 0x01B0;
    DEBUG("\tAF Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.AF);

    m_Registers.BC = 0x0013;
    DEBUG("\tBC Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.BC);

    m_Registers.DE = 0x00D8;
    DEBUG("\tDE Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.DE);

    m_Registers.HL = 0x014D;
    DEBUG("\tHL Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.HL);

    m_Registers.SP = 0xFFFE;
    DEBUG("\tSP Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.SP);

    m_Registers.PC = 0x0100;
    DEBUG("\tPC Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC);

    m_IME = false;
    m_Halted = false;
    m_Branched = false;
}