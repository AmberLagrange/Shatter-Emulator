#include "gameboy.h"

#include "screen.h"

Gameboy::Gameboy()
    : m_Running(false), m_Halted(false)
{
    m_CPU.setMMU(&m_MMU);
    m_MMU.setCPU(&m_CPU);

    m_PPU.setMMU(&m_MMU);
    m_PPU.setDrawCallback(std::bind(Screen::draw, std::placeholders::_1));

    Screen::setGameboy(this);
}
void Gameboy::load(const char* path)
{
    m_MMU.load(path);
}

void Gameboy::run()
{
    DEBUG("Starting Gameboy!");

    m_Running = true;

    while(m_Running)
    {
        u8 cycles = m_CPU.tick();
        m_PPU.tick(cycles);

        Screen::poll();
    }

    DEBUG("Stopping Gameboy!");
}

void Gameboy::stop()
{
    m_Running = false;
}