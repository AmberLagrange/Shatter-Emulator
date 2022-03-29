#pragma once

#include "logging/logger.h"

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

#define CLOCK_SPEED     4194304

#define TIMER_SPEED_00  CLOCK_SPEED / 1024
#define TIMER_SPEED_01  CLOCK_SPEED / 16
#define TIMER_SPEED_10  CLOCK_SPEED / 64
#define TIMER_SPEED_11  CLOCK_SPEED / 256

#define DIV_REGISTER    0xFF04
#define TIMA_REGISTER   0xFF05
#define TMA_REGISTER    0xFF06
#define TAC_REGISTER    0xFF07

#define SCY_REGISTER    0xFF42
#define SCX_REGISTER    0xFF43

#define LY_REGISTER     0xFF44
#define LYC_REGISTER    0xFF45

#define WY_REGISTER     0xFF4A
#define WX_REGISTER     0xFF4B

#define IF_REGISTER     0xFF0F
#define IE_REGISTER     0xFFFF

#define GAMEBOY_WIDTH   160
#define GAMEBOY_HEIGHT  144

#define VRAM_WIDTH      0xFF
#define VRAM_HEIGHT     0xFF

#define FRAME_BUFFER_SIZE GAMEBOY_WIDTH * GAMEBOY_HEIGHT * 4
#define BG_BUFFER_SIZE    VRAM_WIDTH    * VRAM_HEIGHT    * 4

#define CLOCK_HZ        4194304

#include <cstdint>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;