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

// TODO: Maybe change how this is handled
struct Gameboy;

int init_sdl();
void quit_sdl();

int init_screen(struct Screen *screen, int scale);
void cleanup_screen(struct Screen *screen);

void update_screen(struct Screen *screen);
void poll_screen_events(struct Gameboy *gb);

#endif // SCREEN_H
