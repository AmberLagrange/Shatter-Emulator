#include "core.hpp"

#include "timer.hpp"

#include "core.hpp"
#include "flags.hpp"

#include "gameboy.hpp"

Timer::Timer(Gameboy& gb)
    : m_Gameboy(gb), m_DIV(0), m_Counter(0), m_TIMA(0), m_Speed(TIMER_SPEED_00) {}

void Timer::update(u8 cycles)
{
    m_Counter += cycles;

    while(m_Counter >= 0x0100)
    {
        m_Counter -= 0x0100;
        m_DIV += 1;
    }

    u8 tac = m_Gameboy.read(TIMER_TAC_REGISTER);

    if(bit_functions::get_bit(tac, 2))
    {
        m_TIMA += cycles;

        while (m_TIMA >= m_Speed)
        {
            m_TIMA -= m_Speed;
            u8 tima = m_Gameboy.read(TIMER_TIMA_REGISTER);

            if (tima == 0xFF)
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

auto Timer::getDIV() -> u8
{
    return m_DIV;
}

void Timer::resetDiv()
{
    m_DIV = 0;
    m_TIMA = 0;
}

void Timer::setSpeed(u32 speed)
{
    m_Speed = speed;
}
