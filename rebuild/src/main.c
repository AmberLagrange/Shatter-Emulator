#include <gameboy.h>

#include <cartridge/cartridge.h>
#include <cartridge/cartridge_helper.h>
#include <logging/logging.h>

#include <stdio.h>

void dump_rom(struct Gameboy *gb) {

    printf("\t\t");

    for (int j = 0; j < 16; ++j) {
        printf("%02X ", j);
    }

    printf("\n\n");

    for (int i = 0; i < ROM_SIZE * get_rom_bank_count(gb->cart.header.rom_size) / 16; ++i) {
        printf("%04X\t", i * 16);
        for (int j = 0; j < 16; ++j) {
            printf("%02X ", gb->cart.rom_banks[i * 16 + j]);
        }
        printf("\n");
    }

}

int main(int argc, char **argv) {

    if (argc < 2) {
        gameboy_log(LOG_CRITICAL, "No rom path provided.");
        return -1;
    }

    struct Gameboy gb;
    init_gameboy(&gb);

    load_rom_from_path(&gb.cart, argv[1]);

    start_gameboy(&gb);
    while (gb.running) {
        step(&gb);
    }

    cleanup_gameboy(&gb);

    return 0;
}