#include "mmu.hpp"
#include "gameboy.hpp"

MMU::MMU(Gameboy& gb)
    : m_Gameboy(gb)
{
    DEBUG("Initializing MMU!");
}

void MMU::load(const char* path)
{
    m_Rom.load(path);
}

auto MMU::read(u16 address) const -> u8
{
    if(address < ROM_ADDR)
    {
        return m_Rom.read(address);
    }
    else if(address < VRAM_ADDR)
    {
        return m_Memory.at(address - ROM_SIZE);
    }
    else if(address < RAM_BANK_ADDR)
    {
        return m_Memory.at(address - ROM_SIZE);
    }
    else if(address < INTERNAL_RAM_ADDR)
    {
        return m_Memory.at(address - ROM_SIZE);
    }
    else if(address < ECHO_RAM_ADDR)
    {
        return m_Memory.at(address - ROM_SIZE - INTERNAL_RAM_SIZE); // Map back into RAM
    }
    else if(address < OAM_ADDR)
    {
        return m_Memory.at(address - ROM_SIZE);
    }
    else if(address < UNUSABLE_ADDR)
    {
        return UINT8_MAX;
    }
    else if(address < IO_ADDR)
    {
        return m_Memory.at(address - ROM_SIZE);
    }
    else
    {
        return m_Memory.at(address - ROM_SIZE);
    }

    return UINT8_MAX;
}

void MMU::write(u16 address, u8 val)
{
    if(address < ROM_ADDR)
    {
        if(address < BANK_SIZE) // ROM Bank Swapping
        {
            m_Rom.swapBank(val & 0x1F); //NOLINT(cppcoreguidelines-avoid-magic-numbers)
        }
    }
    else if(address < VRAM_ADDR)
    {
        m_Memory.at(address - ROM_SIZE) = val;
    }
    else if(address < RAM_BANK_ADDR)
    {
        m_Memory.at(address - ROM_SIZE) = val;
    }
    else if(address < INTERNAL_RAM_ADDR)
    {
        m_Memory.at(address - ROM_SIZE) = val;
    }
    else if(address < ECHO_RAM_ADDR)
    {
        m_Memory.at(address - ROM_SIZE - INTERNAL_RAM_SIZE) = val; // Map back into RAM
    }
    else if(address < OAM_ADDR)
    {
        m_Memory.at(address - ROM_SIZE) = val;
    }
    else if(address < UNUSABLE_ADDR)
    {
        
    }
    else if(address < IO_ADDR)
    {
        switch(address)
        {
            case DIV_REGISTER:
                m_Gameboy.resetDiv();
                break;
            case TAC_REGISTER:
                m_Memory.at(address - ROM_SIZE) = val;
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
                m_Memory.at(address - ROM_SIZE) = val;
        }
    }
    else
    {
        m_Memory.at(address - ROM_SIZE) = val;
    }
}