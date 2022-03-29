#pragma once

#include "core.h"

#include <array>
#include <functional>

class Gameboy;

class PPU
{
    public:
        PPU(Gameboy& gb);

        inline void setDrawCallback(std::function<void(std::array<u8, FRAME_BUFFER_SIZE> buffer)> callback) { m_DrawCallback = callback; }

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