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
        Gameboy(const std::string& path);
        ~Gameboy();

        /**
         * @brief Loads a rom into memory
         * 
         * @param path The filepath to the rom
         */
        void load(const std::string& path);

        /**
         * @brief Loads the current 
         * 
         */
        void load();

        /**
         * @brief Saves a ram to disk
         * 
         */
        void save();

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
        [[nodiscard]] __always_inline auto read(u16 address) const -> u8;

        /**
         * @brief Writes a byte at the specified memory address
         * 
         * @param address The address to write to
         * @param val The value to write
         */
        __always_inline void write(u16 address, u8 val);

        /**
         * @brief Gets the status of the IME (interrupt master enable)
         * 
         * @return The current value of the IME
         */
        [[nodiscard]] __always_inline auto getIME() const -> bool;

        /**
         * @brief Sets the IME (interrupt master enable) to the specified state
         * 
         * @param ime The state to set the IME to
         */
        __always_inline void setIME(bool ime);

        /**
         * @brief Raise an interrupt of a specified type
         * 
         * @param flag The type of interrupt to raise
         */
        __always_inline void raiseInterrupt(const Flags::Interrupt& flag);

        /**
         * @brief Presses a button on the joypad
         * 
         * @param button The button to press
         */
        __always_inline void press(Button button);

        /**
         * @brief Releases a button on the joypad
         * 
         * @param button The button to release
         */
        __always_inline void release(Button button);

        /**
         * @brief Writes an input state to the joypad
         * 
         * @param val The input state to write
         */
        __always_inline void setInput(u8 val);

        /**
         * @brief Gets the current joypad input state
         * 
         */
        __always_inline auto getInput() -> u8;

        /**
         * @brief Gets the button from an SDL keycode
         * 
         */
        __always_inline auto getButton(SDL_Keycode keycode) -> Button;

        /**
         * @brief Gets the value of the DIV register
         * 
         */
        __always_inline auto getDIV() -> u8;

        /**
         * @brief Resets the DIV counter in the timer
         * 
         */
        __always_inline void resetDiv();

        /**
         * @brief Sets the speed of the timer
         * 
         * @param speed The speed to be set
         */
        __always_inline void setTimerSpeed(u32 speed);

        /**
         * @brief Set the title of the window
         * 
         * @param title The title to set
         */
        __always_inline void setTitle(std::string title);
    private:
        APU m_APU;
        CPU m_CPU;
        MMU m_MMU;
        PPU m_PPU;

        Joypad m_Joypad;

        Timer m_Timer;

        Screen m_Screen;

        std::string m_Path;
        bool m_Running;
};

//--------------------------  Inline function implementations --------------------------//

__always_inline auto Gameboy::read(u16 address) const -> u8
{
    return m_MMU.read(address);
}

__always_inline void Gameboy::write(u16 address, u8 val)
{
    m_MMU.write(address, val);
}

__always_inline auto Gameboy::getIME() const -> bool
{
    return m_CPU.getIME();
}

__always_inline void Gameboy::setIME(bool ime)
{
    m_CPU.setIME(ime);
}

__always_inline void Gameboy::raiseInterrupt(const Flags::Interrupt& flag)
{
    m_CPU.raiseInterrupt(flag);
}

__always_inline void Gameboy::press(Button button)
{
    m_Joypad.press(button);
    raiseInterrupt(Flags::Interrupt::Joypad);
}

__always_inline void Gameboy::release(Button button)
{
    m_Joypad.release(button);
}

__always_inline void Gameboy::setInput(u8 val)
{
    m_Joypad.setInput(val);
}

__always_inline auto Gameboy::getInput() -> u8
{
    return m_Joypad.getInput();
}

__always_inline auto Gameboy::getButton(SDL_Keycode keycode) -> Button
{
    return m_Joypad.getButton(keycode);
}

__always_inline auto Gameboy::getDIV() -> u8
{
    return m_Timer.getDIV();
}

__always_inline void Gameboy::resetDiv()
{
    m_Timer.resetDiv();
}

__always_inline void Gameboy::setTimerSpeed(u32 speed)
{
    m_Timer.setSpeed(speed);
}

__always_inline void Gameboy::setTitle(std::string title)
{
    m_Screen.setTitle(title);
}
