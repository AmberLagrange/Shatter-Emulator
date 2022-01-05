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

        void load(const char* path);

        void start();
        void tick();
        void stop();

        inline CPU& getCPU() { return m_CPU; }
        inline MMU& getMMU() { return m_MMU; }
        inline PPU& getPPU() { return m_PPU; }
        inline APU& getAPU() { return m_APU; }

        inline bool isRunning() { return m_Running; }
    private:
        Screen m_Screen;

        CPU m_CPU;
        MMU m_MMU;
        PPU m_PPU;
        APU m_APU;

        Timer m_Timer;

        bool m_Running;
};