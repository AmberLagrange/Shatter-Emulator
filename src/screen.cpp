#include "screen.h"
#include "gameboy.h"

namespace Screen
{
    namespace
    {
        Gameboy* s_Gameboy;

        SDL_Window* s_Window;
        SDL_Renderer* s_Renderer;
        SDL_Texture* s_Texture;

        int s_Scale = 3;
    }

    int initScreen()
    {
        DEBUG("Initializing Screen!");

        SDL_Init(SDL_INIT_VIDEO);

        s_Window = SDL_CreateWindow("Shatter Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAMEBOY_WIDTH * s_Scale, GAMEBOY_HEIGHT * s_Scale, SDL_WINDOW_SHOWN);
        if(!s_Window)
        {
            ERROR("Could not create window: " << SDL_GetError());
            return -1;
        }
        else
        {
            DEBUG("\tWindow Created.");
        }

        s_Renderer = SDL_CreateRenderer(s_Window, -1, SDL_RENDERER_ACCELERATED);
        if(!s_Window)
        {
            ERROR("Could not create renderer: " << SDL_GetError());
            return -2;
        }
        else
        {
            DEBUG("\tRenderer Created.");
        }

        s_Texture = SDL_CreateTexture(s_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, GAMEBOY_WIDTH, GAMEBOY_HEIGHT);

        return 0;
    }

    void destroyScreen()
    {
        SDL_DestroyTexture(s_Texture);
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

    void draw(u8* buffer)
    {
        SDL_UpdateTexture(s_Texture, NULL, buffer, GAMEBOY_WIDTH * 4);
        SDL_RenderCopy(s_Renderer, s_Texture, NULL, NULL);
        SDL_RenderPresent(s_Renderer);
    }
}