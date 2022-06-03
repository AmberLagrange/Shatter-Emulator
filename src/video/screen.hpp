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
        static void initSDL();

        /**
         * @brief Quits SDL2
         * 
         */
        static void quitSDL();
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
        void setTitle(const std::string& title);

        /**
         * @brief Get the title of the window
         * 
         */
         auto getTitle() -> const std::string&;

         /**
         * @brief Set the fps in the title of the window
         * 
         * @param fps The fps to set
         */
        void setTitleFPS(u32 fps);

        /**
         * @brief Set the rendering scale of the window
         * 
         * @param scale The rendering scale to set
         */
        void setRenderingScale(u32 scale);

        /**
         * @brief Get the rendering scale of the window
         * 
         */
        auto getRenderingScale() -> u32;
    private:
        SDL_Window*     m_Window;
        SDL_Renderer*   m_Renderer;
        SDL_Texture*    m_Texture;

        std::string m_Title;
        u32 m_RenderingScale;
};
