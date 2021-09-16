#include "gameboy.h"

#include "screen.h"

Gameboy::Gameboy()
    : m_Running(false), m_Halted(false)
{
    m_CPU.setMMU(&m_MMU);
    m_MMU.setCPU(&m_CPU);
    m_PPU.setDrawCallback(std::bind(Screen::draw));
}

void Gameboy::load(const char* path)
{
    m_MMU.load(path);
}

void Gameboy::run()
{
    LOG("Starting Gameboy\n");

    m_Running = true;

    Screen::initScreen();
    Screen::setGameboy(this);

    while(m_Running)
    {
        u8 cycles = m_CPU.tick();
        m_PPU.tick(cycles);

        Screen::poll();
    }
    
    Screen::destroyScreen();
}

void Gameboy::stop()
{
    m_Running = false;
}