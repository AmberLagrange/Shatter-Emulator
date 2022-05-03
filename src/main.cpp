#include "SDL_events.h"
#include "SDL_keycode.h"
#include "core.hpp"

#include <span>

#include <algorithm>
#include <string>

#include <filesystem>
#include <fstream>

#include <chrono>
#include <thread>

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
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_RIGHT:     gb.press(Button::Right);  break;
                        case SDLK_LEFT:      gb.press(Button::Left);   break;
                        case SDLK_UP:        gb.press(Button::Up);     break;
                        case SDLK_DOWN:      gb.press(Button::Down);   break;

                        case SDLK_z:         gb.press(Button::A);      break;
                        case SDLK_x:         gb.press(Button::B);      break;
                        case SDLK_BACKSPACE: gb.press(Button::Select); break;
                        case SDLK_RETURN:    gb.press(Button::Start);  break;

                        case SDLK_1:         Logger::setLogLevel(LogLevel::Opcode);   break;
                        case SDLK_2:         Logger::setLogLevel(LogLevel::Trace);    break;
                        case SDLK_3:         Logger::setLogLevel(LogLevel::Debug);    break;
                        case SDLK_4:         Logger::setLogLevel(LogLevel::Warn);     break;
                        case SDLK_5:         Logger::setLogLevel(LogLevel::Error);    break;
                        case SDLK_6:         Logger::setLogLevel(LogLevel::Critical); break;
                    }
                    break;
                case SDL_KEYUP:
                    if(e.key.repeat) break;
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_RIGHT:     gb.release(Button::Right);  break;
                        case SDLK_LEFT:      gb.release(Button::Left);   break;
                        case SDLK_UP:        gb.release(Button::Up);     break;
                        case SDLK_DOWN:      gb.release(Button::Down);   break;

                        case SDLK_z:         gb.release(Button::A);      break;
                        case SDLK_x:         gb.release(Button::B);      break;
                        case SDLK_BACKSPACE: gb.release(Button::Select); break;
                        case SDLK_RETURN:    gb.release(Button::Start);  break;
                    }
                    break;
            }
        }

        gb.renderFrame();
    }

    Screen::quit();

    _Exit(0); // SDL segfaults unless I call _Exit
}
