#include "core.h"
#include <video/screen.h>

#include <logging/logging.h>
#include <gameboy.h>

int init_sdl(void) {

    #ifndef GRAPHICS_DISABLED

    int ret;
    if ((ret = SDL_Init(SDL_INIT_VIDEO))) {

        gameboy_log(LOG_FATAL, "Could not initialize SDL: %s", SDL_GetError());
    } else {

        gameboy_log(LOG_DEBUG, "Initialized SDL!");
    }

    return ret;
    #else
    return RETURN_OK;
    #endif
}

void quit_sdl(void) {

    #ifndef GRAPHICS_DISABLED
    gameboy_log(LOG_DEBUG, "Quitting SDL!");
    SDL_Quit();
    #endif
}

int init_screen(struct Screen *screen, int scale) {

    #ifndef GRAPHICS_DISABLED

    screen->rendering_scale = scale;

    screen->sdl_window = SDL_CreateWindow("Shatter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * screen->rendering_scale, SCREEN_HEIGHT * screen->rendering_scale, SDL_WINDOW_SHOWN);
    if (!screen->sdl_window) {

        gameboy_log(LOG_FATAL, "Could not create SDL window: %s", SDL_GetError());
        goto init_window_fail;
    }

    screen->sdl_renderer = SDL_CreateRenderer(screen->sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (!screen->sdl_renderer) {

        gameboy_log(LOG_FATAL, "Could not create SDL renderer: %s", SDL_GetError());
        goto init_renderer_fail;
    }

    gameboy_log(LOG_DEBUG, "Initialized Screen!");
    return RETURN_OK;

    init_renderer_fail:
    SDL_DestroyWindow(screen->sdl_window);

    init_window_fail:
    return INIT_FAIL;

    #else
    (void)screen;
    (void)scale;
    return RETURN_OK;
    #endif
}

void cleanup_screen(struct Screen *screen) {

    #ifndef GRAPHICS_DISABLED
    SDL_DestroyRenderer(screen->sdl_renderer);
    SDL_DestroyWindow(screen->sdl_window);
    gameboy_log(LOG_DEBUG, "Destroyed Screen!");
    #else
    (void)screen;
    #endif
}

void update_screen(struct Screen *screen) {

    #ifndef GRAPHICS_DISABLED
    /* TODO: Proper screen */
    SDL_SetRenderDrawColor(screen->sdl_renderer, 255, 255, 255, 255);
    SDL_RenderClear(screen->sdl_renderer);
    SDL_RenderPresent(screen->sdl_renderer);
    #else
    (void)screen;
    #endif
}

void poll_screen_events(struct Gameboy *gb) {

    #ifndef GRAPHICS_DISABLED
    /* TODO: Proper polling */
    SDL_PollEvent(&gb->screen.sdl_event);
    if (gb->screen.sdl_event.type == SDL_QUIT) {
        gb->running = false;
    }
    #else
    (void)gb;
    #endif
}
