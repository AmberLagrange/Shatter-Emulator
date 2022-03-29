#pragma once

#include "core.hpp"

#include "audio/apu.hpp"

#include "mmu.hpp"

#include "cpu/cpu.hpp"
#include "cpu/timer.hpp"

#include "video/screen.hpp"
#include "video/ppu.hpp"

class Gameboy
{
    public:
        Gameboy();
        Gameboy(const char* path);

        void load(const char* path);

        void start();
        void tick();
        void stop();

        inline auto isRunning() -> bool { return m_Running; }

        [[nodiscard]]
        inline auto read(u16 address) const -> u8 { return m_MMU.read(address); }
        inline void write(u16 address, u8 val)    { m_MMU.write(address, val); }

        [[nodiscard]]
        inline auto getIME() const -> bool { return m_CPU.getIME(); }
        inline void setIME(bool ime)       { m_CPU.setIME(ime); }

        inline void raiseInterrupt(const Flags::Interrupt& flag) { m_CPU.raiseInterrupt(flag); }

        inline void resetDiv() { m_Timer.resetDiv(); }
        inline void setTimerSpeed(u32 speed) { m_Timer.setSpeed(speed); }

        [[nodiscard]]
        inline auto getWindowID() const -> u32 { return m_Screen.getWindowID(); }
    private:
        APU m_APU;
        CPU m_CPU;
        MMU m_MMU;
        PPU m_PPU;

        Timer m_Timer;

        Screen m_Screen;

        bool m_Running;
};