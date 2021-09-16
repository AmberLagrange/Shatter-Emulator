#include "gameboy.h"

#include "screen.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        ERROR("No file provided.");
        return -1;
    }

    if(Screen::initScreen())
    {
        ERROR("Could not Initialize the screen! Exiting!");
        return -1;
    }

    Gameboy gb;
    gb.load(argv[1]);
    gb.run();

    Screen::destroyScreen();

    return 0;
}