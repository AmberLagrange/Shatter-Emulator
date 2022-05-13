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
         * @brief Gets the value of the DIV register
         * 
         */
        auto getDIV() -> u8;

        /**
         * @brief Reset the DIV counter to 0
         * 
         */
        void resetDiv();

        /**
         * @brief Set the speed of the timer
         * 
         * @param speed The speed to be set
         */
        void setSpeed(u32 speed);
    private:
        Gameboy& m_Gameboy;

        u8  m_DIV;
        u32 m_Counter;
        u32 m_TIMA;
        u32 m_Speed;
};
