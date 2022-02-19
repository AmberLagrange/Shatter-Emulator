#include "core.h"

#include <algorithm>
#include <string>

#include <filesystem>
#include <fstream>

#include "scheduler.h"
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
        return -2;
    }

    if(std::filesystem::is_directory(argv[1]))
    {
        ERROR("'" << argv[1] << "' is a directory!");
        return -3;
    }

    if(optionExists(argv, argv + argc, "-l"))
    {
        std::ofstream file("./logs/log.log");
        Logger::setDefaultStream(file);
    }

    if(optionExists(argv, argv + argc, "-v") || optionExists(argv, argv + argc, "--verbose"))
    {
        ENABLE_OP_LOGGING();
    }

    Screen::init();

    Scheduler s;

    s.addGameboy(argv[1]);
    s.start();

    while(s.run()) {}

    Screen::quit();

    _Exit(0); // SDL segfaults unless I call _Exit
}