#pragma once

#include "core.h"

#include "cartridge.h"

class Gameboy;
class CPU;

class MMU
{
    public:
        MMU();

        void load(const char* path);

        u8 read(const u16& address) const;
        void write(const u16& address, const u8& val);

        inline void setCPU(CPU* cpu) { m_CPU = cpu; }
        
    private:
        void reset();

    private:
        Cartridge m_Rom;
        CPU* m_CPU;

        u8 m_Memory[0x8000] = {};
};