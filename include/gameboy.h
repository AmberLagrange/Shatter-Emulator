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

        u8 read(const u16& address);
        u8 write(const u16& address, const u8& val);
    private:
        CPU m_CPU;
        MMU m_MMU;
        GPU m_GPU;
        APU m_APU;

        friend Instruction;
};