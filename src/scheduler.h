#pragma once

#include "core.h"

#include <unordered_map>
#include <memory>

#include "gameboy.h"

class Scheduler
{
    public:
        Scheduler() = default;
        Scheduler(const Scheduler& s) = delete;
        ~Scheduler() = default;

        inline void addGameboy(const char* path) { addGameboy(std::make_unique<Gameboy>(path)); }
        void addGameboy(std::unique_ptr<Gameboy> gb);

        void start();

        void stop();
        void stop(u32 index);

        auto run() -> bool;
    private:
        std::unordered_map<u32, std::unique_ptr<Gameboy>> m_Gameboys;
};