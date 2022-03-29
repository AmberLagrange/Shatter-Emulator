#include "timer.hpp"

#include "gameboy.hpp"
#include "flags.hpp"

Timer::Timer(Gameboy& gb)
    : m_Gameboy(gb), m_DIV(0), m_TIMA(0), m_Speed(TIMER_SPEED_00) {}

void Timer::update(u8 cycles)
{
    m_DIV += cycles;
    
    if(GET_BIT(m_Gameboy.read(TAC_REGISTER), 2)) // Check if the timer is enabled
    {
        m_TIMA += cycles;
        while(m_TIMA >= m_Speed)
        {
            m_TIMA -= m_Speed;

            if(m_Gameboy.read(TIMA_REGISTER) == UINT8_MAX)
            {
                m_Gameboy.write(TIMA_REGISTER, m_Gameboy.read(TMA_REGISTER));
                m_Gameboy.raiseInterrupt(Flags::Interrupt::Timer);
            }
            else
            {
                m_Gameboy.write(TIMA_REGISTER, m_Gameboy.read(TIMA_REGISTER) + 1);
            }
        }
        
    }

    if(m_DIV >= 0x0100)
    {
        m_DIV -= 0x0100;
        m_Gameboy.write(DIV_REGISTER, m_Gameboy.read(DIV_REGISTER) + 1);
    }
}
