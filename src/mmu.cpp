#include "core.hpp"

#include "mmu.hpp"

#include "gameboy.hpp"

MMU::MMU(Gameboy& gb)
    : m_Gameboy(gb)
{
    DEBUG("Initializing MMU!");
}

void MMU::load(const char* path)
{
    Cart::Type type = MBC::getCartType(path);
    DEBUG("Loaded " << MBC::getCartTitle(path) << "!");

    switch(type)
    {
        case Cart::Type::ROM_ONLY:
            m_Rom = std::make_unique<RomOnly>();
            break;
        case Cart::Type::MBC1:
        case Cart::Type::MBC1_RAM:
        case Cart::Type::MBC1_RAM_BATTERY:
            m_Rom = std::make_unique<MBC1>();
            break;
        case Cart::Type::MBC3_TIMER_BATTERY:
        case Cart::Type::MBC3_TIMER_RAM_BATTERY_2:
        case Cart::Type::MBC3:
        case Cart::Type::MBC3_RAM_2:
        case Cart::Type::MBC3_RAM_BATTERY_2:
            m_Rom = std::make_unique<MBC3>();
            break;
        default:
            m_Rom = std::make_unique<RomOnly>();
    }

    m_Rom->load(path);
}

auto MMU::read(u16 address) const -> u8
{
    if(address < ROM_ADDR)
    {
        return m_Rom->read(address);
    }
    else if(address < VRAM_ADDR)
    {
        return m_Memory[address - ROM_SIZE];
    }
    else if(address < RAM_BANK_ADDR)
    {
        return m_Memory[address - ROM_SIZE];
    }
    else if(address < INTERNAL_RAM_ADDR)
    {
        return m_Memory[address - ROM_SIZE];
    }
    else if(address < ECHO_RAM_ADDR)
    {
        return m_Memory[address - ROM_SIZE - INTERNAL_RAM_SIZE]; // Map back into RAM
    }
    else if(address < OAM_ADDR)
    {
        return m_Memory[address - ROM_SIZE];
    }
    else if(address < UNUSABLE_ADDR)
    {
        return UINT8_MAX;
    }
    else if(address < IO_ADDR)
    {
        switch(address)
        {
            case JOYPAD_REGISTER:
                return m_Gameboy.getInput();
                
            default:
                return m_Memory[address - ROM_SIZE];
        }
    }
    else
    {
        return m_Memory[address - ROM_SIZE];
    }

    return UINT8_MAX;
}

void MMU::write(u16 address, u8 val)
{
    if(address < ROM_ADDR)
    {
        m_Rom->write(address, val);
    }
    else if(address < VRAM_ADDR)
    {
        m_Memory[address - ROM_SIZE] = val;
    }
    else if(address < RAM_BANK_ADDR)
    {
        m_Memory[address - ROM_SIZE] = val;
    }
    else if(address < INTERNAL_RAM_ADDR)
    {
        m_Memory[address - ROM_SIZE] = val;
    }
    else if(address < ECHO_RAM_ADDR)
    {
        m_Memory[address - ROM_SIZE - INTERNAL_RAM_SIZE] = val; // Map back into RAM
    }
    else if(address < OAM_ADDR)
    {
        m_Memory[address - ROM_SIZE] = val;
    }
    else if(address < UNUSABLE_ADDR)
    {
        
    }
    else if(address < IO_ADDR)
    {
        switch(address)
        {
            case JOYPAD_REGISTER:
                m_Gameboy.setInput(val);
                break;
            case TIMER_DIV_REGISTER:
                m_Gameboy.resetDiv();
                break;
            case TIMER_TAC_REGISTER:
                m_Memory[address - ROM_SIZE] = val;
                switch(val & 0x03)
                {
                    case 0b00:
                        m_Gameboy.setTimerSpeed(TIMER_SPEED_00);
                        break;
                    case 0b01:
                        m_Gameboy.setTimerSpeed(TIMER_SPEED_01);
                        break;
                    case 0b10:
                        m_Gameboy.setTimerSpeed(TIMER_SPEED_10);
                        break;
                    case 0b11:
                        m_Gameboy.setTimerSpeed(TIMER_SPEED_11);
                        break;
                    default:
                        ASSERT(false, "Timer Speed Switch branched to invalid case!");
                }
                break;
            default:
                m_Memory[address - ROM_SIZE] = val;
        }
    }
    else
    {
        m_Memory[address - ROM_SIZE] = val;
    }
}
