#include "apu.hpp"

APU::APU(Gameboy& gb)
    : m_Gameboy(gb)
{
    DEBUG("Initializing APU!");
}
