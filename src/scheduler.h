#pragma once

#include "core.h"

#include <map>

#include "gameboy.h"

class Scheduler
{
    public:
        Scheduler() = default;
        Scheduler(const Scheduler& s) = delete;
        ~Scheduler() = default;

        inline void addGameboy(const char* path) { addGameboy(new Gameboy(path)); }
        void addGameboy(Gameboy* gb);

        void start();

        void stop();
        void stop(u32 index);

        bool run();
    private:
        std::map<u32, Gameboy*> m_Gameboys;
};