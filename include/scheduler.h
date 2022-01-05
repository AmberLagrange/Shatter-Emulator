#pragma once

#include "core.h"

#include <vector>

#include "gameboy.h"

class Scheduler
{
    public:
        Scheduler() = default;
        Scheduler(const Scheduler& s) = delete;
        ~Scheduler() = default;

        inline void addGameboy(Gameboy* gb) { m_Gameboys.push_back(gb); }

        bool run();
    private:
        std::vector<Gameboy*> m_Gameboys;
};