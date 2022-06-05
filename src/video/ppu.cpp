#include "core.hpp"

#include "ppu.hpp"

#include "flags.hpp"
#include "gameboy.hpp"
#include "video/video_defs.hpp"

PPU::PPU(Gameboy& gb)
    : m_Gameboy(gb), m_Mode(VideoMode::OAM_Scan), m_Cycles(0), m_Line(0)
{
    DEBUG("Initializing GPU.");
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
        case VideoMode::HBlank: // Mode 0
            if(m_Cycles >= CYCLES_PER_HBLANK)
            {
                m_Cycles -= CYCLES_PER_HBLANK;

                drawBackgroundLine(m_Line);
                drawWindowLine(m_Line);
                drawSprites(m_Line);
                
                m_Line++;

                if(m_Line == SCREEN_HEIGHT)
                {
                    m_Mode = VideoMode::VBlank;
                    m_Gameboy.raiseInterrupt(Flags::Interrupt::VBlank);

                    u8 stat = m_Gameboy.read(LCD_STAT_REGISTER);

                    bit_functions::set_bit_to(stat, 0, 1);
                    bit_functions::set_bit_to(stat, 1, 0);

                    // STAT interrupt checks OAM bit as well
                    if(bit_functions::get_bit(stat, STAT_VBLANK_BIT) || bit_functions::get_bit(stat, STAT_OAM_BIT))
                    {
                        m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                    }

                    m_Gameboy.write(LCD_STAT_REGISTER, stat);
                }
                else
                {
                    m_Mode = VideoMode::OAM_Scan;

                    u8 stat = m_Gameboy.read(LCD_STAT_REGISTER);

                    bit_functions::set_bit_to(stat, 0, 0);
                    bit_functions::set_bit_to(stat, 1, 1);

                    if(bit_functions::get_bit(stat, STAT_OAM_BIT))
                    {
                        m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                    }

                    m_Gameboy.write(LCD_STAT_REGISTER, stat);
                }
            }
            break;
        case VideoMode::VBlank: // Mode 1
            if(m_Cycles >= CYCLES_PER_LINE)
            {
                m_Cycles -= CYCLES_PER_LINE;
                m_Line++;

                if(m_Line == VBLANK_HEIGHT)
                {
                    m_Mode = VideoMode::OAM_Scan;

                    std::invoke(m_DrawCallback, m_FrameBuffer);
                    m_Line = 0;
                    
                    u8 stat = m_Gameboy.read(LCD_STAT_REGISTER);
                    
                    bit_functions::set_bit_to(stat, 0, 0);
                    bit_functions::set_bit_to(stat, 1, 1);

                    if(bit_functions::get_bit(stat, STAT_OAM_BIT))
                    {
                        m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                    }

                    m_Gameboy.write(LCD_STAT_REGISTER, stat);
                };
            }
            break;
        case VideoMode::OAM_Scan: // Mode 2
            if (m_Cycles >= CYCLES_PER_OAM_SCAN)
            {
                m_Cycles -= CYCLES_PER_OAM_SCAN;

                m_Mode = VideoMode::Transfer;
                
                u8 stat = m_Gameboy.read(LCD_STAT_REGISTER);
                
                bit_functions::set_bit_to(stat, 0, 1);
                bit_functions::set_bit_to(stat, 1, 1);

                m_Gameboy.write(LCD_STAT_REGISTER, stat);
            }
            break;
        case VideoMode::Transfer: // Mode 3
            if(m_Cycles >= CYCLES_PER_TRANSFER)
            {
                m_Cycles -= CYCLES_PER_TRANSFER;

                m_Mode = VideoMode::HBlank;
                
                u8 stat = m_Gameboy.read(LCD_STAT_REGISTER);
                
                bit_functions::set_bit_to(stat, 0, 0);
                bit_functions::set_bit_to(stat, 1, 0);

                if(bit_functions::get_bit(stat, STAT_HBLANK_BIT))
                {
                    m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                }

                // LYC enabled
                if(bit_functions::get_bit(stat, STAT_LYC_BIT))
                {
                    u8 lyc = m_Gameboy.read(LYC_REGISTER);
                    if(m_Line == lyc)
                    {
                        bit_functions::set_bit(stat, STAT_LCY_LY_BIT);
                        m_Gameboy.raiseInterrupt(Flags::Interrupt::LCD_STAT);
                    }
                    else
                    {
                        bit_functions::clear_bit(stat, STAT_LCY_LY_BIT);
                    }
                }

                m_Gameboy.write(LCD_STAT_REGISTER, stat);
            }
            break;
        default:
            ASSERT(false, "Invalid PPU Mode!");
    }

    m_Gameboy.write(LY_REGISTER, m_Line);
}

auto PPU::getMode() const -> VideoMode
{
    return m_Mode;
}

void PPU::drawBackgroundLine(u8 line)
{
    u8 lcdc = m_Gameboy.read(LCD_CONTROL_REGISTER);

    u8 scrollX = m_Gameboy.read(SCX_REGISTER);
    u8 scrollY = m_Gameboy.read(SCY_REGISTER);

    u16 tileMapAddress  = bit_functions::get_bit(lcdc, 3) ? TILE_MAP_HIGH  : TILE_MAP_LOW;
    u16 tileDataAddress = bit_functions::get_bit(lcdc, 4) ? TILE_DATA_HIGH : TILE_DATA_LOW;

    for(u8 col = 0; col < SCREEN_WIDTH; ++col)
    {
        // get the x and y position in the background
        u8 xPos = col  + scrollX;
        u8 yPos = line + scrollY;

        // get the x and y position of the tile within the background
        u8 tileXPos = xPos / TILE_WIDTH;
        u8 tileYPos = yPos / TILE_HEIGHT;

        // get the x and y position of the pixel within the tile
        u8 pixelXPos = xPos % TILE_WIDTH;
        u8 pixelYPos = yPos % TILE_HEIGHT;

        // get the tile index number and the tile within the tile map
        u16 tileIndex = tileXPos + tileYPos * TILES_PER_LINE;
        u8  tileID    = m_Gameboy.read(tileMapAddress + tileIndex);

        // get the memory offset of the tile in the tile data
        // if we're using tile data one we need treat the offset as signed from 128
        u16 tileDataOffset = (tileDataAddress == TILE_DATA_HIGH)
                           ? (tileID                                   ) * BYTES_PER_TILE
                           : (static_cast<i8>(tileID) + TILE_ONE_OFFSET) * BYTES_PER_TILE;

        // add 2 bytes/line based on the y offset into the tile
        u16 tileAddress = tileDataAddress + tileDataOffset + 2 * pixelYPos;

        u8 low  = m_Gameboy.read(tileAddress    );
        u8 high = m_Gameboy.read(tileAddress + 1);

        Colour::GBColour    gbc = getGBColour(pixelXPos, tileAddress);
        Colour::ScreenColour sc = getScreenColour(gbc);
        drawPixel(col, line, sc);
    }
}

void PPU::drawWindowLine(u8 line)
{
    u8 lcdc = m_Gameboy.read(LCD_CONTROL_REGISTER);

    if(!bit_functions::get_bit(lcdc, 5)) // Window not rendering
    {
        return;
    }

    u8 windowX = m_Gameboy.read(WX_REGISTER) - 7; // window x scroll has an offset of 7

    if(windowX >= SCREEN_WIDTH) // Don't render the window if it's to the right of the screen
    {
        return;
    }

    u8 windowY = m_Gameboy.read(WY_REGISTER);

    if(windowY >= SCREEN_HEIGHT || windowY > line) // Don't render the window if it's below the screen (or we're not at the scanline yet)
    {
        return;
    }

    u16 tileDataAddress = bit_functions::get_bit(lcdc, 4) ? TILE_DATA_HIGH : TILE_DATA_LOW;
    u16 tileMapAddress  = bit_functions::get_bit(lcdc, 6) ? TILE_MAP_HIGH  : TILE_MAP_LOW;

    for(u8 col = 0; col < SCREEN_WIDTH; ++col)
    {
        // get the x and y position in the background
        u8 xPos = col  + windowX;
        u8 yPos = line + windowY;

        // get the x and y position of the tile within the background
        u8 tileXPos = xPos / TILE_WIDTH;
        u8 tileYPos = yPos / TILE_HEIGHT;

        // get the x and y position of the pixel within the tile
        u8 pixelXPos = xPos % TILE_WIDTH;
        u8 pixelYPos = yPos % TILE_HEIGHT;

        // get the tile index number and the tile within the tile map
        u16 tileIndex = tileXPos + tileYPos * TILES_PER_LINE;
        u8  tileID    = m_Gameboy.read(tileMapAddress + tileIndex);

        // get the memory offset of the tile in the tile data
        // if we're using tile data one we need treat the offset as signed from 128
        u16 tileDataOffset = (tileDataAddress == TILE_DATA_HIGH)
                           ? (tileID                                   ) * BYTES_PER_TILE
                           : (static_cast<i8>(tileID) + TILE_ONE_OFFSET) * BYTES_PER_TILE;

        // add 2 bytes/line based on the y offset into the tile
        u16 tileAddress = tileDataAddress + tileDataOffset + 2 * pixelYPos;

        Colour::GBColour gbc    = getGBColour(pixelXPos, tileAddress);
        Colour::ScreenColour sc = getScreenColour(gbc);
        drawPixel(col, line, sc);
    }
}

void PPU::drawSprites(u8 line)
{
    u8 lcdc = m_Gameboy.read(LCD_CONTROL_REGISTER);
    u8 spriteSize = bit_functions::get_bit(lcdc, 2) ? 2 * SPRITE_HEIGHT : SPRITE_HEIGHT;
    
    for (int sprite = 0; sprite < 40; sprite++)
    {
        u8 spriteIndex = sprite * BYTES_PER_SPRITE;

        // Get the sprite's y position, x position, data address and its attributes
        u8 spriteYPos      = m_Gameboy.read(OAM_START_ADDR + spriteIndex    ) - SPRITE_Y_OFFSET;
        u8 spriteXPos      = m_Gameboy.read(OAM_START_ADDR + spriteIndex + 1) - SPRITE_X_OFFSET;
        u8 tileDataAddress = m_Gameboy.read(OAM_START_ADDR + spriteIndex + 2);
        u8 attributes      = m_Gameboy.read(OAM_START_ADDR + spriteIndex + 3);

        bool xFlip      = bit_functions::get_bit(attributes, 5);
        bool yFlip      = bit_functions::get_bit(attributes, 6);
        bool bgPriority = bit_functions::get_bit(attributes, 7);

        // Get the pixel in the sprite (and flip it if needed)
        u8 pixelYPos = line - spriteYPos;
        if(yFlip)
        {
            pixelYPos = spriteSize - 1 - pixelYPos;
        }

        // skip sprites that don't need to be rendered
        if(line < spriteYPos || line >= spriteYPos + spriteSize) continue;

        // add 2 bytes/line based on the y offset into the tile
        u16 tileAddress = TILE_DATA_HIGH + tileDataAddress * BYTES_PER_TILE + 2 * pixelYPos;

        // Loop over all the pixels in the sprite
        for(u8 x = 0; x < SPRITE_WIDTH; ++x)
        {
            // And flip the x position if needed
            u8 pixelXPos = x;
            if(xFlip)
            {
                pixelXPos = SPRITE_WIDTH - 1 - pixelXPos;
            }

            u8 screenXPos = spriteXPos + x;

            // Only render pixels visible on the screen.

            // Less than 0 might continue with the sprite
            if(screenXPos < 0) continue;

            // Greater than the screen's width would not, so go to the next sprite
            if(screenXPos >= SCREEN_WIDTH) break;

            Colour::GBColour gbc = getGBColour(pixelXPos, tileAddress);

            // Don't render white pixels, as they're transparent
            if (gbc == Colour::GBColour::WHITE) continue;

            // Don't draw if the background has priority, unless the colour is white
            if(!bgPriority || getPixel(screenXPos, line) == Colour::GBColour::WHITE)
            {
                Colour::ScreenColour sc = getScreenColour(gbc);
                drawPixel(spriteXPos + x, line, sc);
            }
       }
    }
}

auto PPU::getGBColour(u8 pixelXPos, u16 tileAddress) const -> Colour::GBColour
{
    u8 low  = m_Gameboy.read(tileAddress    );
    u8 high = m_Gameboy.read(tileAddress + 1);

    u8 bit = 7 - pixelXPos;
    u8 colour = bit_functions::get_bit(low, bit) | (bit_functions::get_bit(high, bit) << 1);

    return static_cast<Colour::GBColour>(colour);
}

auto PPU::getScreenColour(Colour::GBColour colour) const -> Colour::ScreenColour
{
    // TODO: Colour pallets

    Colour::ScreenColour c;
    switch(colour)
    {
        case Colour::GBColour::WHITE:
            c.red     = 0x9B;
            c.green   = 0xBC;
            c.blue    = 0x0F;
            c.alpha   = 0xFF;
            break;
        case Colour::GBColour::LIGHT_GRAY:
            c.red     = 0x8B;
            c.green   = 0xAC;
            c.blue    = 0x0F;
            c.alpha   = 0xFF;
            break;
        case Colour::GBColour::DARK_GRAY:
            c.red     = 0x30;
            c.green   = 0x62;
            c.blue    = 0x30;
            c.alpha   = 0xFF;
            break;
        case Colour::GBColour::BLACK:
            c.red     = 0x0F;
            c.green   = 0x38;
            c.blue    = 0x0F;
            c.alpha   = 0xFF;
            break;
        default:
            ASSERT(false, "Invalid colour!");
    }

    return c;
}

void PPU::drawPixel(u8 x, u8 y, Colour::ScreenColour c)
{
    ASSERT(((x + y * SCREEN_WIDTH) * 4 < 0x16800), "INVALID PIXEL POSITION! X: " << (int)x << ", Y: " << (int)y << ", Pos: " << (int)((x + y * SCREEN_WIDTH) * 4));
    
    m_FrameBuffer.at((x + y * SCREEN_WIDTH) * 4    ) = c.red;
    m_FrameBuffer.at((x + y * SCREEN_WIDTH) * 4 + 1) = c.green;
    m_FrameBuffer.at((x + y * SCREEN_WIDTH) * 4 + 2) = c.blue;
    m_FrameBuffer.at((x + y * SCREEN_WIDTH) * 4 + 3) = c.alpha;
}

auto PPU::getPixel(u8 x, u8 y) const -> Colour::GBColour
{
    Colour::ScreenColour c;
    c.red   = m_FrameBuffer.at((x + y * SCREEN_WIDTH) * 4    );
    c.green = m_FrameBuffer.at((x + y * SCREEN_WIDTH) * 4 + 1);
    c.blue  = m_FrameBuffer.at((x + y * SCREEN_WIDTH) * 4 + 2); 
    c.alpha = m_FrameBuffer.at((x + y * SCREEN_WIDTH) * 4 + 3);

    // TODO: Better conversion between screen colour and internal colour
    // Also handle pallets better

    switch(c.red)
    {
        case 0x9B:
            return Colour::GBColour::WHITE;
        case 0x8B:
            return Colour::GBColour::LIGHT_GRAY;
        case 0x30:
            return Colour::GBColour::DARK_GRAY;
        case 0x0F:
            return Colour::GBColour::BLACK;
        default:
            ASSERT(false, "Invalid colour at pixel!");
            return Colour::GBColour::WHITE;
    }
}
