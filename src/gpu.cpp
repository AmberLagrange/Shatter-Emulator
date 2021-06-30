#include "gpu.h"

GPU::GPU(Gameboy* gb)
    : m_Gameboy(gb)
{
    LOG("Initializing GPU!");
}