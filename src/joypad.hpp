#pragma once

#include "core.hpp"

/**
    The eight Game Boy action/direction buttons are arranged
    as a 2x4 matrix. Select either action or direction buttons
    by writing to this register, then read out the bits 0-3.

    https://gbdev.io/pandocs/Joypad_Input.html
**/

enum class Button
{
    Right,  A,      // Bit 0
    Left,   B,      // Bit 1
    Up,     Select, // Bit 2
    Down,   Start   // Bit 3
};

class Joypad
{
    public:
        void press(Button button);
        void release(Button button);

        void writeInput(u8 val);
        [[nodiscard]] auto getInput() -> u8;
    private:

        /**
            A value of 0 means the button is held
            A value of 1 means the button is not held
        **/

        bool right      = 1;
        bool left       = 1;
        bool up         = 1;
        bool down       = 1;

        bool a          = 1;
        bool b          = 1;
        bool select     = 1;
        bool start      = 1;

        bool direction  = 1;
        bool action     = 1;
};
