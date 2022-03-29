#pragma once

#include "core.h"

#include <array>

#include <SDL2/SDL.h>

class Gameboy;

class Screen
{
    public:
        static void init();
        static void quit();
    public:
        Screen();
        ~Screen();

        [[nodiscard]]
        inline auto getWindowID() const -> u32 { return static_cast<u32>(SDL_GetWindowID(m_Window)); }

        void poll();
        void draw(const std::array<u8, FRAME_BUFFER_SIZE>& buffer);
    private:
        SDL_Window*     m_Window;
        SDL_Renderer*   m_Renderer;
        SDL_Texture*    m_Texture;

        int m_Scale = 3;
};