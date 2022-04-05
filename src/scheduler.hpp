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

        /**
         * @brief Creates and adds a new Gameboy to the scheduler
         * using the filepath as the rom to load
         * 
         * @param path The filepath of the rom
         */
        void addGameboy(const char* path);

        /**
         * @brief Adds a Gameboy to the scheduler
         * 
         * @param gb The Gameboy to add
         */
        void addGameboy(std::unique_ptr<Gameboy> gb);

        /**
         * @brief Starts all Gameboys managed by the scheduler
         * 
         */
        void start();

        /**
         * @brief Stops all Gameboys managed by the scheduler
         * 
         */
        void stop();

        /**
         * @brief Stops the specified Gameboy
         * 
         * @param index The SDL Window ID of the Gameboy to stop
         */
        void stop(u32 index);

        auto run() -> bool;
    private:
        std::unordered_map<u32, std::unique_ptr<Gameboy>> m_Gameboys;
};
