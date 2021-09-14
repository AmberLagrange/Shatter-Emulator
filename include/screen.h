#pragma once

#include <SDL2/SDL.h>

#include "core.h"

class Gameboy;

class Screen
{
    public:
        Screen(Gameboy* gb);
        ~Screen();

        void update();
    private:
        Gameboy* m_Gameboy;

        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
};