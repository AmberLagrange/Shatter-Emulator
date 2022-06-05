#pragma once

#include "core.hpp"

#include <array>
#include <functional>

#include "video_defs.hpp"

class Gameboy;

class PPU
{
    public:
        PPU(Gameboy& gb);

        /**
         * @brief Sets the callback function for when the screen is ready to be drawn
         * 
         * @param callback The callback function to be called
         */
        void setDrawCallback(std::function<void(std::array<u8, FRAME_BUFFER_SIZE> buffer)> callback);

        /**
         * @brief Emulate the PPU for a specified amount of cycles
         * once the CPU has finished its instruction
         * 
         * @param cycles The amount of cycles that has passed
         */
        void tick(u8 cycles);

        /**
         * @brief Returns the mode the PPU is in
        **/
        [[nodiscard]] auto getMode() const -> VideoMode;

    private:
        /**
         * @brief Draw a background line to the screen
         * 
         * @param line The background line to draw
         */
        void drawBackgroundLine(u8 line);

        /**
         * @brief Draw a window line to the screen
         * 
         * @param line The window line to draw
         */
        void drawWindowLine(u8 line);

        /**
         * @brief Draw the sprites to the screen
         * 
         */
        void drawSprites(u8 line);

        /**
         * @brief Get the gameboy colour of a pixel within a tile
         * 
         * @param pixelXPos The x position of the pixel within
         * the tile
         * 
         * @param tileAddress The address of the tile to get the
         * colour information from
         * @return The Gameboy colour at the x position of the tile
         */
        auto getGBColour(u8 pixelXPos, u16 tileAddress) const -> Colour::GBColour;

        /**
         * @brief Convert a GB Colour into a screen colour
         * 
         * @param colour The colour of the gb colour to convert into a screen colour
         * @return The screen colour at the pixel
         */
        auto getScreenColour(Colour::GBColour colour) const -> Colour::ScreenColour;

        /**
         * @brief Draw a specified pixel at position (x, y) with
         * colour c
         * 
         * @param x The x coordinate of the pixel
         * 
         * @param y The y coordinate of the pixel
         *
         * @param c The colour of the pixel
         */
        void drawPixel(u8 x, u8 y, Colour::ScreenColour c);

        /**
         * @brief Get the colour of a pixel on the screen
         * 
         * @param x The x coordinate of the pixel to get
         * 
         * @param y The y coordinate of the pixel to get
         * @return The Gameboy colour at the pixel
         */
        [[nodiscard]] auto getPixel(u8 x, u8 y) const -> Colour::GBColour;
    private:
        Gameboy& m_Gameboy;

        std::array<u8, FRAME_BUFFER_SIZE> m_FrameBuffer      {{}};
        std::function<void(std::array<u8, FRAME_BUFFER_SIZE> buffer)> m_DrawCallback;

        VideoMode m_Mode;
        u16 m_Cycles;
        u8 m_Line;
};
