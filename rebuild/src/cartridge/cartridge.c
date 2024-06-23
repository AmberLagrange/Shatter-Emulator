#include "cartridge.h"

#include <stdbool.h>

#include "cartridge_helper.h"

#include <logging/logging.h>

#include <stdlib.h>
#include <string.h>

int parse_cartridge_header(struct Cartridge *cart, u8 *rom_contents) {

    memcpy(&cart->header, rom_contents + HEADER_START, HEADER_SIZE);

    struct CartridgeHeader header = cart->header;

    u8 *title = header.cartridge_title;
    u8 title_length = CARTRIDGE_TITLE_SIZE;

    const char *cgb_support_str;
    bool cgb_support = check_cgb_support(header.cgb_flag, &cgb_support_str);

    if (cgb_support) {

        title = header.cgb_cartridge_title;
        title_length = CGB_CARTRIDGE_TITLE_SIZE;
    }

    gameboy_log(LOG_INFO, "Title:\t\t%.*s",
                title_length, title);

    gameboy_log(LOG_INFO, "Manufacturer:\t%.4s",
                (cgb_support) ? header.manufacturer_code : (u8 *)"");

    gameboy_log(LOG_INFO, "Supports CGB:\t%s",
                cgb_support_str);

    gameboy_log(LOG_INFO, "Licensee:\t%s",
                get_licensee_str(header.old_licensee_code,
                                 header.new_licensee_code));

    gameboy_log(LOG_INFO, "SGB Enabled:\t%s",
                (header.sgb_flag == SGB_ENABLE_FLAG) ? "Yes" : "No");

    gameboy_log(LOG_INFO, "Cartridge Type:\t%s",
                get_cart_type_str(header.cartridge_type));

    gameboy_log(LOG_INFO, "ROM Size:\t%s",
                get_rom_size_str(header.rom_size));

    gameboy_log(LOG_INFO, "RAM Size:\t%s",
                get_ram_size_str(header.ram_size));

    gameboy_log(LOG_INFO, "Destination:\t%s",
                get_destination_str(header.destination_code));

    gameboy_log(LOG_INFO, "Version Number:\t%d",
                header.rom_version_number);

    return RETURN_OK;
}

int init_cartridge(struct Cartridge *cart, u8 *rom_contents, u8 *ram_contents) {

    struct CartridgeHeader header = cart->header;

    size_t rom_size = ROM_BANK_SIZE * get_rom_bank_count(header.rom_size);
    cart->rom_banks = malloc(sizeof(u8) * rom_size);
    if (!cart->rom_banks) {
        goto rom_init_fail;
    }
    memcpy(cart->rom_banks, rom_contents, rom_size);
    cart->rom_bank_index = 0;

    size_t ram_size = EXTERNAL_RAM_SIZE * get_ram_bank_count(header.ram_size);
    cart->ram_banks = malloc(sizeof(u8) * ram_size);
    if (!cart->ram_banks) {
        goto ram_init_fail;
    }

    if (ram_contents) {
        memcpy(cart->ram_banks, ram_contents, ram_size);
    }
    cart->ram_bank_index = 0;

    goto init_finish;

    ram_init_fail:
    gameboy_log(LOG_ERROR, "Could not load RAM into RAM banks.");
    goto init_finish;

    rom_init_fail:
    return INIT_FAIL;

    init_finish:
    gameboy_log(LOG_DEBUG, "Initialized Cartridge!");
    return RETURN_OK;
}

void cleanup_cartridge(struct Cartridge *cart) {

    if (cart->rom_banks) {                  // If rom_banks is allocated, then the rest are as well
        free(cart->ram_banks);
        free(cart->rom_banks);
    }

    gameboy_log(LOG_DEBUG, "Destroyed Cartridge!");
}

int load_rom_from_path(struct Cartridge *cart, const char *rom_path) {

    u8 *rom_contents = load_data_from_file(rom_path, LOG_CRITICAL);

    if (!rom_contents) {
        return INIT_FAIL;
    }

    char ram_path[MAX_STR_LEN] = { 0 };
    strcpy(ram_path, rom_path);
    strcat(ram_path, ".sav");
    u8 *ram_contents = load_data_from_file(ram_path, LOG_WARN);

    parse_cartridge_header(cart, rom_contents);
    init_cartridge(cart, rom_contents, NULL);

    free(ram_contents);
    free(rom_contents);
    return RETURN_OK;
}
