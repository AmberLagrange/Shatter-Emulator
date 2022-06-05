#include "core.hpp"

#include "apu.hpp"

void APU::initSDL()
{
    DEBUG("Initializing SDL Audio.");
    if(SDL_Init(SDL_INIT_AUDIO))
    {
        CRITICAL("Could not initialize SDL Audio: " << SDL_GetError());
    }
}

void APU::quitSDL()
{
    SDL_CloseAudio();
}

void APU::staticCallback(void* userdata, u8* buf, int len)
{
    static_cast<APU*>(userdata)->tempCallback(buf, len);
}

APU::APU(Gameboy& gb)
    : m_Gameboy(gb)
{
    DEBUG("Initializing APU.");

    m_Spec.freq     = 22050;
    m_Spec.format   = AUDIO_U8;
    m_Spec.channels = 2;
    m_Spec.samples  = 512;
    m_Spec.callback = staticCallback;   //userdata is used to allow the static callback function to
    m_Spec.userdata = this;             //dispatch the correct object's callback.

    if(SDL_OpenAudio(&m_Spec, nullptr))
    {
        ERROR("Could not open audio: " << SDL_GetError());
    }
    else
    {
        SDL_PauseAudio(0);
    }
}

void APU::tempCallback(u8* buf, int len)
{
    for(int i = 0; i < len; i += 2)
    {
        if(high)
        {
            buf[i] = 15;
            buf[i + 1] = 15;
        }
        else
        {
            buf[i] = 0;
            buf[i + 1] = 0;
        }

        duty++;

        if(duty >= 22050 / (440 * 2))
        {
            high = !high;
            duty = 0;
        }
    }
}
