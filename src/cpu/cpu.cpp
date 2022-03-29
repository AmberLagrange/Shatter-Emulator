#include "cpu.hpp"
#include "gameboy.hpp"

#ifndef NDEBUG
    #define LOG_OP() OPCODE(instruction.mnemonic) //NOLINT(cppcoreguidelines-macro-usage)
#else
    #define LOG_OP() ((void)0) //NOLINT(cppcoreguidelines-macro-usage)
#endif

CPU::CPU(Gameboy& gb)
    : m_Registers({}), m_Gameboy(gb), m_Halted(false), m_IME(false), m_Branched(false)
{
    DEBUG("Initializing CPU!");
    reset();
}

auto CPU::tick() -> u8
{
    u8 cycles = 0;
    if(m_Halted) return 4; // Halted CPU takes 4 ticks

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

//--------------------------------------Registers--------------------------------------//

auto CPU::Registers::A() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->af).at(0);
        }
        else
        {
            u16 af = std::get<u16>(this->af);
            u8   a = static_cast<u8>(af >> CHAR_BIT);
            u8   f = static_cast<u8>(af & UINT8_MAX);
            this->af = std::array<u8, 2> {a, f};
            return std::get<std::array<u8, 2>>(this->af).at(0);
        }
    }, this->af);
}

auto CPU::Registers::F() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->af).at(1);
        }
        else
        {
            u16 af = std::get<u16>(this->af);
            u8   a = static_cast<u8>(af >> CHAR_BIT);
            u8   f = static_cast<u8>(af & UINT8_MAX);
            this->af = std::array<u8, 2> {a, f};
            return std::get<std::array<u8, 2>>(this->af).at(1);
        }
    }, this->af);
}

auto CPU::Registers::AF() -> u16&
{
    return std::visit([this] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& af = std::get<std::array<u8, 2>>(this->af);
            u16 a = static_cast<u16>(af.at(0) << CHAR_BIT);
            u16 f = static_cast<u16>(af.at(1)            );
            this->af = static_cast<u16>(a | f);
            return std::get<u16>(this->af);
        }
        else
        {
            return std::get<u16>(this->af);
        }
    }, this->af);
}

auto CPU::Registers::A() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->af).at(0);
        }
        else
        {
            u16 af = std::get<u16>(this->af);
            u8   a = static_cast<u8>(af >> CHAR_BIT);
            u8   f = static_cast<u8>(af & UINT8_MAX);
            this->af = std::array<u8, 2> {a, f};
            return std::get<std::array<u8, 2>>(this->af).at(0);
        }
    }, this->af);
}

auto CPU::Registers::F() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->af).at(1);
        }
        else
        {
            u16 af = std::get<u16>(this->af);
            u8   a = static_cast<u8>(af >> CHAR_BIT);
            u8   f = static_cast<u8>(af & UINT8_MAX);
            this->af = std::array<u8, 2> {a, f};
            return std::get<std::array<u8, 2>>(this->af).at(1);
        }
    }, this->af);
}

auto CPU::Registers::AF() const -> u16
{
    return std::visit([this] (auto&& arg) -> u16
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& af = std::get<std::array<u8, 2>>(this->af);
            u16 a = static_cast<u16>(af.at(0) << CHAR_BIT);
            u16 f = static_cast<u16>(af.at(1)            );
            this->af = static_cast<u16>(a | f);
            return std::get<u16>(this->af);
        }
        else
        {
            return std::get<u16>(this->af);
        }
    }, this->af);
}

auto CPU::Registers::B() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->bc).at(0);
        }
        else
        {
            u16 bc = std::get<u16>(this->bc);
            u8   b = static_cast<u8>(bc >> CHAR_BIT);
            u8   c = static_cast<u8>(bc & UINT8_MAX);
            this->bc = std::array<u8, 2> {b, c};
            return std::get<std::array<u8, 2>>(this->bc).at(0);
        }
    }, this->bc);
}

auto CPU::Registers::C() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->bc).at(1);
        }
        else
        {
            u16 bc = std::get<u16>(this->bc);
            u8   b = static_cast<u8>(bc >> CHAR_BIT);
            u8   c = static_cast<u8>(bc & UINT8_MAX);
            this->bc = std::array<u8, 2> {b, c};
            return std::get<std::array<u8, 2>>(this->bc).at(1);
        }
    }, this->bc);
}

auto CPU::Registers::BC() -> u16&
{
    return std::visit([this] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& bc = std::get<std::array<u8, 2>>(this->bc);
            u16 b = static_cast<u16>(bc.at(0) << CHAR_BIT);
            u16 c = static_cast<u16>(bc.at(1)            );
            this->bc = static_cast<u16>(b | c);
            return std::get<u16>(this->bc);
        }
        else
        {
            return std::get<u16>(this->bc);
        }
    }, this->bc);
}

auto CPU::Registers::B() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->bc).at(0);
        }
        else
        {
            u16 bc = std::get<u16>(this->bc);
            u8   b = static_cast<u8>(bc >> CHAR_BIT);
            u8   c = static_cast<u8>(bc & UINT8_MAX);
            this->bc = std::array<u8, 2> {b, c};
            return std::get<std::array<u8, 2>>(this->bc).at(0);
        }
    }, this->bc);
}

auto CPU::Registers::C() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->bc).at(1);
        }
        else
        {
            u16 bc = std::get<u16>(this->bc);
            u8   b = static_cast<u8>(bc >> CHAR_BIT);
            u8   c = static_cast<u8>(bc & UINT8_MAX);
            this->bc = std::array<u8, 2> {b, c};
            return std::get<std::array<u8, 2>>(this->bc).at(1);
        }
    }, this->bc);
}

auto CPU::Registers::BC() const -> u16
{
    return std::visit([this] (auto&& arg) -> u16
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& bc = std::get<std::array<u8, 2>>(this->bc);
            u16 b = static_cast<u16>(bc.at(0) << CHAR_BIT);
            u16 c = static_cast<u16>(bc.at(1)            );
            this->bc = static_cast<u16>(b | c);
            return std::get<u16>(this->bc);
        }
        else
        {
            return std::get<u16>(this->bc);
        }
    }, this->bc);
}

auto CPU::Registers::D() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->de).at(0);
        }
        else
        {
            u16 de = std::get<u16>(this->de);
            u8   d = static_cast<u8>(de >> CHAR_BIT);
            u8   e = static_cast<u8>(de & UINT8_MAX);
            this->de = std::array<u8, 2> {d, e};
            return std::get<std::array<u8, 2>>(this->de).at(0);
        }
    }, this->de);
}

auto CPU::Registers::E() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->de).at(1);
        }
        else
        {
            u16 de = std::get<u16>(this->de);
            u8   d = static_cast<u8>(de >> CHAR_BIT);
            u8   e = static_cast<u8>(de & UINT8_MAX);
            this->de = std::array<u8, 2> {d, e};
            return std::get<std::array<u8, 2>>(this->de).at(1);
        }
    }, this->de);
}

auto CPU::Registers::DE() -> u16&
{
    return std::visit([this] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& de = std::get<std::array<u8, 2>>(this->de);
            u16 d = static_cast<u16>(de.at(0) << CHAR_BIT);
            u16 e = static_cast<u16>(de.at(1)            );
            this->de = static_cast<u16>(d | e);
            return std::get<u16>(this->de);
        }
        else
        {
            return std::get<u16>(this->de);
        }
    }, this->de);
}

auto CPU::Registers::D() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->de).at(0);
        }
        else
        {
            u16 de = std::get<u16>(this->de);
            u8   d = static_cast<u8>(de >> CHAR_BIT);
            u8   e = static_cast<u8>(de & UINT8_MAX);
            this->de = std::array<u8, 2> {d, e};
            return std::get<std::array<u8, 2>>(this->de).at(0);
        }
    }, this->de);
}

auto CPU::Registers::E() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->de).at(1);
        }
        else
        {
            u16 de = std::get<u16>(this->de);
            u8   d = static_cast<u8>(de >> CHAR_BIT);
            u8   e = static_cast<u8>(de & UINT8_MAX);
            this->de = std::array<u8, 2> {d, e};
            return std::get<std::array<u8, 2>>(this->de).at(1);
        }
    }, this->de);
}

auto CPU::Registers::DE() const -> u16
{
    return std::visit([this] (auto&& arg) -> u16
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& de = std::get<std::array<u8, 2>>(this->de);
            u16 d = static_cast<u16>(de.at(0) << CHAR_BIT);
            u16 e = static_cast<u16>(de.at(1)            );
            this->de = static_cast<u16>(d | e);
            return std::get<u16>(this->de);
        }
        else
        {
            return std::get<u16>(this->de);
        }
    }, this->de);
}

auto CPU::Registers::H() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->hl).at(0);
        }
        else
        {
            u16 hl = std::get<u16>(this->hl);
            u8   h = static_cast<u8>(hl >> CHAR_BIT);
            u8   l = static_cast<u8>(hl & UINT8_MAX);
            this->hl = std::array<u8, 2> {h, l};
            return std::get<std::array<u8, 2>>(this->hl).at(0);
        }
    }, this->hl);
}

auto CPU::Registers::L() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->hl).at(1);
        }
        else
        {
            u16 hl = std::get<u16>(this->hl);
            u8   h = static_cast<u8>(hl >> CHAR_BIT);
            u8   l = static_cast<u8>(hl & UINT8_MAX);
            this->hl = std::array<u8, 2> {h, l};
            return std::get<std::array<u8, 2>>(this->hl).at(1);
        }
    }, this->hl);
}

auto CPU::Registers::HL() -> u16&
{
    return std::visit([this] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& hl = std::get<std::array<u8, 2>>(this->hl);
            u16 h = static_cast<u16>(hl.at(0) << CHAR_BIT);
            u16 l = static_cast<u16>(hl.at(1)            );
            this->hl = static_cast<u16>(h | l);
            return std::get<u16>(this->hl);
        }
        else
        {
            return std::get<u16>(this->hl);
        }
    }, this->hl);
}

auto CPU::Registers::H() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->hl).at(0);
        }
        else
        {
            u16 hl = std::get<u16>(this->hl);
            u8   h = static_cast<u8>(hl >> CHAR_BIT);
            u8   l = static_cast<u8>(hl & UINT8_MAX);
            this->hl = std::array<u8, 2> {h, l};
            return std::get<std::array<u8, 2>>(this->hl).at(0);
        }
    }, this->hl);
}

auto CPU::Registers::L() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->hl).at(1);
        }
        else
        {
            u16 hl = std::get<u16>(this->hl);
            u8   h = static_cast<u8>(hl >> CHAR_BIT);
            u8   l = static_cast<u8>(hl & UINT8_MAX);
            this->hl = std::array<u8, 2> {h, l};
            return std::get<std::array<u8, 2>>(this->hl).at(1);
        }
    }, this->hl);
}

auto CPU::Registers::HL() const -> u16
{
    return std::visit([this] (auto&& arg) -> u16
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& hl = std::get<std::array<u8, 2>>(this->hl);
            u16 h = static_cast<u16>(hl.at(0) << CHAR_BIT);
            u16 l = static_cast<u16>(hl.at(1)            );
            this->hl = static_cast<u16>(h | l);
            return std::get<u16>(this->hl);
        }
        else
        {
            return std::get<u16>(this->hl);
        }
    }, this->hl);
}