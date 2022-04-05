#include "cpu.hpp"

#include "gameboy.hpp"

#ifdef NDEBUG
    #define LOG_OP() ((void)0) //NOLINT(cppcoreguidelines-macro-usage)
#else
    #define LOG_OP() OPCODE(instruction.mnemonic) //NOLINT(cppcoreguidelines-macro-usage)
#endif

CPU::CPU(Gameboy& gb)
    : m_Registers({}), m_Gameboy(gb), m_Halted(false), m_IME(false), m_Branched(false)
{
    DEBUG("Initializing CPU!");
    reset();
}

auto CPU::getIME() const -> bool
{
    return m_IME;
}

void CPU::setIME(bool ime)
{
    m_IME = ime;
}

auto CPU::tick() -> u8
{
    u8 cycles = 0;
    if(m_Halted) return 4; // Halted CPU takes 4 cycles

    Instruction instruction;
    u8 opcode = m_Gameboy.read(m_Registers.PC()++);

    if(opcode == CB_OPCODE)
    {
        opcode = m_Gameboy.read(m_Registers.PC()++);
        instruction = instructionsCB.at(opcode);
        cycles += 4; // Add 4 cycles due to the CB prefix

        ASSERT(instruction.op, "Opcode CB 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(opcode) << ": " << instruction.mnemonic);
    }
    else
    {
        instruction = instructions.at(opcode);
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

    m_Gameboy.write(IF_REGISTER, m_Gameboy.read(IF_REGISTER) | flag);
}

void CPU::handleInterrupts(u8& cycles)
{
    u8 flags = m_Gameboy.read(IF_REGISTER);
    u8 enabledFlags = (flags & m_Gameboy.read(IE_REGISTER));

    if(enabledFlags)
    {
        if(m_IME)
        {
            pushStack(m_Registers.PC());

            if(enabledFlags & Flags::Interrupt::VBlank)
            {
                flags &= ~Flags::Interrupt::VBlank;
                m_Registers.PC() = VBLANK_VECTOR;
            }
            else if(enabledFlags & Flags::Interrupt::LCD_STAT)
            {
                flags &= ~Flags::Interrupt::LCD_STAT;
                m_Registers.PC() = LCD_STAT_VECTOR;
            }
            else if(enabledFlags & Flags::Interrupt::Timer)
            {
                flags &= ~Flags::Interrupt::Timer;
                m_Registers.PC() = TIMER_VECTOR;
            }
            else if(enabledFlags & Flags::Interrupt::Serial)
            {
                flags &= ~Flags::Interrupt::Serial;
                m_Registers.PC() = SERIAL_VECTOR;
            }
            else if(enabledFlags & Flags::Interrupt::Joypad)
            {
                flags &= ~Flags::Interrupt::Joypad;
                m_Registers.PC() = JOYPAD_VECTOR;
            }
            
            m_IME = false;
            m_Gameboy.write(IF_REGISTER, flags);
            // TODO: Have more accurate cycle updating
            cycles += 20; //NOLINT(cppcoreguidelines-avoid-magic-numbers)
        }
        else if(m_Halted)
        {
            m_Halted = false;
        }
    }
}

void CPU::reset()
{
    DEBUG("CPU Reset sequence:");

    m_Registers.AF() = AF_RESET;
    DEBUG("\tAF Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.AF());

    m_Registers.BC() = BC_RESET;
    DEBUG("\tBC Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.BC());

    m_Registers.DE() = DE_RESET;
    DEBUG("\tDE Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.DE());

    m_Registers.HL() = HL_RESET;
    DEBUG("\tHL Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.HL());

    m_Registers.SP() = SP_RESET;
    DEBUG("\tSP Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.SP());

    m_Registers.PC() = PC_RESET;
    DEBUG("\tPC Register: 0x" << std::setw(4) << std::setfill('0') << std::hex << m_Registers.PC());

    m_Halted = false;
    m_IME = false;
    m_Branched = false;

    m_Gameboy.resetDiv();
    m_Gameboy.write(TIMA_REGISTER, 0);
}

auto CPU::isFlagSet(const Flags::Register& flag) const -> bool
{
    return m_Registers.F() & flag;
}

void CPU::setFlag(const Flags::Register& flag)
{
    m_Registers.F() |= flag;
}

void CPU::clearFlag(const Flags::Register& flag)
{
    m_Registers.F() &= ~flag;
}

void CPU::flipFlag(const Flags::Register& flag)
{
    m_Registers.F() ^= flag;
}

void CPU::clearAllFlags()
{
    clearFlag(Flags::Register::Zero      |
              Flags::Register::Negative  |
              Flags::Register::HalfCarry |
              Flags::Register::Carry);
}

void CPU::setZeroFromVal(u8 val)
{
    if(!val) setFlag(Flags::Register::Zero);
}