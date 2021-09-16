#include "screen.h"
#include "gameboy.h"

namespace Screen
{
    namespace
    {
        static Gameboy* s_Gameboy;

        static SDL_Window* s_Window;
        static SDL_Renderer* s_Renderer;
    }

    int initScreen()
    {
        LOG("Initializing Screen!");

        SDL_Init(SDL_INIT_VIDEO);

        s_Window = SDL_CreateWindow("Shatter Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 432, SDL_WINDOW_SHOWN);
        if(!s_Window)
        {
            ERROR("Could not create window: " << SDL_GetError());
            return -1;
        }
        else
        {
            LOG("\tWindow Created.");
        }

        s_Renderer = SDL_CreateRenderer(s_Window, -1, SDL_RENDERER_ACCELERATED);
        if(!s_Window)
        {
            ERROR("Could not create renderer: " << SDL_GetError());
            return -2;
        }
        else
        {
            LOG("\tRenderer Created.");
        }

        return 0;
    }

    void destroyScreen()
    {
        SDL_DestroyRenderer(s_Renderer);
        SDL_DestroyWindow(s_Window);
        SDL_Quit();
    }

    void setGameboy(Gameboy* gameboy)
    {
        s_Gameboy = gameboy;
    }

    void poll()
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    s_Gameboy->stop();
                    return;
            }
        }
    }

    void draw()
    {
        SDL_SetRenderDrawColor(s_Renderer, 65, 105, 225, 255);
        SDL_RenderClear(s_Renderer);
        SDL_RenderPresent(s_Renderer);
    }
}