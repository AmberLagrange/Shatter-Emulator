#ifndef CARTRIDGE_HELPER_H
#define CARTRIDGE_HELPER_H

#include <core.h>

#include <stdbool.h>

#include <logging/logging.h>

enum OldLicenseeCode {

    LICENSE_NONE                    = 0x00,
    LICENSE_NINTENDO_1              = 0x01,
    LICENSE_CAPCOM_1                = 0x08,
    LICENSE_HOT_B                   = 0x09,
    LICENSE_JALECO_1                = 0x0A,
    LICENSE_COCONUTS                = 0x0B,
    LICENSE_ELITE_SYSTEMS_1         = 0x0C,
    LICENSE_ELECTRONIC_ARTS_1       = 0x13,
    LICENSE_HUDSONSOFT              = 0x18,
    LICENSE_ITC_ENTERTAINMENT       = 0x19,
    LICENSE_YANOMAN                 = 0x1A,
    LICENSE_CLARY                   = 0x1D,
    LICENSE_VIRGIN_1                = 0x1F,
    LICENSE_PCM_COMPLETE            = 0x24,
    LICENSE_SAN_X                   = 0x25,
    LICENSE_KOTOBUKI_SYSTEMS        = 0x28,
    LICENSE_SETA                    = 0x29,
    LICENSE_INFOGRAMES_1            = 0x30,
    LICENSE_NINTENDO_2              = 0x31,
    LICENSE_BANDAI_1                = 0x32,
    LICENSE_NEW_LICENSEE_CODE       = 0x33,
    LICENSE_KONAMI_1                = 0x34,
    LICENSE_HECTOR                  = 0x35,
    LICENSE_CAPCOM_2                = 0x38,
    LICENSE_BANPRESTO_1             = 0x39,
    LICENSE_ENTERTAINMENT_I         = 0x3C,
    LICENSE_GREMLIN                 = 0x3E,
    LICENSE_UBI_SOFT                = 0x41,
    LICENSE_ATLUS_1                 = 0x42,
    LICENSE_MALIBU_1                = 0x44,
    LICENSE_ANGEL_1                 = 0x46,
    LICENSE_SPECTRUM_HOLOBY         = 0x47,
    LICENSE_IREM                    = 0x49,
    LICENSE_VIRGIN_2                = 0x4A,
    LICENSE_MALIBU_2                = 0x4D,
    LICENSE_U_S_GOLD                = 0x4F,
    LICENSE_ABSOLUTE                = 0x50,
    LICENSE_ACCLAIM_1               = 0x51,
    LICENSE_ACTIVISION              = 0x52,
    LICENSE_AMERICAN_SAMMY          = 0x53,
    LICENSE_GAMETEK                 = 0x54,
    LICENSE_PARK_PLACE              = 0x55,
    LICENSE_LJN_1                   = 0x56,
    LICENSE_MATCHBOX                = 0x57,
    LICENSE_MILTON_BRADLEY          = 0x59,
    LICENSE_MINDSCAPE               = 0x5A,
    LICENSE_ROMSTAR                 = 0x5B,
    LICENSE_NAXAT_SOFT_1            = 0x5C,
    LICENSE_TRADEWEST               = 0x5D,
    LICENSE_TITUS                   = 0x60,
    LICENSE_VIRGIN_3                = 0x61,
    LICENSE_OCEAN                   = 0x67,
    LICENSE_ELECTRONIC_ARTS_2       = 0x69,
    LICENSE_ELITE_SYSTEMS_2         = 0x6E,
    LICENSE_ELECTRO_BRAIN           = 0x6F,
    LICENSE_INFOGRAMES_2            = 0x70,
    LICENSE_INTERPLAY               = 0x71,
    LICENSE_BRODERBUND_1            = 0x72,
    LICENSE_SCULPTERED_SOFT         = 0x73,
    LICENSE_THE_SALES_CURVE         = 0x75,
    LICENSE_T_HQ                    = 0x78,
    LICENSE_ACCOLADE                = 0x79,
    LICENSE_TRIFFIX_ENTERTAINMENT   = 0x7A,
    LICENSE_MICROPROSE              = 0x7C,
    LICENSE_KEMCO_1                 = 0x7F,
    LICENSE_MISAWA_ENTERTAINMENT    = 0x80,
    LICENSE_LOZC                    = 0x83,
    LICENSE_TOKUMA_SHOTEN_I_1       = 0x86,
    LICENSE_BULLET_PROOF_SOFTWARE   = 0x8B,
    LICENSE_VIC_TOKAI               = 0x8C,
    LICENSE_APE                     = 0x8E,
    LICENSE_I_MAX                   = 0x8F,
    LICENSE_CHUN_SOFT               = 0x91,
    LICENSE_VIDEO_SYSTEM            = 0x92,
    LICENSE_TSUBURAVA               = 0x93,
    LICENSE_VARIE_1                 = 0x95,
    LICENSE_YONEZAWAS_PAL           = 0x96,
    LICENSE_KANEKO                  = 0x97,
    LICENSE_ARC                     = 0x99,
    LICENSE_NIHON_BUSSAN            = 0x9A,
    LICENSE_TECMO                   = 0x9B,
    LICENSE_IMAGINEER               = 0x9C,
    LICENSE_BANPRESTO_2             = 0x9D,
    LICENSE_NOVA                    = 0x9F,
    LICENSE_HORI_ELECTRIC           = 0xA1,
    LICENSE_BANDAI_2                = 0xA2,
    LICENSE_KONAMI_2                = 0xA4,
    LICENSE_KAWADA                  = 0xA6,
    LICENSE_TAKARA                  = 0xA7,
    LICENSE_TECHNOS_JAPAN           = 0xA9,
    LICENSE_BRODERBUND_2            = 0xAA,
    LICENSE_TOEI_ANIMATION          = 0xAC,
    LICENSE_TOHO                    = 0xAD,
    LICENSE_NAMCO                   = 0xAF,
    LICENSE_ACCLAIM_2               = 0xB0,
    LICENSE_ASCII_OR_NEXSOFT        = 0xB1,
    LICENSE_BANDAI_3                = 0xB2,
    LICENSE_ENIX                    = 0xB4,
    LICENSE_HAL                     = 0xB6,
    LICENSE_SNK                     = 0xB7,
    LICENSE_PONY_CANYON             = 0xB9,
    LICENSE_CULTURE_BRAIN_O         = 0xBA,
    LICENSE_SUNSOFT                 = 0xBB,
    LICENSE_SONY_IMAGESOFT          = 0xBD,
    LICENSE_SAMMY                   = 0xBF,
    LICENSE_TAITO_1                 = 0xC0,
    LICENSE_KEMCO_2                 = 0xC2,
    LICENSE_SQUARESOFT              = 0xC3,
    LICENSE_TOKUMA_SHOTEN_I_2       = 0xC4,
    LICENSE_DATA_EAST               = 0xC5,
    LICENSE_TONKIN_HOUSE            = 0xC6,
    LICENSE_KOEI                    = 0xC8,
    LICENSE_UFL                     = 0xC9,
    LICENSE_ULTRA                   = 0xCA,
    LICENSE_VAP                     = 0xCB,
    LICENSE_USE                     = 0xCC,
    LICENSE_MELDAC                  = 0xCD,
    LICENSE_PONY_CANYON_OR          = 0xCE,
    LICENSE_ANGEL_2                 = 0xCF,
    LICENSE_TAITO_2                 = 0xD0,
    LICENSE_SOFEL                   = 0xD1,
    LICENSE_QUEST                   = 0xD2,
    LICENSE_SIGMA_ENTERPRISES       = 0xD3,
    LICENSE_ASK_KODANSHA            = 0xD4,
    LICENSE_NAXAT_SOFT_2            = 0xD6,
    LICENSE_COPYA_SYSTEMS           = 0xD7,
    LICENSE_BANPRESTO_3             = 0xD9,
    LICENSE_TOMY                    = 0xDA,
    LICENSE_LJN_2                   = 0xDB,
    LICENSE_NCS                     = 0xDD,
    LICENSE_HUMAN                   = 0xDE,
    LICENSE_ALTRON                  = 0xDF,
    LICENSE_JALECO_2                = 0xE0,
    LICENSE_TOWACHIKI               = 0xE1,
    LICENSE_UUTAKA                  = 0xE2,
    LICENSE_VARIE_2                 = 0xE3,
    LICENSE_EPOCH                   = 0xE5,
    LICENSE_ATHENA                  = 0xE7,
    LICENSE_ASMIK                   = 0xE8,
    LICENSE_NATSUME                 = 0xE9,
    LICENSE_KING_RECORDS            = 0xEA,
    LICENSE_ATLUS_2                 = 0xEB,
    LICENSE_EPIC_SONY_RECORDS       = 0xEC,
    LICENSE_IGS                     = 0xEE,
    LICENSE_A_WAVE                  = 0xF0,
    LICENSE_EXTREME_ENTERTAINMENT   = 0xF3,
    LICENSE_LJN_3                   = 0xFF
};

enum NewLicenseeCode {
    TODO
};

enum CartridgeType {
     ROM_ONLY                       = 0x00,
     MBC1                           = 0x01,
     MBC1_RAM                       = 0x02,
     MBC1_RAM_BATTERY               = 0x03,
     MBC2                           = 0x05,
     MBC2_BATTERY                   = 0x06,
     ROM_RAM                        = 0x08,
     ROM_RAM_BATTERY                = 0x09,
     MMM01                          = 0x0B,
     MMM01_RAM                      = 0x0C,
     MMM01_RAM_BATTERY              = 0x0D,
     MBC3_TIMER_BATTERY             = 0x0F,
     MBC3_TIMER_RAM_BATTERY         = 0x10,
     MBC3                           = 0x11,
     MBC3_RAM                       = 0x12,
     MBC3_RAM_BATTERY               = 0x13,
     MBC5                           = 0x19,
     MBC5_RAM                       = 0x1A,
     MBC5_RAM_BATTERY               = 0x1B,
     MBC5_RUMBLE                    = 0x1C,
     MBC5_RUMBLE_RAM                = 0x1D,
     MBC5_RUMBLE_RAM_BATTERY        = 0x1E,
     MBC6                           = 0x20,
     MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,
     POCKET_CAMERA                  = 0xFC,
     BANDAI_TAMA5                   = 0xFD,
     HUC3                           = 0xFE,
     HUC1_RAM_BATTERY               = 0xFF
};

/**
 * @brief Checks if the cartridge supports CGB
 * 
 * @param cgb_support_str 
 * @return int 
 */
bool check_cgb_support(u8 cgb_flag, const char **cgb_support_str);

/**
 * @brief Converts two ASCII characters from the cartridge header
          into an enum representing a new licensee code
 * 
 * @param new_licensee_code 
 * @return enum NewLicenseeCode 
 */
enum NewLicenseeCode convert_ascii_license_to_enum(u8 *new_licensee_code);

/**
 * @brief Converts a licensee code from a cartridge header into a c string
 * 
 * @param old_licensee_code 
 * @param new_licensee_code 
 * @return const char* 
 */
const char *get_licensee_str(u8 old_licensee_code, u8 *new_licensee_code);

/**
 * @brief Converts a cart type from a cartridge header into a c string
 * 
 * @param cart_type 
 * @return const char* 
 */
const char *get_cart_type_str(u8 cart_type);

/**
 * @brief Gets the number of ROM banks in the cartridge from the header
 * 
 * @param rom_size 
 * @return int 
 */
__attribute__((always_inline)) int get_rom_bank_count(u8 rom_size);

/**
 * @brief Get the size of the ROM in human readable format 
 * 
 * @param bank_count 
 * @return const char* 
 */
const char *get_rom_size_str(u8 rom_size);

/**
 * @brief Gets the number of RAM banks in the cartridge from the header
 * 
 * @param ram_size 
 * @return int 
 */
int get_ram_bank_count(u8 ram_size);

/**
 * @brief Get the size of RAM in human readable format 
 * 
 * @param bank_count 
 * @return const char* 
 */
const char *get_ram_size_str(u8 ram_size);

/**
 * @brief Get the destination of the cartridge
 * 
 * @param destination_code 
 * @return const char* 
 */
const char *get_destination_str(u8 destination_code);

/**
 * @brief Loads data from a given file into a buffer to read into a cart
 * 
 * @param file_path 
 * @param level 
 * @return u8* 
 */
u8 *load_data_from_file(const char *file_path, enum LogLevel level);

#endif // CARTRIDGE_HELPER_H
