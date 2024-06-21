#ifndef CORE_H
#define CORE_H

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
#define ROM_BANK_0_SIZE             0x4000

#define ROM_BANK_SWAPPABLE_START    0x4000
#define ROM_BANK_SWAPPABLE_END      0x7FFF
#define ROM_BANK_SWAPPABLE_SIZE     0x4000

#define VIDEO_RAM_START             0x8000
#define VIDEO_RAM_END               0x9FFF
#define VIDEO_RAM_SIZE              0x2000
#define VIDEO_RAM_BANK_COUNT        0x0002  // GBC Mode allows for 2 vram banks

#define EXTERNAL_RAM_START          0xA000
#define EXTERNAL_RAM_END            0xBFFF
#define EXTERNAL_RAM_SIZE           0x2000

#define WORK_RAM_0_START            0xC000
#define WORK_RAM_0_END              0xCFFF
#define WORK_RAM_0_SIZE             0x1000

#define WORK_RAM_SWAPPABLE_START    0xD000  // GBC Mode allows swapping banks
#define WORK_RAM_SWAPPABLE_END      0xDFFF
#define WORK_RAM_SWAPPABLE_SIZE     0x1000
#define WORK_RAM_SWAPPABLE_COUNT    0x0007  // Treating the swappable work ram as a continuous 

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

#define INTERUPT_ENABLE_REGISTER    0xFFFF  // No need for size as it's one byte

#endif
