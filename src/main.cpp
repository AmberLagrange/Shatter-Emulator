#include "CLI11.hpp"
#include "core.hpp"

#include <filesystem>
#include <fstream>

#include "gameboy.hpp"
#include "video/screen.hpp"

[[noreturn]]
auto main(int argc, char** argv) -> int
{
    Logger::setDefaultStream(std::cout);

    CLI::App shatter{"Shatter Emulator"};

    std::string path;
    shatter.add_option("-r,--rom,-f,--file,rom", path, "Path to the rom.");
    
    std::string bootPath;
    shatter.add_option("-b,--br,--boot,--bootrom,bootrom", bootPath, "Path to a boot rom.");

    std::string logPath;
    shatter.add_option("-l,--log", logPath, "Path to the log file.");

    #ifndef NDEBUG
        bool verbose = false;
        shatter.add_flag("-v,--verbose", verbose, "Enable opcode logging.");
        if(verbose)
        {
            Logger::enableOpcodeLogging();
        }
    #endif

    try
    {
        shatter.parse(argc, argv);
    }
    catch (const CLI::ParseError &e)
    {
        _Exit(shatter.exit(e));
    }

    if(path.empty())
    {
        ERROR("No file provided!");
        _Exit(-1);
    }

    if(!std::filesystem::exists(path))
    {
        ERROR("File '" << path << "' not found!");
        _Exit(-2);
    }

    if(std::filesystem::is_directory(path))
    {
        ERROR("'" << path << "' is a directory!");
        _Exit(-3);
    }

    if(!logPath.empty())
    {
        std::ofstream file(logPath);
        Logger::setDefaultStream(file);
    }

    #ifndef NDEBUG
        if(verbose)
        {
            Logger::enableOpcodeLogging();
        }
    #endif

    Screen::init();

    Gameboy gb;
    gb.load(path);
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
    shatter.~App();

    _Exit(0); // SDL segfaults unless I call _Exit
}
