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
         * @brief Returns the state the PPU is in
        **/
        auto getMode() -> VideoMode;

    private:
        void drawBackgroundLine(u8 line);
        void drawWindowLine(u8 line);
        void drawSprites();

        auto getColour(u8 pixelXPos, u16 tileAddress) -> Colour;
        void drawPixel(u8 x, u8 y, Colour c);
    private:
        Gameboy& m_Gameboy;

        std::array<u8, FRAME_BUFFER_SIZE> m_FrameBuffer      {{}};
        std::function<void(std::array<u8, FRAME_BUFFER_SIZE> buffer)> m_DrawCallback;

        VideoMode m_Mode;
        u16 m_Cycles;
        u8 m_Line;

        // Temp drawing before implementing pixel FIFO
        // HBLANK and DRAWING clocks might be different with FIFO
        static constexpr u16 CYCLES_PER_HBLANK   = 204;
        static constexpr u16 CYCLES_PER_VBLANK   = 4560;
        static constexpr u16 CYCLES_PER_OAM_SCAN = 80;
        static constexpr u16 CYCLES_PER_TRANSFER = 172;

        static constexpr u16 CYCLES_PER_LINE     = CYCLES_PER_HBLANK
                                                 + CYCLES_PER_OAM_SCAN
                                                 + CYCLES_PER_TRANSFER;

};
