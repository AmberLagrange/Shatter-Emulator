#include "gameboy.h"

Gameboy::Gameboy()
    : m_CPU(this), m_MMU(this), m_GPU(this), m_APU(this)
{
    load("./roms/tetris.gb");
}

void Gameboy::load(const char* path)
{
    m_MMU.load(path);
}

void Gameboy::start()
{
    for(int i = 0; i < 0x200; ++i)
    {
        m_CPU.tick();
    }
}

u8 Gameboy::read8(u16 address)
{
    return m_MMU.read8(address);
}

u8 Gameboy::write8(u16 address, u8 val)
{
    return m_MMU.write8(address, val);
}

u16 Gameboy::read16(u16 address)
{
    return m_MMU.read16(address);
}

u16 Gameboy::write16(u16 address, u16 val)
{
    return m_MMU.write16(address, val);
}