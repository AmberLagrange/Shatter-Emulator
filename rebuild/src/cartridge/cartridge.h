#ifndef CART_H
#define CART_H

#include <core.h>

struct Bus;

struct CartridgeHeader {

    u8 entry_location[ENTRY_LOCATION_SIZE];
    u8 nintendo_logo[NINTENDO_LOGO_SIZE];

    union {
        
        u8 cartridge_title[CARTRIDGE_TITLE_SIZE];
        struct {

            u8 cgb_cartridge_title[CGB_CARTRIDGE_TITLE_SIZE];
            u8 manufacturer_code[MANUFACTURER_CODE_SIZE];
            u8 cgb_flag;
        };
    };

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

    u8 *rom_banks;
    u8 rom_bank_index;
    
    u8 *ram_banks;
    u8 ram_bank_index;

    // TODO: Rest of cartridge features
};

/**
 * @brief Parse information from the cartridge header.
 * 
 * @param cart 
 * @param contents 
 * @return int 
 */
int parse_cartridge_header(struct Cartridge *cart, u8 *rom_contents);

/**
 * @brief Initializes the cartridge and copy contents into its buffers
 * 
 * @param cart 
 * @param rom_contents 
 * @param ram_contents 
 * @return int 
 */
int init_cartridge(struct Cartridge *cart, u8 *rom_contents, u8 *ram_contents);

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
int load_rom_from_path(struct Cartridge *cart, const char *rom_path);

/**
 * @brief Read a byte from the cartridge at a specific address
 *        Will handle ROM banks behind the scenes
 * 
 * @param cart 
 * @param address 
 * @return u8 
 */
u8 read_byte_from_cart(struct Cartridge *cart, u16 address);

#endif // CART_H
