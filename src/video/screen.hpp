#pragma once

#include "core.hpp"

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

        /**
         * @brief Get the SDL Window ID of the screen
         * 
         * @return The SDL Window ID 
         */
        [[nodiscard]] auto getWindowID() const -> u32;

        /**
         * @brief Draws the buffer to the screen
         * 
         * @param buffer The buffer to draw
         */
        void draw(const std::array<u8, FRAME_BUFFER_SIZE>& buffer);
    private:
        SDL_Window*     m_Window;
        SDL_Renderer*   m_Renderer;
        SDL_Texture*    m_Texture;

        int m_Scale = 3;
};
