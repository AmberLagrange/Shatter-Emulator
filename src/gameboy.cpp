#include "core.hpp"

#include "flags.hpp"
#include "gameboy.hpp"

Gameboy::Gameboy()
    :   m_MMU(*this), m_APU(*this), m_CPU(*this), m_PPU(*this),
        m_Cycles(0), m_Timer(*this), m_Path(""), m_Running(false)
{
    m_PPU.setDrawCallback([screen = &m_Screen](std::array<u8, FRAME_BUFFER_SIZE> buffer) { screen->draw(buffer); });
}

void Gameboy::reset()
{
    m_CPU.reset();
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

void Gameboy::loadBoot(const std::string& path)
{
    m_MMU.loadBoot(path);
}

void Gameboy::save()
{
    m_MMU.save(m_Path);
}

void Gameboy::start()
{
    DEBUG("Starting Gameboy.");
    reset();
    m_Running = true;
    m_Screen.setTitleFPS(0);
}

void Gameboy::tick()
{
    u8 cycles = m_CPU.tick();
    m_CPU.handleInterrupts(cycles);
    m_Timer.update(cycles);
    m_PPU.tick(cycles);
    
    m_Cycles += cycles;
}

void Gameboy::renderFrame()
{
    while(m_Cycles <= CYCLES_PER_FRAME)
    {
        tick();
    }

    m_Cycles -= CYCLES_PER_FRAME;
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
