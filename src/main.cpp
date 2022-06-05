#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "core.hpp"

#include "CLI11.hpp"

#include <filesystem>
#include <fstream>

#include "gameboy.hpp"
#include "video/screen.hpp"

auto run(int argc, char** argv) -> int;
void pollEvents(Gameboy& gb);

void foo_callback(void*, u8* buf, u32 len);

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

    Screen::init();

    Gameboy gb;
    gb.load(path);

    if(!bootPath.empty())
    {
        gb.loadBoot(bootPath);
    }

    gb.start();

    u32 startTime, endTime, delta;
    u32 target = 1000 / 60;

    DEBUG("Initializing SDL Audio.");
    if(SDL_Init(SDL_INIT_AUDIO))
    {
        CRITICAL("Could not initialize SDL Audio: " << SDL_GetError());
    }
    SDL_AudioSpec spec;
    spec.freq = 22050;
    spec.format = AUDIO_U8;
    spec.channels = 2;
    spec.samples = 512;
    spec.callback = SDL_AudioCallback(foo_callback);

    SDL_OpenAudio(&spec, nullptr);
    SDL_PauseAudio(0);
    SDL_Delay(5000);
    SDL_CloseAudio();
    delete[] (int*)spec.userdata;
    return 0;

    while(gb.isRunning())
    {
        startTime = SDL_GetTicks();
        gb.renderFrame();
        pollEvents(gb);
        endTime = SDL_GetTicks();
        delta = endTime - startTime;
        if(delta < target)
        {
            SDL_Delay(target - delta);
        }
    }

    Screen::quit();

    return 0;
}

void pollEvents(Gameboy& gb)
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
                                gb.save();
                                gb.reset();
                            }
                            break;
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
}

static bool high = false;
u32 duty = 0;
u32 sampleFrames = 0;

void foo_callback(void*, u8* buf, u32 len)
{
    for(int i = 0; i < len; i += 2)
    {
        if(high)
        {
            buf[i] = 15;
            buf[i + 1] = 15;
        }
        else
        {
            buf[i] = 0;
            buf[i + 1] = 0;
        }

        duty++;

        if(duty >= 22050 / (440 * 2))
        {
            high = !high;
            duty = 0;
        }
    }
}
