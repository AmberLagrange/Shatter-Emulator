#include "scheduler.h"

#include <SDL2/SDL.h>

void Scheduler::addGameboy(Gameboy* gb)
{
    m_Gameboys[gb->getWindowID()] = gb;
}

void Scheduler::start()
{
    for(auto& [index, gb] : m_Gameboys)
    {
        gb->start();
    }
}

void Scheduler::stop()
{
    for(auto& [index, gb] : m_Gameboys)
    {
        gb->stop();
    }
}

void Scheduler::stop(u32 index)
{
    m_Gameboys[index]->stop();
}

bool Scheduler::run()
{
    bool running = false;

    SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        if(e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            stop((u32)e.window.windowID);
        }
    }

    for(auto i = m_Gameboys.begin(); i != m_Gameboys.end();)
    {
        Gameboy* gb = i->second;
        if(!gb->isRunning())
        {
            delete gb;
            i = m_Gameboys.erase(i);
            continue;
        }

        running = true;
        gb->tick();
        ++i;
    }

    return running;
}