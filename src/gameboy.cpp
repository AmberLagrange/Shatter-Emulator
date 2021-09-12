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
    LOG("Staring Gameboy\n");

    for(int i = 0; i < 0x200; ++i)
    {
        m_CPU.tick();
    }
}

u8 Gameboy::read(u16 address)
{
    return m_MMU.read(address);
}

u8 Gameboy::write(u16 address, u8 val)
{
    return m_MMU.write(address, val);
}