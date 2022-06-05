#pragma once

#include "core.hpp"

#include <SDL2/SDL.h>

class Gameboy;

class APU
{
    public:
        /**
         * @brief Initializes SDL2 Audio
         * 
         */
        static void initSDL();

        /**
         * @brief Quits SDL2 Audio
         * 
         */
        static void quitSDL();
    private:
        /**
         * @brief Calls the object's callback by using userdata to
         * get the object itself
         * 
         * @param userdata The pointer to the object to dispatch the callback to
         *
         * @param buf The buffer to write the sound data to
         *
         * @param len The length of the buffer
         */
        static void staticCallback(void* userdata, u8* buf, int len);
    public:
        APU(Gameboy& gb);

        //Temp
        void tempCallback(u8* buf, int len);

    private:
        Gameboy& m_Gameboy [[maybe_unused]];

        SDL_AudioSpec m_Spec;

        //Temp
        bool high = false;
        u32 tone = 440;
        u32 duty = 0;
        u32 sampleFrames = 0;
};
