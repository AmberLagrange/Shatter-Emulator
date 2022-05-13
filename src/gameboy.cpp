#include "core.hpp"

#include "flags.hpp"
#include "gameboy.hpp"

Gameboy::Gameboy()
    :   m_APU(*this), m_CPU(*this), m_MMU(*this), m_PPU(*this),
        m_Timer(*this), m_Path(""), m_Running(false)
{
    m_PPU.setDrawCallback([screen = &m_Screen](std::array<u8, FRAME_BUFFER_SIZE> buffer) { screen->draw(buffer); });
}

Gameboy::Gameboy(const std::string& path)
    : Gameboy()
{
    load(path);
}

Gameboy::~Gameboy()
{
    save();
}

void Gameboy::load(const std::string& path)
{
    m_Path = path;
    m_MMU.load(m_Path);
}

void Gameboy::save()
{
    m_MMU.save(m_Path);
}

void Gameboy::start()
{
    DEBUG("Starting Gameboy.");
    m_Running = true;
}

void Gameboy::tick()
{
    u8 cycles = m_CPU.tick();
    m_CPU.handleInterrupts(cycles);
    m_Timer.update(cycles);
    m_PPU.tick(cycles);
}

void Gameboy::renderFrame()
{
    while(m_PPU.getMode() != VideoMode::VBlank)
    {
        tick();
    }

    while(m_PPU.getMode() == VideoMode::VBlank)
    {
        tick();
    }
}

void Gameboy::stop()
{
    DEBUG("Stopping Gameboy.");
    m_Running = false;
}

auto Gameboy::isRunning() const -> bool
{
    return m_Running;
}
