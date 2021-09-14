#pragma once

#include "core.h"

#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "screen.h"

#include "cartridge.h"
#include "instruction.h"

class Gameboy
{
    public:
        Gameboy();

        void load(const char* path);
        void start();
        void stop();

        u8 read(const u16& address);
        u8 write(const u16& address, const u8& val);
    private:
        CPU m_CPU;
        MMU m_MMU;
        PPU m_PPU;
        APU m_APU;
        Screen m_Screen;

        bool m_Running;
        bool m_Halted;

        friend Instruction;
};