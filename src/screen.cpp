#include "screen.h"
#include "gameboy.h"

void Screen::initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
}

void Screen::quitSDL()
{
    DEBUG("Quitting SDL!");
    SDL_Quit();
}

Screen::Screen()
{
    DEBUG("Initializing Screen!");
    
    m_Window = SDL_CreateWindow("Shatter Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                GAMEBOY_WIDTH * m_Scale, GAMEBOY_HEIGHT * m_Scale, SDL_WINDOW_SHOWN);
    if(!m_Window)
    {
        ERROR("\tCould not create window: " << SDL_GetError());
        return;
    }
    DEBUG("\tWindow Created.");

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    if(!m_Renderer)
    {
        ERROR("\tCould not create renderer: " << SDL_GetError());
        return;
    }
    DEBUG("\tRenderer Created.");

    m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, GAMEBOY_WIDTH, GAMEBOY_HEIGHT);
}

Screen::~Screen()
{
    SDL_DestroyTexture(m_Texture);
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
}

void Screen::poll()
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        if(e.type == SDL_WINDOWEVENT
           && e.window.event == SDL_WINDOWEVENT_CLOSE
           && SDL_GetWindowID(m_Window) == e.window.windowID)
        {
            m_Gameboy->stop();
        }
    }
}

void Screen::draw(u8* buffer)
{
    SDL_UpdateTexture(m_Texture, NULL, buffer, GAMEBOY_WIDTH * 4);
    SDL_RenderCopy(m_Renderer, m_Texture, NULL, NULL);
    SDL_RenderPresent(m_Renderer);
}