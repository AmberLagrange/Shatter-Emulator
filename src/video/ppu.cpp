#include "ppu.hpp"

#include "core.hpp"
#include "flags.hpp"
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
                m_Cycles -= CYCLES_PER_HBLANK;

                drawBackgroundLine(m_Line);
                drawWindowLine(m_Line);
                
                m_Line++;

                if (m_Line == GAMEBOY_HEIGHT)
                {
                    m_Mode = Mode::VBlank;
                    m_Gameboy.raiseInterrupt(Flags::Interrupt::VBlank);

                    u8 stat = m_Gameboy.read(STAT_REGISTER);

                    SET_BIT(stat, 0);
                    CLEAR_BIT(stat, 1);

                    // STAT interrupt checks OAM bit as well
                    if(GET_BIT(stat, STAT_VBLANK_BIT) || GET_BIT(stat, STAT_OAM_BIT))
                    {
                        m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                    }

                    m_Gameboy.write(STAT_REGISTER, stat);
                } else
                {
                    m_Mode = Mode::OAM_Scan;

                    u8 stat = m_Gameboy.read(STAT_REGISTER);

                    CLEAR_BIT(stat, 0);
                    SET_BIT(stat, 1);

                    if(GET_BIT(stat, STAT_OAM_BIT))
                    {
                        m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                    }

                    m_Gameboy.write(STAT_REGISTER, stat);
                }
            }
            break;
        case Mode::VBlank:
            if(m_Cycles >= CYCLES_PER_LINE)
            {
                m_Cycles -= CYCLES_PER_LINE;
                m_Line++;

                if (m_Line == VBLANK_HEIGHT)
                {
                    m_Mode = Mode::OAM_Scan;

                    drawSprites();
                    std::invoke(m_DrawCallback, m_FrameBuffer);
                    m_Line = 0;
                    
                    u8 stat = m_Gameboy.read(STAT_REGISTER);
                    
                    CLEAR_BIT(stat, 0);
                    SET_BIT(stat, 1);

                    if(GET_BIT(stat, STAT_OAM_BIT))
                    {
                        m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                    }

                    m_Gameboy.write(STAT_REGISTER, stat);
                };
            }
            break;
        case Mode::OAM_Scan:
            if (m_Cycles >= CYCLES_PER_OAM_SCAN)
            {
                m_Cycles -= CYCLES_PER_OAM_SCAN;

                m_Mode = Mode::Transfer;
                
                u8 stat = m_Gameboy.read(STAT_REGISTER);
                
                SET_BIT(stat, 0);
                SET_BIT(stat, 1);

                m_Gameboy.write(STAT_REGISTER, stat);
            }
            break;
        case Mode::Transfer:
            if (m_Cycles >= CYCLES_PER_TRANSFER)
            {
                m_Cycles -= CYCLES_PER_TRANSFER;

                m_Mode = Mode::HBlank;
                
                u8 stat = m_Gameboy.read(STAT_REGISTER);
                
                CLEAR_BIT(stat, 0);
                CLEAR_BIT(stat, 1);

                if(GET_BIT(stat, STAT_HBLANK_BIT))
                {
                    m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                }

                // LYC enabled
                if(GET_BIT(stat, STAT_LYC_BIT))
                {
                    u8 lyc = m_Gameboy.read(LYC_REGISTER);
                    if(m_Line == lyc)
                    {
                        m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                    }
                }

                m_Gameboy.write(STAT_REGISTER, stat);
            }
            break;
        default:
            ASSERT(false, "Invalid PPU Mode!");
    }

    m_Gameboy.write(LY_REGISTER, m_Line);
}

void PPU::drawBackgroundLine(u8 line)
{
    u8 lcdc = m_Gameboy.read(LCDC_REGISTER);

    u8 scrollX = m_Gameboy.read(SCX_REGISTER);
    u8 scrollY = m_Gameboy.read(SCY_REGISTER);

    u16 tileMapAddress  = GET_BIT(lcdc, 3) ? TILE_MAP_ZERO  : TILE_MAP_ONE;
    u16 tileDataAddress = GET_BIT(lcdc, 4) ? TILE_DATA_ZERO : TILE_DATA_ONE;

    bool usingTileDataZero = (tileDataAddress == TILE_DATA_ZERO);

    for(u8 col = 0; col < GAMEBOY_WIDTH; ++col)
    {
        // get the x and y position in the background
        u8 xPos = col  + scrollX;
        u8 yPos = line + scrollY;

        // get the x and y position of the tile within the background
        u8 tileXPos = xPos / BG_TILE_WIDTH;
        u8 tileYPos = yPos / BG_TILE_HEIGHT;

        // get the x and y position of the pixel within the tile
        u8 pixelXPos = xPos % BG_TILE_WIDTH;
        u8 pixelYPos = yPos % BG_TILE_HEIGHT;

        // get the tile index number and the tile within the tile map
        u16 tileIndex = tileXPos + tileYPos * BG_TILES_PER_LINE;
        u8  tileID    = m_Gameboy.read(tileMapAddress + tileIndex);

        // get the memory offset of the tile in the tile data
        // if we're using tile data one we need treat the offset as signed from 128
        u16 tileDataOffset = usingTileDataZero
                           ? (tileID                                   ) * BYTES_PER_TILE
                           : (static_cast<i8>(tileID) + TILE_ONE_OFFSET) * BYTES_PER_TILE;

        // add 2 bytes/line based on the y offset into the tile
        u16 tileAddress = tileDataAddress + tileDataOffset + 2 * pixelYPos;

        u8 low  = m_Gameboy.read(tileAddress    );
        u8 high = m_Gameboy.read(tileAddress + 1);

        // TODO: Colour pallets
        u8 bit = 7 - pixelXPos;
        u8 colour = GET_BIT(low, bit) | (GET_BIT(high, bit) << 1);
        
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

        m_FrameBuffer.at((col + line * GAMEBOY_WIDTH) * 4    ) = red;
        m_FrameBuffer.at((col + line * GAMEBOY_WIDTH) * 4 + 1) = green;
        m_FrameBuffer.at((col + line * GAMEBOY_WIDTH) * 4 + 2) = blue;
        m_FrameBuffer.at((col + line * GAMEBOY_WIDTH) * 4 + 3) = 0xFF;
    }
}

void PPU::drawWindowLine(u8 line)
{
    u8 lcdc = m_Gameboy.read(LCDC_REGISTER);

    if(!GET_BIT(lcdc, 5)) // Window not rendering
    {
        return;
    }

    u8 windowX = m_Gameboy.read(WX_REGISTER) - 7; // window x scroll has an offset of 7

    if(windowX >= GAMEBOY_WIDTH) // Don't render the window if it's to the right of the screen
    {
        return;
    }

    u8 windowY = m_Gameboy.read(WY_REGISTER);

    if(windowY >= GAMEBOY_HEIGHT || windowY > line) // Don't render the window if it's below the screen (or we're not at the scanline yet)
    {
        return;
    }

    u16 tileDataAddress = GET_BIT(lcdc, 4) ? TILE_DATA_ZERO : TILE_DATA_ONE;
    u16 tileMapAddress  = GET_BIT(lcdc, 6) ? TILE_MAP_ZERO  : TILE_MAP_ONE;

    bool usingTileDataZero = (tileDataAddress == TILE_DATA_ZERO);

    for(u8 col = 0; col < GAMEBOY_WIDTH; ++col)
    {
        // get the x and y position in the background
        u8 xPos = col  + windowX;
        u8 yPos = line + windowY;

        // get the x and y position of the tile within the background
        u8 tileXPos = xPos / BG_TILE_WIDTH;
        u8 tileYPos = yPos / BG_TILE_HEIGHT;

        // get the x and y position of the pixel within the tile
        u8 pixelXPos = xPos % BG_TILE_WIDTH;
        u8 pixelYPos = yPos % BG_TILE_HEIGHT;

        // get the tile index number and the tile within the tile map
        u16 tileIndex = tileXPos + tileYPos * BG_TILES_PER_LINE;
        u8  tileID    = m_Gameboy.read(tileMapAddress + tileIndex);

        // get the memory offset of the tile in the tile data
        // if we're using tile data one we need treat the offset as signed from 128
        u16 tileDataOffset = usingTileDataZero
                           ? (tileID                                   ) * BYTES_PER_TILE
                           : (static_cast<i8>(tileID) + TILE_ONE_OFFSET) * BYTES_PER_TILE;

        // add 2 bytes/line based on the y offset into the tile
        u16 tileAddress = tileDataAddress + tileDataOffset + 2 * pixelYPos;

        u8 low  = m_Gameboy.read(tileAddress    );
        u8 high = m_Gameboy.read(tileAddress + 1);

        // TODO: Colour pallets
        u8 bit = 7 - pixelXPos;
        u8 colour = GET_BIT(low, bit) | (GET_BIT(high, bit) << 1);
        
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

        m_FrameBuffer.at((col + line * GAMEBOY_WIDTH) * 4    ) = red;
        m_FrameBuffer.at((col + line * GAMEBOY_WIDTH) * 4 + 1) = green;
        m_FrameBuffer.at((col + line * GAMEBOY_WIDTH) * 4 + 2) = blue;
        m_FrameBuffer.at((col + line * GAMEBOY_WIDTH) * 4 + 3) = 0xFF;
    }
}

void PPU::drawSprites()
{

}
