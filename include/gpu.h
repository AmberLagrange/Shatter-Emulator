#pragma once

#include <SDL2/SDL.h>

#include "core.h"

class Gameboy;

class GPU
{
    public:
        GPU(Gameboy* gb);
        ~GPU();

        bool update();
    private:
        Gameboy* m_Gameboy;

        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
};