#include "SDL_error.h"
#include "SDL_video.h"
#include "core.hpp"

#include <iomanip>
#include <sstream>

#include "screen.hpp"

#include "gameboy.hpp"

void Screen::initSDL()
{
    DEBUG("Initializing SDL Video.");
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        CRITICAL("Could not initialize SDL Video: " << SDL_GetError());
    }
}

void Screen::quitSDL()
{
    SDL_Quit();
}

Screen::Screen()
    : m_RenderingScale(DEFAULT_RENDERING_SCALE)
{
    DEBUG("Initializing Screen.");
    
    m_Window = SDL_CreateWindow("Shatter Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH * m_RenderingScale, SCREEN_HEIGHT * m_RenderingScale, SDL_WINDOW_SHOWN);
    if(!m_Window)
    {
        CRITICAL("\tCould not create window: " << SDL_GetError());
        return;
    }
    DEBUG("\tWindow Created.");

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    if(!m_Renderer)
    {
        CRITICAL("\tCould not create renderer: " << SDL_GetError());
        return;
    }
    DEBUG("\tRenderer Created.");

    m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
}

Screen::~Screen()
{
    SDL_DestroyTexture(m_Texture);
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
}

void Screen::draw(const std::array<u8, FRAME_BUFFER_SIZE>& buffer)
{
    SDL_UpdateTexture(m_Texture, nullptr, buffer.data(), SCREEN_WIDTH * 4);
    SDL_RenderCopy(m_Renderer, m_Texture, nullptr, nullptr);
    SDL_RenderPresent(m_Renderer);
}

void Screen::setTitle(const std::string& title)
{
    m_Title = title;
    SDL_SetWindowTitle(m_Window, m_Title.c_str());
}

auto Screen::getTitle() const -> const std::string&
{
    return m_Title;
}

void Screen::setTitleFPS(u32 fps)
{
    std::stringstream ss;
    ss << m_Title
       << ", " << std::setprecision(4) << (static_cast<float>(fps) * TARGET_SPEED_MULTIPLIER)
       << "% (" << fps << " FPS)";
    SDL_SetWindowTitle(m_Window, ss.str().c_str());
}

void Screen::setRenderingScale(u8 renderingScale)
{
    m_RenderingScale = renderingScale;

    SDL_SetWindowSize(m_Window, SCREEN_WIDTH * m_RenderingScale, SCREEN_HEIGHT * m_RenderingScale);
    DEBUG("Resized the window to " << (SCREEN_WIDTH * m_RenderingScale) << "x" << (SCREEN_HEIGHT * m_RenderingScale) << ".");
}

auto Screen::getRenderingScale() const -> u32
{
    return m_RenderingScale;
}
