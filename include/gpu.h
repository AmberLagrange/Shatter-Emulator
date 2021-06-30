#pragma once

#include "core.h"

class Gameboy;

class GPU
{
    public:
        GPU(Gameboy* gb);
    private:
        Gameboy* m_Gameboy;
};