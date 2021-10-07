#pragma once

#include "core.h"

#include <functional>

class MMU;

class PPU
{
    public:
        PPU();

        inline void setDrawCallback(std::function<void(u8* buffer)> callback) { m_DrawCallback = callback; }

        inline void setMMU(MMU* mmu) { m_MMU = mmu; }

        void tick(u8 cycles);
        
    private:

        enum State
        {
            HBlank,
            VBlank,
            OAM_Search,
            
        };

        MMU* m_MMU;

        u8 m_FrameBuffer[GAMEBOY_WIDTH * GAMEBOY_HEIGHT * 4];
        u8 m_BackgroundBuffer[VRAM_WIDTH * VRAM_HEIGHT * 4];
        std::function<void(u8* buffer)> m_DrawCallback;

        int temp; // Speed up emulation by not redrawing every tick
};