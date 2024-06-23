#ifndef CORE_H
#define CORE_H

// General C Macros
#define NULL ((void*)0)
#define MAX_STR_LEN 256

// Return type values
#define RETURN_OK 0
#define INIT_FAIL -1

// Endianness for register type punning
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define IS_BIG_ENDIAN
#else
    #define IS_LITTLE_ENDIAN
#endif

// Typedefs for common data types
#include <stdint.h>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

// DMG Reset values
#define AF_RESET 0x01B0
#define BC_RESET 0x0013
#define DE_RESET 0x00D8
#define HL_RESET 0x014D
#define SP_RESET 0xFFFE
#define PC_RESET 0x0100

// Memory addresses
#define MEMORY_SIZE                 0x10000
#define ROM_SIZE                    0x8000
#define RAM_SIZE                    0x8000

#define ROM_BANK_0_START            0x0000
#define ROM_BANK_0_END              0x3FFF
#define ROM_BANK_SWAPPABLE_START    0x4000
#define ROM_BANK_SWAPPABLE_END      0x7FFF
#define ROM_BANK_SIZE               0x4000

#define VIDEO_RAM_START             0x8000
#define VIDEO_RAM_END               0x9FFF
#define VIDEO_RAM_SIZE              0x2000
#define VIDEO_RAM_BANK_COUNT        0x0002  // CGB Mode allows for 2 vram banks

#define EXTERNAL_RAM_START          0xA000
#define EXTERNAL_RAM_END            0xBFFF
#define EXTERNAL_RAM_SIZE           0x2000

#define WORK_RAM_0_START            0xC000
#define WORK_RAM_0_END              0xCFFF
#define WORK_RAM_0_SIZE             0x1000

#define WORK_RAM_SWAPPABLE_START    0xD000  // CGB Mode allows swapping banks
#define WORK_RAM_SWAPPABLE_END      0xDFFF
#define WORK_RAM_SWAPPABLE_SIZE     0x1000
#define WORK_RAM_SWAPPABLE_COUNT    0x0007

#define ECHO_RAM_0_START            0xE000  // Prohibited by Nintendo
#define ECHO_RAM_0_END              0xEFFF
#define ECHO_RAM_0_SIZE             0x1000

#define ECHO_RAM_SWAPPABLE_START    0xF000  // Swappable echo ram just maps to swappable work ram
#define ECHO_RAM_SWAPPABLE_END      0xFDFF
#define ECHO_RAM_SWAPPABLE_SIZE     0x0E00

#define OAM_START                   0xFE00
#define OAM_END                     0xFE9F
#define OAM_SIZE                    0x00A0

#define NOT_USABLE_START            0xFEA0  // Prohibited by Nintendo
#define NOT_USABLE_END              0xFEFF
#define NOT_USABLE_SIZE             0x0060

#define IO_REGISTERS_START          0xFF00
#define IO_REGISTERS_END            0xFF7F
#define IO_REGISTERS_SIZE           0x0080

#define HIGH_RAM_START              0xFF80
#define HIGH_RAM_END                0xFFFE
#define HIGH_RAM_SIZE               0x007F

#define IE_REGISTER_ADDRESS         0xFFFF  // No need for size as it's one byte

// Cartridge Header
#define HEADER_START                0x0100
#define HEADER_END                  0x0150
#define HEADER_SIZE                 0x0050

#define ENTRY_LOCATION_START        0x0100
#define ENTRY_LOCATION_END          0x0103
#define ENTRY_LOCATION_SIZE         0x0004

#define NINTENDO_LOGO_START         0x0104
#define NINTENDO_LOGO_END           0x0133
#define NINTENDO_LOGO_SIZE          0x0030

#define CARTRIDGE_TITLE_START       0x0134
#define CARTRIDGE_TITLE_END         0x0143
#define CARTRIDGE_TITLE_SIZE        0x0010

#define CGB_CARTRIDGE_TITLE_START   0x0134
#define CGB_CARTRIDGE_TITLE_END     0x013E
#define CGB_CARTRIDGE_TITLE_SIZE    0x000B

#define MANUFACTURER_CODE_START     0x013F  // Note: In older cartridges, this is
#define MANUFACTURER_CODE_END       0x0142  // Part of the title.
#define MANUFACTURER_CODE_SIZE      0x0004  // TODO: Proper handling of this

#define CGB_FLAG_ADDRESS            0x0143  // See above

#define NEW_LICENSEE_CODE_START     0x0144
#define NEW_LICENSEE_CODE_END       0x0145
#define NEW_LICENSEE_CODE_SIZE      0x0002

#define SGB_FLAG_ADDRESS            0x0146

#define CARTRIDGE_TYPE_ADDRESS      0x0147

#define ROM_SIZE_ADDRESS            0x0148

#define RAM_SIZE_ADDRESS            0x0149

#define DESTINATION_CODE_ADDRESS    0x014A

#define OLD_LICENSEE_CODE_ADDRESS   0x014B

#define ROM_VERSION_NUMBER_ADDRESS  0x014C

#define HEADER_CHECKSUM_ADDRESS     0x014D

#define GLOBAL_CHECKSUM_START       0x014E
#define GLOBAL_CHECKSUM_END         0x014F
#define GLOBAL_CHECKSUM_SIZE        0x0002

// SGB
#define CGB_SUPPORT_FLAG            0x80
#define CGB_REQUIRE_FLAG            0xC0
#define SGB_ENABLE_FLAG             0x03

#endif
