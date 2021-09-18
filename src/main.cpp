#include "core.h"

#include <algorithm>
#include <string>

#include <filesystem>

#include "gameboy.h"
#include "screen.h"

bool optionExists(char** begin, char** end, const std::string option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        ERROR("No file provided!");
        return -1;
    }

    if(!std::filesystem::exists(argv[1]))
    {
        ERROR("File '" << argv[1] << "' not found!");
        return -1;
    }

    if(Screen::initScreen())
    {
        ERROR("Could not Initialize the screen!");
        return -1;
    }

    if(optionExists(argv, argv + argc, "-v") | optionExists(argv, argv + argc, "--verbose"))
    {
        ENABLE_OP_LOGGING();
    }

    Gameboy gameboy;
    gameboy.load(argv[1]);
    gameboy.run();

    Screen::destroyScreen();

    return 0;
}