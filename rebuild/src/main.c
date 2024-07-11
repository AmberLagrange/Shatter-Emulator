#include <gameboy.h>

#include <cartridge/cartridge.h>
#include <cartridge/cartridge_helper.h>
#include <logging/logging.h>

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

    start_gameboy(&gb);
    while (gb.running) {

        step(&gb);
    }

    cleanup:
    cleanup_gameboy(&gb);

    return 0;
}