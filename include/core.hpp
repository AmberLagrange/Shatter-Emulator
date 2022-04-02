#pragma once

#include "logging/logger.hpp"

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define IS_BIG_ENDIAN
#else
    #define IS_LITTLE_ENDIAN
#endif

#ifdef NDEBUG
    #define ASSERT(x, msg) ((void)0) //NOLINT(cppcoreguidelines-macro-usage)
#else
    #ifndef EXE_NAME
        #define EXE_NAME "executible" //NOLINT(cppcoreguidelines-macro-usage)
    #endif

    //NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define ASSERT(x, msg)                                                  \
    if(!x)                                                                  \
    {                                                                       \
        CRITICAL(EXE_NAME << ": " << __FILE__ << ":" << __LINE__ << ": "    \
        << __PRETTY_FUNCTION__ << ": Assertion `" << #x << "' failed.");    \
        CRITICAL(msg);                                                      \
        std::abort();                                                       \
    }
#endif

#include <cstdint>
#include <climits>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;

//TODO: Move constexpr into namespace

//Bit Functions
constexpr auto GET_BIT(u8 x, u8 n)    -> bool { return (x >> n) & 0x01; }
constexpr auto SET_BIT(u8& x, u8 n)   -> u8&  { return x |= (0x01 << n); }
constexpr auto CLEAR_BIT(u8& x, u8 n) -> u8&  { return x &= ~(0x01 << n); }
constexpr auto FLIP_BIT(u8& x, u8 n)  -> u8&  { return x ^= (0x01 << n); }

//Clock and Timers
constexpr u32 CLOCK_SPEED       = 4194304;

constexpr u32 TIMER_SPEED_00    = CLOCK_SPEED / 1024;
constexpr u32 TIMER_SPEED_01    = CLOCK_SPEED / 16;
constexpr u32 TIMER_SPEED_10    = CLOCK_SPEED / 64;
constexpr u32 TIMER_SPEED_11    = CLOCK_SPEED / 256;

//Opcode
constexpr u8 CB_OPCODE          = 0xCB;

//Reset Vectors
constexpr u8 VBLANK_VECTOR      = 0x40;
constexpr u8 LCD_STAT_VECTOR    = 0x48;
constexpr u8 TIMER_VECTOR       = 0x50;
constexpr u8 SERIAL_VECTOR      = 0x58;
constexpr u8 JOYPAD_VECTOR      = 0x60;

constexpr u8 RST_0x00           = 0x00;
constexpr u8 RST_0x08           = 0x08;
constexpr u8 RST_0x10           = 0x10;
constexpr u8 RST_0x18           = 0x18;
constexpr u8 RST_0x20           = 0x20;
constexpr u8 RST_0x28           = 0x28;
constexpr u8 RST_0x30           = 0x30;
constexpr u8 RST_0x38           = 0x38;

//Registers
constexpr u16 DIV_REGISTER      = 0xFF04;
constexpr u16 TIMA_REGISTER     = 0xFF05;
constexpr u16 TMA_REGISTER      = 0xFF06;
constexpr u16 TAC_REGISTER      = 0xFF07;

constexpr u16 LCD_CTRL_REGISTER = 0xFF40;
constexpr u16 LCD_STAT_REGISTER = 0xFF41;
constexpr u16 SCY_REGISTER      = 0xFF42;
constexpr u16 SCX_REGISTER      = 0xFF43;

constexpr u16 LY_REGISTER       = 0xFF44;
constexpr u16 LYC_REGISTER      = 0xFF45;

constexpr u16 WY_REGISTER       = 0xFF4A;
constexpr u16 WX_REGISTER       = 0xFF4B;

constexpr u16 IF_REGISTER       = 0xFF0F;
constexpr u16 IE_REGISTER       = 0xFFFF;

//Graphics Data
constexpr u16 TILE_MAP_LOW      = 0x9800;
constexpr u16 TILE_MAP_HIGH     = 0x9C00;
constexpr u16 TILE_DATA_LOW     = 0x8800;
constexpr u16 TILE_DATA_HIGH    = 0x8000;

constexpr u8 GAMEBOY_WIDTH      = 160;
constexpr u8 GAMEBOY_HEIGHT     = 144;

constexpr u8 VRAM_WIDTH         = 0xFF;
constexpr u8 VRAM_HEIGHT        = 0xFF;

constexpr u32 FRAME_BUFFER_SIZE = GAMEBOY_WIDTH * GAMEBOY_HEIGHT * 4;
constexpr u32 BG_BUFFER_SIZE    = VRAM_WIDTH    * VRAM_HEIGHT    * 4;

//MMU Addresses
constexpr u32 ROM_SIZE          = 0x8000;
constexpr u32 RAM_SIZE          = 0x8000;
constexpr u32 BANK_SIZE         = 0x4000;
constexpr u32 INTERNAL_RAM_SIZE = 0x2000;

constexpr u16 ROM_ADDR          = 0x8000;
constexpr u16 VRAM_ADDR         = 0xA000;
constexpr u16 RAM_BANK_ADDR     = 0xC000;
constexpr u16 INTERNAL_RAM_ADDR = 0xE000;
constexpr u16 ECHO_RAM_ADDR     = 0xFE00;
constexpr u16 OAM_ADDR          = 0xFEA0;
constexpr u16 UNUSABLE_ADDR     = 0xFF00;
constexpr u16 IO_ADDR           = 0xFF80;

//Gameboy Reset Values
constexpr u16 AF_RESET = 0x01B0;
constexpr u16 BC_RESET = 0x0013;
constexpr u16 DE_RESET = 0x00D8;
constexpr u16 HL_RESET = 0x014D;
constexpr u16 SP_RESET = 0xFFFE;
constexpr u16 PC_RESET = 0x0100;
