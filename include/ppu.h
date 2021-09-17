#pragma once

#include "core.h"

#include <functional>

class PPU
{
    public:
        PPU();

        inline void setDrawCallback(std::function<void(void)> callback) { m_DrawCallback = callback; }

        void tick(u8 cycles);
        
    private:
        std::function<void(void)> m_DrawCallback;

        int temp; // Speed up emulation by not redrawing every tick
};