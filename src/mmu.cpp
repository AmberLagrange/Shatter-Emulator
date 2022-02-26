#include "mmu.h"
#include "gameboy.h"

MMU::MMU(Gameboy& gb)
    : m_Gameboy(gb)
{
    DEBUG("Initializing MMU!");
}

void MMU::load(const char* path)
{
    m_Rom.load(path);
}

u8 MMU::read(u16 address) const
{
    if(address < 0x8000) // ROM
    {
        return m_Rom.read(address);
    }
    else if(address < 0xA000) //VRAM
    {
        return m_Memory[address - 0x8000];
    }
    else if(address < 0xC000) // RAM Bank
    {
        return m_Memory[address - 0x8000];
    }
    else if(address < 0xE000) // Internal RAM
    {
        return m_Memory[address - 0x8000];
    }
    else if(address < 0xFE00) // Echo of Internal RAM
    {
        return m_Memory[address - 0x8000 - 0x2000]; // Map back into RAM
    }
    else if(address < 0xFEA0) // OAM
    {
        return m_Memory[address - 0x8000];
    }
    else if(address < 0xFF00) // Unusable
    {
        return 0xFF;
    }
    else if(address < 0xFF80) // IO
    {
        return m_Memory[address - 0x8000];
    }
    else
    {
        return m_Memory[address - 0x8000];
    }

    return 0xFF;
}

void MMU::write(u16 address, u8 val)
{
    if(address < 0x8000) // ROM
    {
        if(address < 0x4000) // ROM Bank Swapping
        {
            m_Rom.swapBank(val & 0x1F);
        }
    }
    else if(address < 0xA000) //VRAM
    {
        m_Memory[address - 0x8000] = val;
    }
    else if(address < 0xC000) // RAM Bank
    {
        m_Memory[address - 0x8000] = val;
    }
    else if(address < 0xE000) // Internal RAM
    {
        m_Memory[address - 0x8000] = val;
    }
    else if(address < 0xFE00) // Echo of Internal RAM
    {
        m_Memory[address - 0x8000 - 0x2000] = val; // Map back into RAM
    }
    else if(address < 0xFEA0) // OAM
    {
        m_Memory[address - 0x8000] = val;
    }
    else if(address < 0xFF00) // Unusable
    {
        
    }
    else if(address < 0xFF80) // IO
    {
        switch(address)
        {
            case DIV_REGISTER:
                m_Gameboy.resetDiv();
                break;
            case TAC_REGISTER:
                m_Memory[address - 0x8000] = val;
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
                m_Memory[address - 0x8000] = val;
        }
    }
    else
    {
        m_Memory[address - 0x8000] = val;
    }
}