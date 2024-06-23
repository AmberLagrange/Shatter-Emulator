#include <gameboy.h>

#include <cartridge/cartridge.h>
#include <logging/logging.h>

int main(int argc, char **argv) {

    if (argc < 2) {
        gameboy_log(LOG_CRITICAL, "No rom path provided.");
        return -1;
    }

    struct Gameboy gb;
    init_gameboy(&gb);

    load_rom_from_path(&gb.cart, argv[1]);

    start_gameboy(&gb);

    cleanup_gameboy(&gb);

    return 0;
}