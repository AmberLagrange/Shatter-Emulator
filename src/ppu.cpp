#include "ppu.h"

PPU::PPU()
{
    LOG("Initializing GPU!");
}

void PPU::tick(u8 cycles)
{
    //TODO: calculate when to render

    std::invoke(m_DrawCallback);
}