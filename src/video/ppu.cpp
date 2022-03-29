#include "ppu.hpp"

#include "gameboy.hpp"

PPU::PPU(Gameboy& gb)
    : m_Gameboy(gb)
{
    DEBUG("Initializing GPU!");
    temp = 0;
}

void PPU::tick(u8 cycles)
{
    temp -= cycles;

    if(temp <= 0)
    {
        //TODO: Proper ppu timing
        temp = 69905; //NOLINT(cppcoreguidelines-avoid-magic-numbers)

        u8 scrollX = m_Gameboy.read(SCX_REGISTER);
        u8 scrollY = 0; //m_Gameboy.read(SCY_REGISTER);

        for(u16 row = 0; row < VRAM_WIDTH; ++row)
        {
            for(u16 col = 0; col < VRAM_HEIGHT; ++col)
            {
                u16 tileMap  = GET_BIT(m_Gameboy.read(LCD_CTRL_REGISTER), 3) ? TILE_MAP_HIGH  : TILE_MAP_LOW;
                u16 tileData = GET_BIT(m_Gameboy.read(LCD_CTRL_REGISTER), 4) ? TILE_DATA_HIGH : TILE_DATA_LOW;
                
                u16 xPos = col + scrollX;
                u16 yPos = row + scrollY;

                if(tileData == TILE_DATA_HIGH)
                {
                    u8 tileIndex = m_Gameboy.read(tileMap + ((yPos / 8) * 32) + (xPos / 8));
                    tileData += tileIndex * 16;
                }
                else
                {
                    i8 tileIndex = m_Gameboy.read(tileMap + ((yPos / 8) * 32) + (xPos / 8));
                    tileData += (tileIndex + 0x80) * 16;
                }

                u8 low  = m_Gameboy.read(tileData + (yPos % 8 * 2)    );
                u8 high = m_Gameboy.read(tileData + (yPos % 8 * 2) + 1);

                u8 bit = 7 - (xPos % 8);
                u8 colour = GET_BIT(low, bit) | (GET_BIT(high, bit) << 1);

                // TODO: Colour pallets
                u8 red, green, blue; // NOLINT(cppcoreguidelines-init-variables)

                if(colour == 0)
                {
                    red     = 0x9B; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                    green   = 0xBC; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                    blue    = 0x0F; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                }
                else if(colour == 1)
                {
                    red     = 0x30; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                    green   = 0x62; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                    blue    = 0x30; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                }
                else if(colour == 2)
                {
                    red     = 0x8B; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                    green   = 0xAC; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                    blue    = 0x0F; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                }
                else
                {
                    red     = 0x0F; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                    green   = 0x38; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                    blue    = 0x0F; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                }

                m_BackgroundBuffer.at((row * VRAM_HEIGHT + col) * 4    ) = red;
                m_BackgroundBuffer.at((row * VRAM_HEIGHT + col) * 4 + 1) = green;
                m_BackgroundBuffer.at((row * VRAM_HEIGHT + col) * 4 + 2) = blue;
                m_BackgroundBuffer.at((row * VRAM_HEIGHT + col) * 4 + 3) = 0xFF;
            }
        }

        for(u32 row = 0; row < GAMEBOY_HEIGHT; ++row)
        {
            for(u32 col = 0; col < GAMEBOY_WIDTH; ++col)
            {
                m_FrameBuffer.at((row * GAMEBOY_WIDTH + col) * 4    ) = m_BackgroundBuffer.at((row * VRAM_WIDTH + col) * 4    );
                m_FrameBuffer.at((row * GAMEBOY_WIDTH + col) * 4 + 1) = m_BackgroundBuffer.at((row * VRAM_WIDTH + col) * 4 + 1);
                m_FrameBuffer.at((row * GAMEBOY_WIDTH + col) * 4 + 2) = m_BackgroundBuffer.at((row * VRAM_WIDTH + col) * 4 + 2);
                m_FrameBuffer.at((row * GAMEBOY_WIDTH + col) * 4 + 3) = m_BackgroundBuffer.at((row * VRAM_WIDTH + col) * 4 + 3);
            }
        }

        std::invoke(m_DrawCallback, m_FrameBuffer);
    }
}
