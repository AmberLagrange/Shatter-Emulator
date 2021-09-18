#include "ppu.h"

#include "mmu.h"

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

        for(u16 row = 0; row < VRAM_WIDTH; ++row)
        {
            for(u16 col = 0; col < VRAM_HEIGHT; ++col)
            {
                m_BackgroundBuffer[(row * VRAM_HEIGHT + col) * 4    ] = 0x41;
                m_BackgroundBuffer[(row * VRAM_HEIGHT + col) * 4 + 1] = 0x69;
                m_BackgroundBuffer[(row * VRAM_HEIGHT + col) * 4 + 2] = 0xE1;
                m_BackgroundBuffer[(row * VRAM_HEIGHT + col) * 4 + 3] = 0xFF;
            }
        }

        //u16 tileMapLocation  = ((m_MMU->read(0xFF40) >> 3) & 1) ? 0x9C00 : 0x9800;
        //u16 tileDataLocation = ((m_MMU->read(0xFF40) >> 4) & 1) ? 0x8000 : 0x8800;

        for(u8 row = 0; row < GAMEBOY_HEIGHT; ++row)
        {
            for(u8 col = 0; col < GAMEBOY_WIDTH; ++col)
            {
                m_FrameBuffer[(row * GAMEBOY_WIDTH + col) * 4    ] = m_BackgroundBuffer[(row * VRAM_WIDTH + col) * 4    ];
                m_FrameBuffer[(row * GAMEBOY_WIDTH + col) * 4 + 1] = m_BackgroundBuffer[(row * VRAM_WIDTH + col) * 4 + 1];
                m_FrameBuffer[(row * GAMEBOY_WIDTH + col) * 4 + 2] = m_BackgroundBuffer[(row * VRAM_WIDTH + col) * 4 + 2];
                m_FrameBuffer[(row * GAMEBOY_WIDTH + col) * 4 + 3] = m_BackgroundBuffer[(row * VRAM_WIDTH + col) * 4 + 3];
            }
        }

        std::invoke(m_DrawCallback, m_FrameBuffer);
    }
}