#pragma once

#include "core.h"

#include "cartridge.h"

class Gameboy;

class MMU
{
    public:
        MMU(Gameboy& gb);

        void load(const char* path);

        u8 read(u16 address) const;
        void write(u16 address, u8 val);
    private:
        void reset();

    private:
        Gameboy& m_Gameboy;
        Cartridge m_Rom;

        u8 m_Memory[0x8000] = {};
};