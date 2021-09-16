#include "cpu.h"
#include "gameboy.h"

#ifdef LOG_ALL
    #define LOG_OP() LOG(instruction.mnemonic);
#else
    #define LOG_OP() ((void)0)
#endif

CPU::CPU()
{
    LOG("Initializing CPU!");
    reset();
}

u8 CPU::tick()
{
    handleInterrupts();

    Instruction instruction;
    u8 cycles = 0;
    u8 opcode = m_MMU->read(m_Registers.PC++);

    if(opcode == 0xCB)
    {
        opcode = m_MMU->read(m_Registers.PC++);
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

    m_Registers.PC = 0x0000;
    LOG("\tPC Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC);

    m_IME = false;
    m_Branched = false;
}

void CPU::handleInterrupts()
{
    if(!m_IME)
        return;

    u8 flags = m_MMU->read(0xFF0F);
    m_IME = false;
}