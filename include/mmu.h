#pragma once

#include "core.h"

#include "cartridge.h"

class Gameboy;
class CPU;

class MMU
{
    public:
        MMU(Gameboy* gb);

        void load(const char* path);

        u8 read8(u16 address);
        u8 write8(u16 address, u8 val);

        u16 read16(u16 address);
        u16 write16(u16 address, u16 val);
    private:
        void reset();

        Gameboy* m_Gameboy;

        Cartridge m_Rom;
        u8 m_VRAM[0x2000];
};