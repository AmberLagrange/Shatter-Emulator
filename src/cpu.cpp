#include "cpu.h"
#include "gameboy.h"

CPU::CPU(Gameboy* gb)
    : m_Gameboy(gb)
{
    LOG("Initializing CPU!");
    reset();
}

void CPU::tick()
{
    handleInterrupts();
    Instruction instruction = Instruction::instructions[m_Gameboy->read(m_Registers.PC++)];
    ASSERT(instruction.op, "Opcode 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_Gameboy->read(m_Registers.PC - 1)) << ": " << instruction.mnemonic);
    LOG(instruction.mnemonic);
    instruction.op(m_Gameboy);
}

void CPU::reset()
{
    LOG("CPU Reset sequence:");

    m_Registers.AF = 0x01B0;
    LOG("\tAF Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.AF);

    m_Registers.BC = 0x0013;
    LOG("\tBC Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.BC);

    m_Registers.DE = 0x00D8;
    LOG("\tDE Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.DE);

    m_Registers.HL = 0x014D;
    LOG("\tHL Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.HL);

    m_Registers.SP = 0xFFFE;
    LOG("\tSP Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.SP);

    m_Registers.PC = 0x0100;
    LOG("\tPC Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC);

    m_IME = false;
}

void CPU::handleInterrupts()
{
    if(!m_IME)
        return;

    u8 flags = m_Gameboy->read(0xFF0F);
    m_IME = false;
}

void CPU::setFlag(const Flags::Register& flag)
{
    m_Registers.F |= flag;
}

void CPU::clearFlag(const Flags::Register& flag)
{
    m_Registers.F &= ~flag;
}

void CPU::flipFlag(const Flags::Register& flag)
{
    m_Registers.F ^= flag;
}

bool CPU::isFlagSet(const Flags::Register& flag)
{
    return m_Registers.F & flag;
}

void CPU::clearAllFlags()
{
    clearFlag(Flags::Register::Zero | Flags::Register::Negative | Flags::Register::HalfCarry | Flags::Register::Carry);
}

void CPU::setZeroFromVal(const u8& val)
{
    if(!val) setFlag(Flags::Register::Zero);
}