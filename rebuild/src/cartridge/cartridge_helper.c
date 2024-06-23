#include "cartridge_helper.h"

#include <logging/logging.h>

bool check_cgb_support(u8 cgb_flag, const char **cgb_support_str) {

    switch (cgb_flag) {
        
        case CGB_SUPPORT_FLAG:
            *cgb_support_str = "Supported";
            return true;

        case CGB_REQUIRE_FLAG:
            *cgb_support_str = "Required";
            return true;

        default:
            *cgb_support_str = "Unsupported";
    }

    return false;
}

enum NewLicenseeCode convert_ascii_license_to_enum(u8 *new_licensee_code) {
    return TODO;
}

const char *get_licensee_str(u8 old_licensee_code, u8 *new_licensee_code) {

    switch ((enum OldLicenseeCode)(old_licensee_code)) {

        case LICENSE_NONE:
            return "None";

        case LICENSE_NINTENDO_1:
            return "Nintendo";

        case LICENSE_CAPCOM_1:
            return "Capcom";

        case LICENSE_HOT_B:
            return "HOT-B";

        case LICENSE_JALECO_1:
            return "Jaleco";

        case LICENSE_COCONUTS:
            return "Coconuts Japan";

        case LICENSE_ELITE_SYSTEMS_1:
            return "Elite Systems";

        case LICENSE_ELECTRONIC_ARTS_1:
            return "EA (Electronic Arts)";

        case LICENSE_HUDSONSOFT:
            return "Hudson Soft";

        case LICENSE_ITC_ENTERTAINMENT:
            return "ITC Entertainment";

        case LICENSE_YANOMAN:
            return "Yanoman";

        case LICENSE_CLARY:
            return "Japan Clary";

        case LICENSE_VIRGIN_1:
            return "Virgin Games Ltd.";

        case LICENSE_PCM_COMPLETE:
            return "PCM Complete";

        case LICENSE_SAN_X:
            return "San-X";

        case LICENSE_KOTOBUKI_SYSTEMS:
            return "Kemco";

        case LICENSE_SETA:
            return "SETA Corporation";

        case LICENSE_INFOGRAMES_1:
            return "Infogrames";

        case LICENSE_NINTENDO_2:
            return "Nintendo";

        case LICENSE_BANDAI_1:
            return "Bandai";

        case LICENSE_KONAMI_1:
            return "Konami";

        case LICENSE_HECTOR:
            return "HectorSoft";

        case LICENSE_CAPCOM_2:
            return "Capcom";

        case LICENSE_BANPRESTO_1:
            return "Banpresto";

        case LICENSE_ENTERTAINMENT_I:
            return ".Entertainment i";

        case LICENSE_GREMLIN:
            return "Gremlin";

        case LICENSE_UBI_SOFT:
            return "Ubi Soft";

        case LICENSE_ATLUS_1:
            return "Atlus";

        case LICENSE_MALIBU_1:
            return "Malibu Interactive";

        case LICENSE_ANGEL_1:
            return "Angel";

        case LICENSE_SPECTRUM_HOLOBY:
            return "Spectrum Holoby";

        case LICENSE_IREM:
            return "Irem";

        case LICENSE_VIRGIN_2:
            return "Virgin Games Ltd.";

        case LICENSE_MALIBU_2:
            return "Malibu Interactive";

        case LICENSE_U_S_GOLD:
            return "U.S. Gold";

        case LICENSE_ABSOLUTE:
            return "Absolute";

        case LICENSE_ACCLAIM_1:
            return "Acclaim Entertainment";

        case LICENSE_ACTIVISION:
            return "Activision";

        case LICENSE_AMERICAN_SAMMY:
            return "Sammy USA Corporation";

        case LICENSE_GAMETEK:
            return "GameTek";

        case LICENSE_PARK_PLACE:
            return "Park Place";

        case LICENSE_LJN_1:
            return "LJN";

        case LICENSE_MATCHBOX:
            return "Matchbox";

        case LICENSE_MILTON_BRADLEY:
            return "Milton Bradley Company";

        case LICENSE_MINDSCAPE:
            return "Mindscape";

        case LICENSE_ROMSTAR:
            return "Romstar";

        case LICENSE_NAXAT_SOFT_1:
            return "Naxat Soft";

        case LICENSE_TRADEWEST:
            return "Tradewest";

        case LICENSE_TITUS:
            return "Titus Interactive";

        case LICENSE_VIRGIN_3:
            return "Virgin Games Ltd.";

        case LICENSE_OCEAN:
            return "Ocean Software";

        case LICENSE_ELECTRONIC_ARTS_2:
            return "EA (Electronic Arts)";

        case LICENSE_ELITE_SYSTEMS_2:
            return "Elite Systems";

        case LICENSE_ELECTRO_BRAIN:
            return "Electro Brain";

        case LICENSE_INFOGRAMES_2:
            return "Infogrames";

        case LICENSE_INTERPLAY:
            return "Interplay Entertainment";

        case LICENSE_BRODERBUND_1:
            return "Broderbund";

        case LICENSE_SCULPTERED_SOFT:
            return "Sculptured Software";

        case LICENSE_THE_SALES_CURVE:
            return "The Sales Curve Limited";

        case LICENSE_T_HQ:
            return "THQ";

        case LICENSE_ACCOLADE:
            return "Accolade";

        case LICENSE_TRIFFIX_ENTERTAINMENT:
            return "Triffix Entertainment";

        case LICENSE_MICROPROSE:
            return "Microprose";

        case LICENSE_KEMCO_1:
            return "Kemco";

        case LICENSE_MISAWA_ENTERTAINMENT:
            return "Misawa Entertainment";

        case LICENSE_LOZC:
            return "Lozc";

        case LICENSE_TOKUMA_SHOTEN_I_1:
            return "Tokuma Shoten";

        case LICENSE_BULLET_PROOF_SOFTWARE:
            return "Bullet-Proof Software";

        case LICENSE_VIC_TOKAI:
            return "Vic Tokai";

        case LICENSE_APE:
            return "Ape";

        case LICENSE_I_MAX:
            return "I\'Max";

        case LICENSE_CHUN_SOFT:
            return "Chunsoft Co.";

        case LICENSE_VIDEO_SYSTEM:
            return "Video System";

        case LICENSE_TSUBURAVA:
            return "Tsubaraya Productions";

        case LICENSE_VARIE_1:
            return "Varie";

        case LICENSE_YONEZAWAS_PAL:
            return "Yonezawa/S\'Pal";

        case LICENSE_KANEKO:
            return "Kemco";

        case LICENSE_ARC:
            return "Arc";

        case LICENSE_NIHON_BUSSAN:
            return "Nihon Bussan";

        case LICENSE_TECMO:
            return "Tecmo";

        case LICENSE_IMAGINEER:
            return "Imagineer";

        case LICENSE_BANPRESTO_2:
            return "Banpresto";

        case LICENSE_NOVA:
            return "Nova";

        case LICENSE_HORI_ELECTRIC:
            return "Hori Electric";

        case LICENSE_BANDAI_2:
            return "Bandai";

        case LICENSE_KONAMI_2:
            return "Konami";

        case LICENSE_KAWADA:
            return "Kawada";

        case LICENSE_TAKARA:
            return "Takara";

        case LICENSE_TECHNOS_JAPAN:
            return "Technos Japan";

        case LICENSE_BRODERBUND_2:
            return "Broderbund";

        case LICENSE_TOEI_ANIMATION:
            return "Toei Animation";

        case LICENSE_TOHO:
            return "Toho";

        case LICENSE_NAMCO:
            return "Namco";

        case LICENSE_ACCLAIM_2:
            return "Acclaim Entertainment";

        case LICENSE_ASCII_OR_NEXSOFT:
            return "ASCII Corporation or Nexsoft";

        case LICENSE_BANDAI_3:
            return "Bandai";

        case LICENSE_ENIX:
            return "Square Enix";

        case LICENSE_HAL:
            return "HAL Laboratory";

        case LICENSE_SNK:
            return "SNK";

        case LICENSE_PONY_CANYON:
            return "Pony Canyon";

        case LICENSE_CULTURE_BRAIN_O:
            return "Culture Brain";

        case LICENSE_SUNSOFT:
            return "Sunsoft";

        case LICENSE_SONY_IMAGESOFT:
            return "Sony Imagesoft";

        case LICENSE_SAMMY:
            return "Sammy Corporation";

        case LICENSE_TAITO_1:
            return "Taito";

        case LICENSE_KEMCO_2:
            return "Kemco";

        case LICENSE_SQUARESOFT:
            return "Square";

        case LICENSE_TOKUMA_SHOTEN_I_2:
            return "Tokuma Shoten";

        case LICENSE_DATA_EAST:
            return "Data East";

        case LICENSE_TONKIN_HOUSE:
            return "Tonkinhouse";

        case LICENSE_KOEI:
            return "Koei";

        case LICENSE_UFL:
            return "UFL";

        case LICENSE_ULTRA:
            return "Ultra";

        case LICENSE_VAP:
            return "Vap";

        case LICENSE_USE:
            return "Use Corporation";

        case LICENSE_MELDAC:
            return "Meldac";

        case LICENSE_PONY_CANYON_OR:
            return "Pony Canyon";

        case LICENSE_ANGEL_2:
            return "Angel";

        case LICENSE_TAITO_2:
            return "Taito";

        case LICENSE_SOFEL:
            return "Sofel";

        case LICENSE_QUEST:
            return "Quest";

        case LICENSE_SIGMA_ENTERPRISES:
            return "Sigma Enterprises";

        case LICENSE_ASK_KODANSHA:
            return "ASK Kodansha Co.";

        case LICENSE_NAXAT_SOFT_2:
            return "Naxat Soft";

        case LICENSE_COPYA_SYSTEMS:
            return "Copya System";

        case LICENSE_BANPRESTO_3:
            return "Banpresto";

        case LICENSE_TOMY:
            return "Tomy";

        case LICENSE_LJN_2:
            return "LJN";

        case LICENSE_NCS:
            return "NCS";

        case LICENSE_HUMAN:
            return "Human";

        case LICENSE_ALTRON:
            return "Altron";

        case LICENSE_JALECO_2:
            return "Jaleco";

        case LICENSE_TOWACHIKI:
            return "Towa Chiki";

        case LICENSE_UUTAKA:
            return "Yutaka";

        case LICENSE_VARIE_2:
            return "Varie";

        case LICENSE_EPOCH:
            return "Epcoh";

        case LICENSE_ATHENA:
            return "Athena";

        case LICENSE_ASMIK:
            return "Asmik Ace Entertainment";

        case LICENSE_NATSUME:
            return "Natsume";

        case LICENSE_KING_RECORDS:
            return "King Records";

        case LICENSE_ATLUS_2:
            return "Atlus";

        case LICENSE_EPIC_SONY_RECORDS:
            return "Epic/Sony Records";

        case LICENSE_IGS:
            return "IGS";

        case LICENSE_A_WAVE:
            return "A Wave";

        case LICENSE_EXTREME_ENTERTAINMENT:
            return "Extreme Entertainment";

        case LICENSE_LJN_3:
            return "LJN";

        default:
            // Fallthrough to check for new licensee code
            break;
    }

    if (old_licensee_code != LICENSE_NEW_LICENSEE_CODE) {
        gameboy_log(LOG_WARN, "Invalid licensee code given: 0x%02X", old_licensee_code);
        return "Invalid";
    }

    switch (convert_ascii_license_to_enum(new_licensee_code)) {

        case TODO:
            return "TODO: New Licensee Codes";
    }

    gameboy_log(LOG_WARN, "Invalid licensee code given: %.2s", new_licensee_code);
    return "Invalid";
}

const char *get_cart_type_str(u8 cart_type) {

    switch ((enum CartridgeType)(cart_type)) {

        case ROM_ONLY:
            return "ROM Only";

        case MBC1:
            return "MBC1";

        case MBC1_RAM:
            return "MBC1 + RAM";

        case MBC1_RAM_BATTERY:
            return "MBC1 + RAM + Battery";

        case MBC2:
            return "MBC2";

        case MBC2_BATTERY:
            return "MBC2 + Battery";

        case ROM_RAM:
            return "ROM + RAM";

        case ROM_RAM_BATTERY:
            return "ROM + RAM + Battery";

        case MMM01:
            return "MMM01";

        case MMM01_RAM:
            return "MMM01 + RAM";

        case MMM01_RAM_BATTERY:
            return "MMM01 + RAM + Battery";

        case MBC3_TIMER_BATTERY:
            return "MBC3 + Timer + Battery";

        case MBC3_TIMER_RAM_BATTERY:
            return "MBC3 + Timer + RAM + Battery";

        case MBC3:
            return "MBC3";

        case MBC3_RAM:
            return "MBC3 + RAM";

        case MBC3_RAM_BATTERY:
            return "MBC3 + RAM + Battery";

        case MBC5:
            return "MBC5";

        case MBC5_RAM:
            return "MBC5 + RAM";

        case MBC5_RAM_BATTERY:
            return "MBC5 + RAM + Battery";

        case MBC5_RUMBLE:
            return "MBC5 + Rumble";

        case MBC5_RUMBLE_RAM:
            return "MBC5 + Rumble + RAM";

        case MBC5_RUMBLE_RAM_BATTERY:
            return "MBC5 + Rumble + RAM + Battery";

        case MBC6:
            return "MBC6";

        case MBC7_SENSOR_RUMBLE_RAM_BATTERY:
            return "MBC7 + Sensor + Rumble + RAM + Battery";

        case POCKET_CAMERA:
            return "Pocket Camera";

        case BANDAI_TAMA5:
            return "Bandai TAMA5";

        case HUC3:
            return "HuC-3";

        case HUC1_RAM_BATTERY:
            return "HuC-1 + Ram + Battery";

        default:
            // Fallthrough
            break;
    }

    gameboy_log(LOG_WARN, "Invalid cartridge type given: 0x%02X", cart_type);
}

// Note: This doesn't account for the 72, 80 and 96 ROMS banks
// However, no known ROMS have these, and no official documents list them
__attribute__((always_inline)) int get_rom_bank_count(u8 rom_size) {

    return 2 << rom_size;
}

const char *get_rom_size_str(u8 rom_size) {

    switch (rom_size) {

        case 0x00:
            return "32 KiB";

        case 0x01:
            return "64 KiB";

        case 0x02:
            return "128 KiB";

        case 0x03:
            return "256 KiB";

        case 0x04:
            return "512 KiB";

        case 0x05:
            return "1 MiB";

        case 0x06:
            return "2 MiB";

        case 0x07:
            return "4 MiB";

        case 0x08:
            return "8 MiB";

        default:
            // Fallthrough
            break;
    }

    gameboy_log(LOG_ERROR, "Unknown ROM size: 0x%02X", rom_size);
    return "Unknown ROM Size";
}

const char *get_ram_size_str(u8 ram_size) {

    switch (ram_size) {

        case 0x00:
            return "No RAM";

        case 0x01:
            gameboy_log(LOG_WARN, "Unused RAM Size.");
            return "Unused";

        case 0x02:
            return "8 KiB";

        case 0x03:
            return "32 KiB";

        case 0x04:
            return "128 KiB";

        case 0x05:
            return "64 KiB";

        default:
            // Fallthrough
            break;
    }

    gameboy_log(LOG_ERROR, "Unknown RAM size: 0x%02X", ram_size);
    return "Unknown RAM Size";
}

const char *get_destination_str(u8 destination_code) {

    switch (destination_code) {

        case 0x00:
            return "Japan (and possibly overseas)";

        case 0x01:
            return "Overseas";

        default:
            // Fallthrough
            break;
    }

    gameboy_log(LOG_ERROR, "Unknown destination: 0x%02X", destination_code);
    return "Unknown destination";
}
