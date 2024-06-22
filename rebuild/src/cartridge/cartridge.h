#ifndef CART_H
#define CART_H

#include <core.h>

struct CartridgeHeader {

    u8 entry_location[ENTRY_LOCATION_SIZE];
    u8 nintendo_logo[NINTENDO_LOGO_SIZE];
    u8 cartridge_title[CARTRIDGE_TITLE_SIZE];
    u8 new_licensee_code[NEW_LICENSEE_CODE_SIZE];
    u8 sgb_flag;
    u8 cartridge_type;
    u8 rom_size;
    u8 ram_size;
    u8 destination_code;
    u8 old_licensee_code;
    u8 rom_version_number;
    u8 header_checksum;
    u8 global_checksum[GLOBAL_CHECKSUM_SIZE];
};

struct Cartridge {

    struct CartridgeHeader header;

    u8 *rom_0;
    u8 *rom_swappable;
    u8 rom_bank;
    
    u8 *ram_0;
    u8 *ram_swappable;
    u8 ram_bank;

    // TODO: Rest of cartridge features
};

/**
 * @brief Parse information from the cartridge header.
 * 
 * @param rom_path 
 * @return int 
 */
int parse_cartridge_header(struct Cartridge *cart, u8 *contents);

/**
 * @brief Initializes the cartridge
 * 
 * @param cart 
 * @return int 
 */
int init_cartridge(struct Cartridge *cart);

/**
 * @brief Cleans up the cartridge
 * 
 * @param cart 
 */
void cleanup_cartridge(struct Cartridge *cart);

/**
 * @brief Load a ROM into a cartridge struct
 * 
 * @param gb 
 * @param rom_path 
 * @return int 
 */
int load_rom(struct Cartridge *cart, const char *rom_path);

#endif // CART_H
