#include "cartridge.h"

#include "cartridge_helper.h"

#include <logging/logging.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_cartridge_header(struct Cartridge *cart, u8 *contents) {

    memcpy(&cart->header, contents + HEADER_START, HEADER_SIZE);

    gameboy_log(LOG_INFO, "Title:\t\t%.*s",
                CARTRIDGE_TITLE_SIZE, cart->header.cartridge_title);

    gameboy_log(LOG_INFO, "Licensee:\t%s",
                get_licensee_str(cart->header.old_licensee_code,
                                 cart->header.new_licensee_code));

    gameboy_log(LOG_INFO, "SGB Enabled:\t%s",
                (cart->header.sgb_flag == SGB_ENABLE_FLAG) ? "Yes" : "No");

    gameboy_log(LOG_INFO, "Cartridge Type:\t%s",
                get_cart_type_str(cart->header.cartridge_type));

    gameboy_log(LOG_INFO, "ROM Size:\t%s",
                get_rom_size_str(cart->header.rom_size));

    gameboy_log(LOG_INFO, "RAM Size:\t%s",
                get_ram_size_str(cart->header.ram_size));

    gameboy_log(LOG_INFO, "Destination:\t%s",
                get_destination_str(cart->header.destination_code));

    gameboy_log(LOG_INFO, "Version Number:\t%d",
                cart->header.rom_version_number);

    return RETURN_OK;
}

int init_cartridge(struct Cartridge *cart) {

    cart->rom_0 = malloc(sizeof(u8) * ROM_BANK_0_SIZE);
    if (!cart->rom_0) {
        goto rom_0_init_fail;
    }

    cart->rom_swappable = malloc(sizeof(u8) * ROM_BANK_SWAPPABLE_SIZE);
    if (!cart->rom_swappable) {
        goto rom_swappable_init_fail;
    }

    cart->rom_bank = 0;

    cart->ram_0 = malloc(sizeof(u8) * EXTERNAL_RAM_SIZE);
    if (!cart->ram_0) {
        goto ram_0_init_fail;
    }

    cart->ram_swappable = malloc(sizeof(u8) * EXTERNAL_RAM_SIZE);
    if (!cart->ram_swappable) {
        goto ram_swappable_init_fail;
    }

    cart->ram_bank = 0;

    goto init_success;

    ram_swappable_init_fail:
    free(cart->ram_0);

    ram_0_init_fail:
    free(cart->rom_swappable);

    rom_swappable_init_fail:
    free(cart->rom_0);

    rom_0_init_fail:
    return INIT_FAIL;

    init_success:
    gameboy_log(LOG_DEBUG, "Initialized Cartridge!");
    return RETURN_OK;
}

void cleanup_cartridge(struct Cartridge *cart) {

    if (cart->rom_0) {                  // If rom_0 is allocated, then the rest are as well
        free(cart->ram_swappable);
        free(cart->ram_0);
        free(cart->rom_swappable);
        free(cart->rom_0);
    }

    gameboy_log(LOG_DEBUG, "Destroyed Cartridge!");
}

int load_rom(struct Cartridge *cart, const char *rom_path) {

    FILE *rom_file = fopen(rom_path, "rb");

    if (!rom_file) {
        gameboy_log(LOG_CRITICAL, "Could not open %s. %s", rom_path, strerror(errno));
        goto fopen_fail;
    }

    if (fseek(rom_file, 0, SEEK_END)) {
        gameboy_log(LOG_CRITICAL, "Could not reach end of %s. %s", rom_path, strerror(errno));
        goto fseek_fail;
    }

    int file_len = ftell(rom_file);
    if (file_len < 0) {
        gameboy_log(LOG_CRITICAL, "Could not get length of %s. %s", rom_path, strerror(errno));
        goto ftell_fail;
    }

    rewind(rom_file);
    if (errno) {
        gameboy_log(LOG_CRITICAL, "Could not rewind %s to the beginning. %s", rom_path, strerror(errno));
        goto rewind_fail;
    }

    u8 *contents = malloc(sizeof(u8) * file_len);
    if (!contents) {
        gameboy_log(LOG_CRITICAL, "Could not allocate data for the contents of %s.", rom_path);
        goto malloc_fail;
    }

    if (!fread(contents, file_len, 1, rom_file)) {
        gameboy_log(LOG_CRITICAL, "Could not read %s in its entirety.", rom_path);
        goto fread_fail;
    }

    if (fclose(rom_file)) {
        gameboy_log(LOG_ERROR, "Could not close %s. %s", rom_path, strerror(errno));
    }

    goto load_success;

    fread_fail:
    free(contents);

    malloc_fail:
    rewind_fail:
    ftell_fail:
    fseek_fail:
    if (fclose(rom_file)) {
        gameboy_log(LOG_ERROR, "Could not close %s. %s", rom_path, strerror(errno));
    }

    fopen_fail:
    return INIT_FAIL;

    load_success:
    parse_cartridge_header(cart, contents);
    init_cartridge(cart);
    free(contents);
    return RETURN_OK;
}