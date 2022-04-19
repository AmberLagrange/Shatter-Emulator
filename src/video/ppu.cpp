#include "ppu.hpp"

#include "core.hpp"
#include "gameboy.hpp"

PPU::PPU(Gameboy& gb)
    : m_Gameboy(gb), m_Mode(Mode::OAM_Scan), m_Cycles(0)
{
    DEBUG("Initializing GPU!");
}

void PPU::setDrawCallback(std::function<void(std::array<u8, FRAME_BUFFER_SIZE> buffer)> callback)
{
    m_DrawCallback = callback;
}

void PPU::tick(u8 cycles)
{
    m_Cycles += cycles;

    switch(m_Mode)
    {
        case Mode::HBlank:
            if(m_Cycles >= CYCLES_PER_HBLANK)
            {
                drawBackgroundLine(m_Line);
                drawWindowLine(m_Line);
                m_Cycles -= CYCLES_PER_HBLANK;
                m_Line++;

                if(m_Line == GAMEBOY_HEIGHT)
                {
                    m_Mode = Mode::VBlank;
                }
            }
            break;
        case Mode::VBlank:
            if(m_Cycles >= CYCLES_PER_LINE)
            {
                m_Cycles -= CYCLES_PER_LINE;
                m_Line++;

                if(m_Line == VBLANK_HEIGHT)
                {
                    drawSprites();

                    std::invoke(m_DrawCallback, m_FrameBuffer);
                    m_Line = 0;
                    m_Mode = Mode::OAM_Scan;
                }
            }
            break;
        case Mode::OAM_Scan:
            if(m_Cycles >= CYCLES_PER_OAM_SCAN)
            {
                m_Cycles -= CYCLES_PER_OAM_SCAN;
                m_Mode = Mode::Drawing;
            }
            break;
        case Mode::Drawing:
            if(m_Cycles >= CYCLES_PER_DRAWING)
            {
                m_Cycles -= CYCLES_PER_DRAWING;
                m_Mode = Mode::HBlank;
            }
            break;
        default:
            ASSERT(false, "Invalid PPU Mode!");
    }
}

void PPU::drawBackgroundLine(u8 line)
{
    u8 scrollX = m_Gameboy.read(SCX_REGISTER);
    u8 scrollY = m_Gameboy.read(SCY_REGISTER);

    for(u16 col = 0; col < GAMEBOY_WIDTH; ++col)
    {
        u16 tileMap  = GET_BIT(m_Gameboy.read(LCD_CTRL_REGISTER), 3) ? TILE_MAP_HIGH  : TILE_MAP_LOW;
        u16 tileData = GET_BIT(m_Gameboy.read(LCD_CTRL_REGISTER), 4) ? TILE_DATA_HIGH : TILE_DATA_LOW;
                
        u16 xPos = (col  + scrollX) % VRAM_WIDTH;
        u16 yPos = (line + scrollY) % VRAM_HEIGHT;

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

        u8 low  = m_Gameboy.read(tileData + ((yPos % 8) * 2)    );
        u8 high = m_Gameboy.read(tileData + ((yPos % 8) * 2) + 1);

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

        m_FrameBuffer.at((line * GAMEBOY_WIDTH + col) * 4    ) = red;
        m_FrameBuffer.at((line * GAMEBOY_WIDTH + col) * 4 + 1) = green;
        m_FrameBuffer.at((line * GAMEBOY_WIDTH + col) * 4 + 2) = blue;
        m_FrameBuffer.at((line * GAMEBOY_WIDTH + col) * 4 + 3) = 0xFF;
    }
}

void PPU::drawWindowLine(u8 line)
{

}

void PPU::drawSprites()
{

}
