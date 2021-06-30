#include "apu.h"

APU::APU(Gameboy* gb)
    : m_Gameboy(gb)
{
    LOG("Initializing APU!");
}