#include <video/screen.h>

#include <logging/logging.h>
#include <gameboy.h>

int init_sdl(void) {

    int ret;
    if ((ret = SDL_Init(SDL_INIT_VIDEO))) {

        gameboy_log(LOG_CRITICAL, "Could not initialize SDL: %s", SDL_GetError());
    } else {

        gameboy_log(LOG_DEBUG, "Initialized SDL!");
    }

    return ret;
}

void quit_sdl(void) {

    gameboy_log(LOG_DEBUG, "Quitting SDL!");
    SDL_Quit();
}

int init_screen(struct Screen *screen, int scale) {

    screen->rendering_scale = scale;

    screen->sdl_window = SDL_CreateWindow("Shatter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * screen->rendering_scale, SCREEN_HEIGHT * screen->rendering_scale, SDL_WINDOW_SHOWN);
    if (!screen->sdl_window) {

        gameboy_log(LOG_CRITICAL, "Could not create SDL window: %s", SDL_GetError());
        goto init_window_fail;
    }

    screen->sdl_renderer = SDL_CreateRenderer(screen->sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (!screen->sdl_renderer) {

        gameboy_log(LOG_CRITICAL, "Could not create SDL renderer: %s", SDL_GetError());
        goto init_renderer_fail;
    }

    gameboy_log(LOG_DEBUG, "Initialized Screen!");
    return RETURN_OK;

    init_renderer_fail:
    SDL_DestroyWindow(screen->sdl_window);

    init_window_fail:
    return INIT_FAIL;
}

void cleanup_screen(struct Screen *screen) {

    SDL_DestroyRenderer(screen->sdl_renderer);
    SDL_DestroyWindow(screen->sdl_window);
    gameboy_log(LOG_DEBUG, "Destroyed Screen!");
}

void update_screen(struct Screen *screen) {

    // TODO: Proper screen
    SDL_SetRenderDrawColor(screen->sdl_renderer, 255, 255, 255, 255);
    SDL_RenderClear(screen->sdl_renderer);
    SDL_RenderPresent(screen->sdl_renderer);
}

void poll_screen_events(struct Gameboy *gb) {

    // TODO: Proper polling
    SDL_PollEvent(&gb->screen.sdl_event);
    if (gb->screen.sdl_event.type == SDL_QUIT) {
        gb->running = false;
    }
}
