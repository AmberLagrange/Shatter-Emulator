#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_timer.h"
#include "core.hpp"

#include "CLI11.hpp"

#include <filesystem>
#include <fstream>

#include "gameboy.hpp"
#include "video/screen.hpp"

auto run(int argc, char** argv) -> int;
void pollEvents(Gameboy* gb);

auto main(int argc, char** argv) -> int
{
    // SDL Segfaults if I don't calll _Exit, so this is 
    // just a hack to get around that, while preserving
    // a stack for objects to get destroyed
    _Exit(run(argc, argv));
}

auto run(int argc, char** argv) -> int
{
    Logger::setDefaultStream(std::cout);

    CLI::App shatter{"Shatter Emulator"};

    std::string path;
    shatter.add_option("-r,--rom,-f,--file,rom", path, "Path to the rom.");
    
    std::string bootPath;
    shatter.add_option("-b,--br,--boot,--bootrom,bootrom", bootPath, "Path to a boot rom.");

    std::string logPath;
    shatter.add_option("-l,--log", logPath, "Path to the log file.");

    u8 renderingScale = 0;
    shatter.add_option("-s, --scale, --rendering-scale", renderingScale, "Change the rendering scale of the window.");

    u32 targetFPS = 60;
    shatter.add_option("--fps,--frame-rate", targetFPS, "Set the desired fps of the emulation. Set to 0 for unlimited.");

    #ifndef NDEBUG
        bool verbose = false;
        shatter.add_flag("-v,--verbose", verbose, "Enable opcode logging.");
        if(verbose)
        {
            Logger::enableOpcodeLogging();
        }
    #endif

    CLI11_PARSE(shatter, argc, argv);

    if(path.empty())
    {
        ERROR("No file provided!");
        return -1;
    }

    if(!std::filesystem::exists(path))
    {
        ERROR("File '" << path << "' not found!");
        return -2;
    }

    if(std::filesystem::is_directory(path))
    {
        ERROR("'" << path << "' is a directory!");
        return -3;
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

    Screen::initSDL();

    Gameboy* gb = new Gameboy;
    gb->load(path);

    if(!bootPath.empty())
    {
        gb->loadBoot(bootPath);
    }

    if(renderingScale > 0)
    {
        gb->setRenderingScale(renderingScale);
    }

    gb->start();

    u64 frameStart, frameEnd, fpsStart, fpsEnd;
    float frameDelta, fpsDelta;
    float target;
    bool unlimited = false;
    u32 fps = 0;
    
    if(targetFPS != 0)
    {
        target = 1.0f / targetFPS;
    }
    else
    {
        unlimited = true;
    }

    fpsStart = SDL_GetPerformanceCounter();
    while(gb->isRunning())
    {
        frameStart = SDL_GetPerformanceCounter();
        pollEvents(gb);
        gb->renderFrame();
        frameEnd = SDL_GetPerformanceCounter();

        if(!unlimited)
        {
            frameDelta = (frameEnd - frameStart) / static_cast<float>(SDL_GetPerformanceFrequency());
            if(frameDelta < target)
            {
                SDL_Delay((target - frameDelta) * 1000.0f);
            }
        }
        
        fpsEnd = SDL_GetPerformanceCounter();
        fpsDelta = (fpsEnd - fpsStart) / static_cast<float>(SDL_GetPerformanceFrequency());
        if(fpsDelta >= DEFAULT_TITLE_UPDATE_RATE)
        {
            gb->setTitleFPS(static_cast<float>(fps) / fpsDelta);
            fpsStart = fpsEnd;
            fps = 0;
        }
        else
        {
            ++fps;
        }
    }

    Screen::quitSDL();

    return 0;
}

void pollEvents(Gameboy* gb)
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
                    gb->stop();
                }
                break;

            case SDL_KEYDOWN:
                if(e.key.repeat) break;
                
                button = gb->getButton(e.key.keysym.sym);
                if(button != Button::None)
                {
                    gb->press(button);
                }
                else
                {
                    switch(e.key.keysym.sym)
                    {
                        case SDLK_1: Logger::setLogLevel(LogLevel::Opcode);   break;
                        case SDLK_2: Logger::setLogLevel(LogLevel::Trace);    break;
                        case SDLK_3: Logger::setLogLevel(LogLevel::Debug);    break;
                        case SDLK_4: Logger::setLogLevel(LogLevel::Warn);     break;
                        case SDLK_5: Logger::setLogLevel(LogLevel::Error);    break;
                        case SDLK_6: Logger::setLogLevel(LogLevel::Critical); break;

                        case SDLK_r:
                            const Uint8* state = SDL_GetKeyboardState(nullptr);
                            if(state[SDL_SCANCODE_LCTRL])
                            {
                                gb->save();
                                gb->reset();
                            }
                            break;
                    }
                }
                break;
                
            case SDL_KEYUP:
                if(e.key.repeat) break;

                button = gb->getButton(e.key.keysym.sym);
                if(button != Button::None)
                {
                    gb->release(button);
                }
                break;
        }
    }
}
