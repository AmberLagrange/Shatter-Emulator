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

    if(!std::filesystem::exists(argv[1]) || std::filesystem::is_directory(argv[1]))
    {
        ERROR("File '" << argv[1] << "' not found!");
        return -1;
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

    Screen::initSDL();

    Gameboy* gb = new Gameboy();

    gb->load(argv[1]);
    
    gb->start();

    Scheduler s;
    s.addGameboy(gb);

    while(s.run()) {}

    Screen::quitSDL();

    return 0;
}