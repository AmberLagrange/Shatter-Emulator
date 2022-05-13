#pragma once

#include "core.hpp"

#include <array>

#include <SDL2/SDL.h>

class Gameboy;

class Screen
{
    public:
        /**
         * @brief Initializes SDL2
         * 
         */
        static void init();

        /**
         * @brief Quits SDL2
         * 
         */
        static void quit();
    public:
        Screen();
        ~Screen();

        /**
         * @brief Draws the buffer to the screen
         * 
         * @param buffer The buffer to draw
         */
        void draw(const std::array<u8, FRAME_BUFFER_SIZE>& buffer);

        /**
         * @brief Set the title of the window
         * 
         * @param title The title to set
         */
        void setTitle(std::string title);
    private:
        SDL_Window*     m_Window;
        SDL_Renderer*   m_Renderer;
        SDL_Texture*    m_Texture;

        int m_Scale = 3;
};
