#pragma once

#include <SDL2/SDL.h>

#include "core.h"

class Gameboy;

namespace Screen
{
    int initScreen();
    void destroyScreen();
    void setGameboy(Gameboy* gameboy);

    void poll();
    void draw();
};