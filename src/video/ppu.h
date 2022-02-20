#pragma once

#include "core.h"

#include <functional>

class Gameboy;

class PPU
{
    public:
        PPU(Gameboy& gb);

        inline void setDrawCallback(std::function<void(u8* buffer)> callback) { m_DrawCallback = callback; }

        void tick(u8 cycles);
        
    private:

        enum State
        {
            HBlank,
            VBlank,
            OAM_Search,
            
        };

        Gameboy& m_Gameboy;

        u8 m_FrameBuffer[GAMEBOY_WIDTH * GAMEBOY_HEIGHT * 4];
        u8 m_BackgroundBuffer[VRAM_WIDTH * VRAM_HEIGHT * 4];
        std::function<void(u8* buffer)> m_DrawCallback;

        int temp; // Speed up emulation by not redrawing every tick
};