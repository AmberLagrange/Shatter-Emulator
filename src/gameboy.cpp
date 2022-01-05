#include "gameboy.h"

#include "screen.h"

Gameboy::Gameboy()
    : m_Running(false)
{
    m_Screen.setGameboy(this);
    m_CPU.setGameboy(this);
    m_MMU.setGameboy(this);
    m_PPU.setGameboy(this);
    m_PPU.setDrawCallback(std::bind(&Screen::draw, &m_Screen, std::placeholders::_1));
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
    m_Screen.poll();
}

void Gameboy::stop()
{
    DEBUG("Stopping Gameboy!");
    m_Running = false;
}