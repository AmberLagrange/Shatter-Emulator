#include "gameboy.h"

Gameboy::Gameboy()
    : m_Screen(*this), m_Running(false), m_Halted(false)
{
    m_CPU.setMMU(&m_MMU);
    m_MMU.setCPU(&m_CPU);
}

void Gameboy::load(const char* path)
{
    m_MMU.load(path);
}

void Gameboy::start()
{
    LOG("Starting Gameboy\n");

    m_Running = true;

    while(m_Running)
    {
        u8 cycles = m_CPU.tick();
        m_PPU.tick(cycles);

        //m_Screen.update();
    }
}

void Gameboy::stop()
{
    m_Running = false;
}

u8 Gameboy::read(const u16& address)
{
    return m_MMU.read(address);
}

u8 Gameboy::write(const u16& address, const u8& val)
{
    return m_MMU.write(address, val);
}