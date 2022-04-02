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

        auto isRunning() const -> bool;

        [[nodiscard]]auto read(u16 address) const -> u8;

        void write(u16 address, u8 val);

        [[nodiscard]] auto getIME() const -> bool;
        void setIME(bool ime);

        void raiseInterrupt(const Flags::Interrupt& flag);

        void resetDiv();
        void setTimerSpeed(u32 speed);

        [[nodiscard]]
        auto getWindowID() const -> u32;
    private:
        APU m_APU;
        CPU m_CPU;
        MMU m_MMU;
        PPU m_PPU;

        Timer m_Timer;

        Screen m_Screen;

        bool m_Running;
};
