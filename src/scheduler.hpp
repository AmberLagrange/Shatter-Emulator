#pragma once

#include "core.hpp"

#include <unordered_map>
#include <memory>

#include "gameboy.hpp"

class Scheduler
{
    public:
        Scheduler() = default;
        Scheduler(const Scheduler&  s) = delete;
        Scheduler(const Scheduler&& s) = delete;
        ~Scheduler() = default;
        Scheduler& operator=(const Scheduler&  s) = delete;
        Scheduler& operator=(const Scheduler&& s) = delete;

        inline void addGameboy(const char* path) { addGameboy(std::make_unique<Gameboy>(path)); }
        void addGameboy(std::unique_ptr<Gameboy> gb);

        void start();

        void stop();
        void stop(u32 index);

        auto run() -> bool;
    private:
        std::unordered_map<u32, std::unique_ptr<Gameboy>> m_Gameboys;
};
