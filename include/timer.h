#pragma once

#include "core.h"

class Timer
{
    public:
        Timer();

        void update(u8 cycles);
    private:
        bool m_Enabled;
};