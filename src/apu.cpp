#include "apu.h"

APU::APU(Gameboy& gb)
    : m_Gameboy(gb)
{
    DEBUG("Initializing APU!");
}