#include <cartridge/cartridge.h>
#include <cartridge/cartridge_helper.h>
#include <logging/logging.h>
#include <gameboy.h>

#include <signal.h>

static struct Gameboy gb;

void stop_gameboy(int sig) {

    (void)sig;
    gb.running = false;
}

int main(int argc, char **argv) {

    if (argc < 2) {

        gameboy_log(LOG_FATAL, "No rom path provided.");
        return -1;
    }

    if (init_sdl()) {

        goto cleanup;
    }

    init_gameboy(&gb);

    if (load_rom_from_path(&gb.cart, argv[1])) {
        
        goto cleanup;
    }

    start_gameboy(&gb);
    signal(SIGINT, stop_gameboy);
    while (gb.running) {

        step(&gb);
    }

    cleanup:
    cleanup_gameboy(&gb);
    quit_sdl();

    return 0;
}
