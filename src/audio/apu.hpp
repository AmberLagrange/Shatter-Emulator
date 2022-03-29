#pragma once

#include "core.hpp"

class Gameboy;

class APU
{
    public:
        APU(Gameboy& gb);
        
    private:
        Gameboy& m_Gameboy [[maybe_unused]];
};