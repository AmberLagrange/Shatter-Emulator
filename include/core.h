#pragma once

#define CLOCK_SPEED 4194304

#include "log.h"

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define IS_BIG_ENDIAN
#else
    #define IS_LITTLE_ENDIAN
#endif

#ifndef NDEBUG
    #ifndef EXE_NAME
        #define EXE_NAME "executible"
    #endif

    #define ASSERT(x, msg)  if(!x)                                                                  \
                            {                                                                       \
                                ERROR(EXE_NAME << ": " << __FILE__ << ":" << __LINE__ << ": "       \
                                << __PRETTY_FUNCTION__ << ": Assertion `" << #x << "' failed.");    \
                                ERROR(msg);                                                         \
                                std::abort();                                                       \
                            }
#else
    #define ASSERT(x, msg) ((void)0)
#endif

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

typedef int8_t i8;
typedef int16_t i16;