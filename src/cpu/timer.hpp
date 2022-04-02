#pragma once

#include "core.hpp"

class Gameboy;

class Timer
{
    public:
        Timer(Gameboy& gb);

        void update(u8 cycles);
        inline void resetDiv() { m_DIV = 0; m_TIMA = 0; }

        inline void setSpeed(u32 speed) { m_Speed = speed; }
    private:
        Gameboy& m_Gameboy;

        u16 m_DIV;
        u32 m_TIMA;
        u32 m_Speed;
};
