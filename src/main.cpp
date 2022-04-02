#include "core.hpp"

#include <span>

#include <algorithm>
#include <string>

#include <filesystem>
#include <fstream>

#include <chrono>
#include <thread>

#include "scheduler.hpp"
#include "gameboy.hpp"
#include "video/screen.hpp"

auto optionExists(const std::span<char*>& args, const std::string& option) -> bool
{
    return std::find(args.begin(), args.end(), option) != args.end();
}

[[noreturn]]
auto main(int argc, char** argv) -> int
{
    Logger::setDefaultStream(std::cout);
    if(argc < 2)
    {
        ERROR("No file provided!");
        _Exit(-1);
    }

    auto args = std::span(argv, size_t(argc));

    if(!std::filesystem::exists(args[1]))
    {
        ERROR("File '" << args[1] << "' not found!");
        _Exit(-2);
    }

    if(std::filesystem::is_directory(args[1]))
    {
        ERROR("'" << args[1] << "' is a directory!");
        _Exit(-3);
    }

    if(optionExists(args, "-l"))
    {
        std::ofstream file("./logs/log.log");
        Logger::setDefaultStream(file);
    }

    #ifndef NDEBUG
        if(optionExists(args, "-v") || optionExists(args, "--verbose"))
        {
            ENABLE_OP_LOGGING();
        }
    #endif

    Screen::init();

    Scheduler s;

    s.addGameboy(args[1]);
    s.start();

    while(s.run()) {}

    Screen::quit();

    _Exit(0); // SDL segfaults unless I call _Exit
}
