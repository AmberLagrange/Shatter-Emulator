#include "gameboy.h"

Gameboy::Gameboy()
    :   m_APU(*this), m_CPU(*this), m_MMU(*this), m_PPU(*this),
        m_Running(false)
{
    m_PPU.setDrawCallback(std::bind(&Screen::draw, &m_Screen, std::placeholders::_1));
}

Gameboy::Gameboy(const char* path)
    : Gameboy()
{
    load(path);
}

void Gameboy::load(const char* path)
{
    m_MMU.load(path);
}

void Gameboy::start()
{
    DEBUG("Starting Gameboy!");
    m_Running = true;
}

void Gameboy::tick()
{
    u8 cycles = m_CPU.tick();
    m_PPU.tick(cycles);
    m_Timer.update(cycles);
}

void Gameboy::stop()
{
    DEBUG("Stopping Gameboy!");
    m_Running = false;
}