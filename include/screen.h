#pragma once

#include <SDL2/SDL.h>

#include "core.h"

class Gameboy;

namespace Screen
{
    void initScreen();
    void destroyScreen();
    void setGameboy(Gameboy* gameboy);

    void poll();
    void draw();
};