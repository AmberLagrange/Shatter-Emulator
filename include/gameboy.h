#pragma once

#include "core.h"

#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"

#include "cartridge.h"
#include "instruction.h"

class Gameboy
{
    public:
        Gameboy();

        void load(const char* path);
        void run();
        void stop();

        inline CPU& getCPU() { return m_CPU; }
        inline MMU& getMMU() { return m_MMU; }
        inline PPU& getPPU() { return m_PPU; }
        inline APU& getAPU() { return m_APU; }
    private:
        CPU m_CPU;
        MMU m_MMU;
        PPU m_PPU;
        APU m_APU;

        bool m_Running;
        bool m_Halted;
};