#include "cpu.h"
#include "gameboy.h"

CPU::CPU()
{
    LOG("Initializing CPU!");
    reset();
}

void CPU::setMMU(MMU* mmu)
{
    m_MMU = mmu;
}

void CPU::tick()
{
    handleInterrupts();
    Instruction instruction = instructions[m_MMU->read(m_Registers.PC++)];
    LOG(std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_MMU->read(m_Registers.PC)));
    ASSERT(instruction.op, "Opcode 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(m_MMU->read(m_Registers.PC - 1)) << ": " << instruction.mnemonic);
    LOG(instruction.mnemonic);
    std::invoke(instruction.op);
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

    u8 flags = m_MMU->read(0xFF0F);
    m_IME = false;
}