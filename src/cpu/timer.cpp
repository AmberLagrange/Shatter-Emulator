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
        m_Gameboy.write(DIV_REGISTER, m_Gameboy.read(DIV_REGISTER) + 1);
    }

    if(bit_functions::get_bit(m_Gameboy.read(TAC_REGISTER), 2))
    {
        m_TIMA += cycles;

        while(m_TIMA >= m_Speed)
        {
            m_TIMA -= m_Speed;
            u8 tima = m_Gameboy.read(TIMA_REGISTER);
            
            if(tima == 0xFF)
            {
                tima = m_Gameboy.read(TMA_REGISTER);
                m_Gameboy.raiseInterrupt(Flags::Interrupt::Timer);
            }
            else
            {
                tima++;
            }

            m_Gameboy.write(TIMA_REGISTER, tima);
        }
    }
}
