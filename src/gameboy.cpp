#include "core.hpp"

#include "gameboy.hpp"

Gameboy::Gameboy()
    :   m_APU(*this), m_CPU(*this), m_MMU(*this), m_PPU(*this),
        m_Timer(*this), m_Running(false)
{
    m_PPU.setDrawCallback([screen = &m_Screen](std::array<u8, FRAME_BUFFER_SIZE> buffer) { screen->draw(buffer); });
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
    m_CPU.handleInterrupts(cycles);
    m_Timer.update(cycles);
    m_PPU.tick(cycles);
}

void Gameboy::stop()
{
    DEBUG("Stopping Gameboy!");
    m_Running = false;
}

auto Gameboy::isRunning() const -> bool
{
    return m_Running;
}

auto Gameboy::read(u16 address) const -> u8
{
    return m_MMU.read(address);
}

void Gameboy::write(u16 address, u8 val)
{
    m_MMU.write(address, val);
}

auto Gameboy::getIME() const -> bool
{
    return m_CPU.getIME();
}

void Gameboy::setIME(bool ime)
{
    m_CPU.setIME(ime);
}

void Gameboy::raiseInterrupt(const Flags::Interrupt& flag)
{
    m_CPU.raiseInterrupt(flag);
}

void Gameboy::resetDiv()
{
    m_Timer.resetDiv();
}

void Gameboy::setTimerSpeed(u32 speed)
{
    m_Timer.setSpeed(speed);
}

auto Gameboy::getWindowID() const -> u32
{
    return m_Screen.getWindowID();
}
