#include "mmu.h"

MMU::MMU(Gameboy* gb)
    : m_Gameboy(gb)
{
    LOG("Initializing MMU!");
}

void MMU::load(const char* path)
{
    m_Rom.load(path);
}

u8 MMU::read8(u16 address)
{
    if(address < 0x8000) return m_Rom.read8(address);
    return 0xab;
}

u8 MMU::write8(u16 address, u8 val)
{
    if(address < 0x8000) return 0x00;
    return val;
}

u16 MMU::read16(u16 address)
{
    if(address < 0x8000) return m_Rom.read16(address);
    return 0xabab;
}

u16 MMU::write16(u16 address, u16 val)
{
    if(address < 0x8000) return 0x0000;
    return val;
}