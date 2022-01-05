#include "scheduler.h"

void Scheduler::start()
{
    for(auto gb : m_Gameboys)
    {
        gb->start();
    }
}

bool Scheduler::run()
{
    bool running = false;

    for(auto i = m_Gameboys.begin(); i != m_Gameboys.end();)
    {
        if(!(*i)->isRunning())
        {
            delete *i;
            i = m_Gameboys.erase(i);
            continue;
        }

        running = true;
        (*i)->tick();
        ++i;
    }

    return running;
}