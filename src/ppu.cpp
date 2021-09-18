#include "ppu.h"

PPU::PPU()
{
    DEBUG("Initializing GPU!");

    temp = 0;
}

void PPU::tick(u8 cycles)
{
    //TODO: calculate when to render

    temp++;

    if(temp == 1000)
    {
        temp = 0;

        for(u8 row = 0; row < GAMEBOY_HEIGHT; ++row)
        {
            for(u8 col = 0; col < GAMEBOY_WIDTH; ++col)
            {
                m_FrameBuffer[(row * GAMEBOY_WIDTH * 4) + (col * 4)    ] = 0x41;
                m_FrameBuffer[(row * GAMEBOY_WIDTH * 4) + (col * 4) + 1] = 0x69;
                m_FrameBuffer[(row * GAMEBOY_WIDTH * 4) + (col * 4) + 2] = 0xE1;
                m_FrameBuffer[(row * GAMEBOY_WIDTH * 4) + (col * 4) + 3] = 0xFF;
            }
        }

        std::invoke(m_DrawCallback, m_FrameBuffer);
    }
}