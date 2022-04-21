#pragma once

#include "core.hpp"

#include "audio/apu.hpp"

#include "mmu.hpp"

#include "cpu/cpu.hpp"
#include "cpu/timer.hpp"

#include "video/screen.hpp"
#include "video/ppu.hpp"

#include "joypad.hpp"

class Gameboy
{
    public:
        Gameboy();
        Gameboy(const char* path);

        /**
         * @brief Loads a rom into memory
         * 
         * @param path The filepath to the rom
         */
        void load(const char* path);

        /**
         * @brief Starts the Gameboy
         * 
         */
        void start();

        /**
         * @brief Emulates a step of the cpu, apu and ppu
         * 
         */
        void tick();

        /**
         * @brief Renders a single frame of the gameboy
         * 
         */
        void renderFrame();

        /**
         * @brief Stops the Gameboy
         * 
         */
        void stop();

        /**
         * @brief Checks if the Gameboy is currently running
         * 
         * @return The state of the Gameboy
         */
        auto isRunning() const -> bool;

        /**
         * @brief Reads a byte from the specified memory address
         * 
         * @param address The address to read from
         * @return The value stored at that address
         */
        [[nodiscard]]auto read(u16 address) const -> u8;

        /**
         * @brief Writes a byte at the specified memory address
         * 
         * @param address The address to write to
         * @param val The value to write
         */
        void write(u16 address, u8 val);

        /**
         * @brief Gets the status of the IME (interrupt master enable)
         * 
         * @return The current value of the IME
         */
        [[nodiscard]] auto getIME() const -> bool;

        /**
         * @brief Sets the IME (interrupt master enable) to the specified state
         * 
         * @param ime The state to set the IME to
         */
        void setIME(bool ime);

        /**
         * @brief Raise an interrupt of a specified type
         * 
         * @param flag The type of interrupt to raise
         */
        void raiseInterrupt(const Flags::Interrupt& flag);

        void press(Button button);
        void release(Button button);
        void writeInput(u8 val);
        auto getInput() -> u8;

        /**
         * @brief Resets the DIV counter in the timer
         * 
         */
        void resetDiv();

        /**
         * @brief Sets the speed of the timer
         * 
         * @param speed The speed to be set
         */
        void setTimerSpeed(u32 speed);
    private:
        APU m_APU;
        CPU m_CPU;
        MMU m_MMU;
        PPU m_PPU;

        Joypad m_Joypad;

        Timer m_Timer;

        Screen m_Screen;

        bool m_Running;
};
