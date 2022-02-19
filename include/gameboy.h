#pragma once

#include "core.h"

#include "screen.h"

#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"

#include "timer.h"

class Gameboy
{
    public:
        Gameboy();
        Gameboy(const char* path);

        void load(const char* path);

        void start();
        void tick();
        void stop();

        inline bool isRunning() { return m_Running; }

        inline u8 read(u16 address) const { return m_MMU.read(address); }
        inline void write(u16 address, u8 val) { m_MMU.write(address, val); }

        inline bool getIME() { return m_CPU.getIME(); }
        inline void setIME(bool ime) { m_CPU.setIME(ime); }

        inline u32 getWindowID() const { return m_Screen.getWindowID(); }
    private:
        APU m_APU;
        CPU m_CPU;
        MMU m_MMU;
        PPU m_PPU;

        Screen m_Screen;

        Timer m_Timer;

        bool m_Running;
};