#pragma once

#include "core.h"

#include <SDL2/SDL.h>

class Gameboy;

namespace Screen
{
    int initScreen();
    void destroyScreen();
    void setGameboy(Gameboy* gameboy);

    void poll();
    void draw(u8* buffer);
};