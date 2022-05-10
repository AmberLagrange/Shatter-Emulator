#include "core.hpp"

#include "timer.hpp"

#include "core.hpp"
#include "flags.hpp"

#include "gameboy.hpp"

Timer::Timer(Gameboy& gb)
    : m_Gameboy(gb), m_DIV(0), m_TIMA(0), m_Speed(TIMER_SPEED_00) {}

void Timer::update(u8 cycles)
{
    m_DIV += cycles;

    while(m_DIV >= 0x0100)
    {
        m_DIV -= 0x0100;
        m_Gameboy.write(TIMER_DIV_REGISTER, m_Gameboy.read(TIMER_DIV_REGISTER) + 1);
    }

    if(bit_functions::get_bit(m_Gameboy.read(TIMER_TAC_REGISTER), 2))
    {
        m_TIMA += cycles;

        while(m_TIMA >= m_Speed)
        {
            m_TIMA -= m_Speed;
            u8 tima = m_Gameboy.read(TIMER_TIMA_REGISTER);
            
            if(tima == 0xFF)
            {
                tima = m_Gameboy.read(TIMER_TMA_REGISTER);
                m_Gameboy.raiseInterrupt(Flags::Interrupt::Timer);
            }
            else
            {
                tima++;
            }

            m_Gameboy.write(TIMER_TIMA_REGISTER, tima);
        }
    }
}
