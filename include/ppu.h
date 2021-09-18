#pragma once

#include "core.h"

#include <functional>

class PPU
{
    public:
        PPU();

        inline void setDrawCallback(std::function<void(u8* buffer)> callback) { m_DrawCallback = callback; }

        void tick(u8 cycles);
        
    private:
        std::function<void(u8* buffer)> m_DrawCallback;

        u8 m_FrameBuffer[GAMEBOY_WIDTH * GAMEBOY_HEIGHT * 4];

        int temp; // Speed up emulation by not redrawing every tick
};