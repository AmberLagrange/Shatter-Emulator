#ifndef SCREEN_H
#define SCREEN_H

#include <core.h>

#include <SDL2/SDL.h>

struct Screen {

    SDL_Window      *sdl_window;
    SDL_Renderer    *sdl_renderer;
    SDL_Event       sdl_event;

    int             rendering_scale;
};

/**
 * @brief Initialized SDL2
 * 
 * @return int return code from SDL2
 */
int init_sdl(void);

/**
 * @brief Allows SDL2 to quit gracefully
 * 
 */
void quit_sdl(void);

/**
 * @brief Initializes the screen for the Gameboy
 * 
 * @param screen 
 * @param scale 
 * @return int 
 */
int init_screen(struct Screen *screen, int scale);

/**
 * @brief Cleans up the screen for the Gameboy
 * 
 * @param screen 
 */
void cleanup_screen(struct Screen *screen);

/**
 * @brief Renders to the Gameboy's screen
 * 
 * @param screen 
 */
void update_screen(struct Screen *screen);

// TODO: Maybe change how this is handled
struct Gameboy;

/**
 * @brief Poll events from SDL2 for IO and quitting the emulator
 * 
 * @param gb 
 */
void poll_screen_events(struct Gameboy *gb);

#endif // SCREEN_H
