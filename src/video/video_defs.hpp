#pragma once

#include "core.hpp"

// TODO: Maybe move colour to its own file?

enum class VideoMode
{
    HBlank,     // Mode 0
    VBlank,     // Mode 1
    OAM_Scan,   // Mode 2
    Transfer    // Mode 3
};

namespace Colour
{
    enum GBColour
    {
        WHITE       = 0b00,
        LIGHT_GRAY  = 0b01,
        DARK_GRAY   = 0b10,
        BLACK       = 0b11
    };

    struct ScreenColour
    {
        u8 red;
        u8 green;
        u8 blue;
        u8 alpha;
    };
}


