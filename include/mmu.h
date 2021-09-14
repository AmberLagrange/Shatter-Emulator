#pragma once

#include "core.h"

#include "cartridge.h"

class Gameboy;
class CPU;

class MMU
{
    public:
        MMU(Gameboy& gb);

        void load(const char* path);

        u8 read(const u16& address);
        u8 write(const u16& address, const u8& val);
    private:
        void reset();

        Gameboy& m_Gameboy;
        
        Cartridge m_Rom;

        u8 m_VRAM[0x2000];
        u8 m_RAMBANK[0x2000];
        u8 m_RAM[0x2000];
};