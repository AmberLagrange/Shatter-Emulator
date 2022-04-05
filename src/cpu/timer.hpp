#pragma once

#include "core.hpp"

class Gameboy;

class Timer
{
    public:
        Timer(Gameboy& gb);

        /**
         * @brief Updates the timer with the elapsed number of cycles
         * 
         * @param cycles The number of cycles since the last update
         */
        void update(u8 cycles);

        /**
         * @brief Reset the DIV counter to 0
         * 
         */
        inline void resetDiv() { m_DIV = 0; m_TIMA = 0; }

        /**
         * @brief Set the speed of the timer
         * 
         * @param speed The speed to be set
         */
        inline void setSpeed(u32 speed) { m_Speed = speed; }
    private:
        Gameboy& m_Gameboy;

        u16 m_DIV;
        u32 m_TIMA;
        u32 m_Speed;
};
