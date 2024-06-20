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

#endif
