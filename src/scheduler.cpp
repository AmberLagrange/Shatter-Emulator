#include "scheduler.h"

#include <SDL2/SDL.h>

void Scheduler::addGameboy(std::unique_ptr<Gameboy> gb)
{
    m_Gameboys[gb->getWindowID()] = std::move(gb);
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

auto Scheduler::run() -> bool
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
        auto& gb = i->second;
        if(!gb->isRunning())
        {
            i = m_Gameboys.erase(i);
            continue;
        }

        running = true;
        gb->tick();
        ++i;
    }

    return running;
}