#pragma once

#include "core.h"

#include <SDL2/SDL.h>

class Gameboy;

class Screen
{
    public:
        static void initSDL();
        static void quitSDL();
    public:
        Screen();
        ~Screen();

        inline void setGameboy(Gameboy* gameboy) { m_Gameboy = gameboy; }

        void poll();
        void draw(u8* buffer);

    private:
        Gameboy* m_Gameboy;

        SDL_Window*     m_Window;
        SDL_Renderer*   m_Renderer;
        SDL_Texture*    m_Texture;

        int m_Scale = 3;
};