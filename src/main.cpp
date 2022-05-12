#include "core.hpp"

#include <span>

#include <algorithm>
#include <string>

#include <filesystem>
#include <fstream>

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
            Logger::enableOpcodeLogging();
        }
    #endif

    Screen::init();

    Gameboy gb;
    gb.load(args[1]);
    gb.start();

    while(gb.isRunning())
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            Button button;
            switch(e.type)
            {
                case SDL_WINDOWEVENT:
                    if(e.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        gb.stop();
                    }
                    break;

                case SDL_KEYDOWN:
                    if(e.key.repeat) break;
                    
                    button = gb.getButton(e.key.keysym.sym);
                    if(button != Button::None)
                    {
                        gb.press(button);
                    }
                    else
                    {
                        switch(e.key.keysym.sym)
                        {
                            case SDLK_1:         Logger::setLogLevel(LogLevel::Opcode);   break;
                            case SDLK_2:         Logger::setLogLevel(LogLevel::Trace);    break;
                            case SDLK_3:         Logger::setLogLevel(LogLevel::Debug);    break;
                            case SDLK_4:         Logger::setLogLevel(LogLevel::Warn);     break;
                            case SDLK_5:         Logger::setLogLevel(LogLevel::Error);    break;
                            case SDLK_6:         Logger::setLogLevel(LogLevel::Critical); break;
                        }
                    }
                    break;
                    
                case SDL_KEYUP:
                    if(e.key.repeat) break;

                    button = gb.getButton(e.key.keysym.sym);
                    if(button != Button::None)
                    {
                        gb.release(button);
                    }
                    break;
            }
        }

        gb.renderFrame();
    }

    Screen::quit();
    gb.~Gameboy(); // main's scope never ends, so Gameboy's destructor is never called
                   // Gross hack until SDL no longer segfaults.

    _Exit(0); // SDL segfaults unless I call _Exit
}
