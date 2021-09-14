#pragma once

#include "core.h"

class Gameboy;

class PPU
{
    public:
        PPU(Gameboy& gb);

        void tick();
    private:
        Gameboy& m_Gameboy;
};