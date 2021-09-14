#include "gpu.h"

GPU::GPU(Gameboy* gb)
    : m_Gameboy(gb)
{
    LOG("Initializing GPU!");

    SDL_Init(SDL_INIT_VIDEO);

    m_Window = SDL_CreateWindow("Shatter Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 432, SDL_WINDOW_SHOWN);
    if(!m_Window)
    {
        LOG("Could not create window: " << SDL_GetError());
    }
    else
    {
        LOG("\tWindow Created.");
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    if(!m_Window)
    {
        LOG("Could not create renderer: " << SDL_GetError());
    }
    else
    {
        LOG("\tRenderer Created.");
    }
}

GPU::~GPU()
{
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

bool GPU::update()
{
    SDL_SetRenderDrawColor(m_Renderer, 65, 105, 225, 255);
    SDL_RenderClear(m_Renderer);

    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                return false;
        }
    }

    SDL_RenderPresent(m_Renderer);

    return true;
}