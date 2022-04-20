#pragma once

#include "core.hpp"

enum class VideoMode
{
    HBlank,     // Mode 0
    VBlank,     // Mode 1
    OAM_Scan,   // Mode 2
    Transfer    // Mode 3
};

struct Colour
{
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
};
