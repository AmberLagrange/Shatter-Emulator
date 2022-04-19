#pragma once

#include "core.hpp"

#include <array>
#include <functional>

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
        
    private:
        void drawBackgroundLine(u8 line);
        void drawWindowLine(u8 line);
        void drawSprites();

    private:
        enum class Mode
        {
            HBlank,     // Mode 0
            VBlank,     // Mode 1
            OAM_Scan,   // Mode 2
            Drawing     // Mode 3
        };

        Gameboy& m_Gameboy;

        std::array<u8, FRAME_BUFFER_SIZE> m_FrameBuffer      {{}};
        std::function<void(std::array<u8, FRAME_BUFFER_SIZE> buffer)> m_DrawCallback;

        Mode m_Mode;
        u16 m_Cycles;
        u8 m_Line;

        // Temp drawing before implementing pixel FIFO
        // HBLANK and DRAWING clocks might be different with FIFO
        static constexpr u8  CYCLES_PER_HBLANK   = 204;
        static constexpr u16 CYCLES_PER_VBLANK   = 4560;
        static constexpr u8  CYCLES_PER_OAM_SCAN = 80;
        static constexpr u8  CYCLES_PER_DRAWING  = 172;

        static constexpr u16 CYCLES_PER_LINE     = CYCLES_PER_HBLANK
                                                 + CYCLES_PER_OAM_SCAN
                                                 + CYCLES_PER_DRAWING;

};
