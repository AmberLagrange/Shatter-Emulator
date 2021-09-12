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

u8 MMU::read(const u16& address)
{
    if(address < 0x8000) return m_Rom.read8(address);
    if(address < 0xA000) return m_VRAM[address - 0x8000];
    if(address < 0xC000) return m_RAMBANK[address - 0xA000];
    if(address < 0xE000) return m_RAM[address - 0xC000];
    return 0x00;
}

u8 MMU::write(const u16& address, const u8& val)
{
    if(address < 0x8000) return 0x00;
    else if(address < 0xA000) m_VRAM[address - 0x8000] = val;
    else if(address < 0xC000) m_RAMBANK[address - 0xA000] = val;
    else if(address < 0xE000) m_RAM[address - 0xC000] = val;
    return val;
}