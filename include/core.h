#pragma once

#define CLOCK_SPEED 4194304

#include "log.h"

#ifndef NDEBUG
    #ifndef EXE_NAME
        #define EXE_NAME "executible"
    #endif

    #define ASSERT(x, msg)  if(!x)\
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