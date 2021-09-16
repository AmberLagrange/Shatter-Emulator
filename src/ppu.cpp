#include "ppu.h"

PPU::PPU()
{
    LOG("Initializing GPU!");

    temp = 0;
}

void PPU::tick(u8 cycles)
{
    //TODO: calculate when to render

    temp++;

    if(temp == 1000)
    {
        temp = 0;
        std::invoke(m_DrawCallback);
    }
}