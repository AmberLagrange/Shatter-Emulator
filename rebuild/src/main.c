#include <SDL_video.h>
#include <gameboy.h>

#include <cartridge/cartridge.h>
#include <cartridge/cartridge_helper.h>
#include <logging/logging.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

int main(int argc, char **argv) {

    if (argc < 2) {

        gameboy_log(LOG_CRITICAL, "No rom path provided.");
        return -1;
    }

    struct Gameboy gb;
    init_gameboy(&gb);

    if (load_rom_from_path(&gb.cart, argv[1])) {
        
        goto cleanup;
    }

    // TODO: Move SDL to proper place
    if (SDL_Init(SDL_INIT_VIDEO)) {

        gameboy_log(LOG_CRITICAL, "Could not initialize SDL.");
        goto cleanup;
    }

    SDL_Window *window = SDL_CreateWindow("Shatter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 720, 0);
    SDL_Surface* screenSurface = SDL_GetWindowSurface( window );
    SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    SDL_UpdateWindowSurface( window );
    SDL_Event e;

    start_gameboy(&gb);
    while (gb.running) {

        step(&gb);
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT) {
            gb.running = false;
        }
    }

    cleanup:
    cleanup_gameboy(&gb);

    return 0;
}