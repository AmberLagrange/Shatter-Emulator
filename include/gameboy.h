#pragma once

#include "core.h"

#include "mmu.h"
#include "cpu.h"
#include "gpu.h"
#include "apu.h"

#include "cartridge.h"
#include "instruction.h"

class Gameboy
{
    public:
        Gameboy();

        void load(const char* path);
        void start();

        u8 read8(u16 address);
        u8 write8(u16 address, u8 val);

        u16 read16(u16 address);
        u16 write16(u16 address, u16 val);
    private:
        CPU m_CPU;
        MMU m_MMU;
        GPU m_GPU;
        APU m_APU;

        friend Instruction;
};