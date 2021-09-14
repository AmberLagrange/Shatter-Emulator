#pragma once

#include "core.h"

class Gameboy;

class APU
{
    public:
        APU(Gameboy& gb);
    private:
        Gameboy& m_Gameboy;
};