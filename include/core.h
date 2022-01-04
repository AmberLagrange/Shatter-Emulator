#pragma once

#define CLOCK_SPEED 4194304

#include "logger.h"

#include "utils.h"

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
                                CRITICAL(EXE_NAME << ": " << __FILE__ << ":" << __LINE__ << ": "    \
                                << __PRETTY_FUNCTION__ << ": Assertion `" << #x << "' failed.");    \
                                CRITICAL(msg);                                                      \
                                std::abort();                                                       \
                            }
#else
    #define ASSERT(x, msg) ((void)0)
#endif

#define DIV_REGISTER    0xFF04
#define TIMA_REGISTER   0xFF05
#define TMA_REGISTER    0xFF06
#define TAC_REGISTER    0xFF07

#define SCR_Y_REGISTER  0xFF42
#define SCR_X_REGISTER  0xFF43

#define IF_REGISTER     0xFF0F
#define IE_REGISTER     0xFFFF

#define GAMEBOY_WIDTH   160
#define GAMEBOY_HEIGHT  144

#define VRAM_WIDTH      0xFF
#define VRAM_HEIGHT     0xFF

#define CLOCK_HZ        4194304

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;