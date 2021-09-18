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
    //      currently just hacking together
    //      a render loop to show the nintendo logo

    temp += cycles;

    if(temp >= 100000)
    {
        temp = 0;

        u8 scrollX = m_MMU->read(0xFF43);
        //u8 scrollY = m_MMU->read(0xFF42);
        u8 scrollY = 0;

        for(u16 row = 0; row < VRAM_WIDTH; ++row)
        {
            for(u16 col = 0; col < VRAM_HEIGHT; ++col)
            {
                u16 tileMap  = GET_BIT(m_MMU->read(0xFF40), 3) ? 0x9C00 : 0x9800;
                u16 tileData = GET_BIT(m_MMU->read(0xFF40), 4) ? 0x8000 : 0x8800;
                
                u16 xPos = col + scrollX;
                u16 yPos = row + scrollY;

                if(tileData == 0x8000)
                {
                    u8 tileIndex = m_MMU->read(tileMap + ((yPos / 8) * 32) + (xPos / 8));
                    tileData += tileIndex * 16;
                }
                else
                {
                    i8 tileIndex = m_MMU->read(tileMap + ((yPos / 8) * 32) + (xPos / 8));
                    tileData += (tileIndex + 0x80) * 16;
                }

                u8 low  = m_MMU->read(tileData + (yPos % 8 * 2)    );
                u8 high = m_MMU->read(tileData + (yPos % 8 * 2) + 1);

                u8 bit = 7 - (xPos % 8);
                u8 colour = GET_BIT(low, bit) | (GET_BIT(high, bit) << 1);

                u8 red, green, blue;

                if(colour == 0)
                {
                    red     = 0x9B;
                    green   = 0xBC;
                    blue    = 0x0F;
                }
                else if(colour == 1)
                {
                    red     = 0x30;
                    green   = 0x62;
                    blue    = 0x30;
                }
                else if(colour == 2)
                {
                    red     = 0x8B;
                    green   = 0xAC;
                    blue    = 0x0F;
                }
                else
                {
                    red     = 0x0F;
                    green   = 0x38;
                    blue    = 0x0F;
                }

                m_BackgroundBuffer[(row * VRAM_HEIGHT + col) * 4    ] = red;
                m_BackgroundBuffer[(row * VRAM_HEIGHT + col) * 4 + 1] = green;
                m_BackgroundBuffer[(row * VRAM_HEIGHT + col) * 4 + 2] = blue;
                m_BackgroundBuffer[(row * VRAM_HEIGHT + col) * 4 + 3] = 0xFF;
            }
        }

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

        m_MMU->write(0xFF44, 0x90);

        std::invoke(m_DrawCallback, m_FrameBuffer);
    }
}