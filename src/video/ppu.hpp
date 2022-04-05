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

        enum State
        {
            HBlank,
            VBlank,
            OAM_Search,
            
        };

        Gameboy& m_Gameboy;

        std::array<u8, FRAME_BUFFER_SIZE> m_FrameBuffer      {{}};
        std::array<u8, BG_BUFFER_SIZE>    m_BackgroundBuffer {{}};

        std::function<void(std::array<u8, FRAME_BUFFER_SIZE> buffer)> m_DrawCallback;

        int temp; // Speed up emulation by not redrawing every tick
};
