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

namespace bit_functions
{
    inline auto get_bit(u8 x, u8 n)              -> bool { return (x >> n) & 0x01; }
    inline auto set_bit(u8& x, u8 n)             -> u8&  { return x |= (0x01 << n); }
    inline auto clear_bit(u8& x, u8 n)           -> u8&  { return x &= ~(0x01 << n); }
    inline auto flip_bit(u8& x, u8 n)            -> u8&  { return x ^= (0x01 << n); }
    inline auto set_bit_to(u8& x, u8 n, bool v)  -> u8&  { return (v ? set_bit(x, n) : clear_bit(x, n)); }
}

//Clock and Timers
constexpr u32 CLOCK_SPEED       = 4194304;

constexpr u32 TIMER_SPEED_00    = 1024;
constexpr u32 TIMER_SPEED_01    = 16;
constexpr u32 TIMER_SPEED_10    = 64;
constexpr u32 TIMER_SPEED_11    = 256;

//Opcode
constexpr u8 CB_OPCODE          = 0xCB;

//Reset Vectors
constexpr u8 VBLANK_VECTOR      = 0x40;
constexpr u8 LCD_STAT_VECTOR    = 0x48;
constexpr u8 TIMER_VECTOR       = 0x50;
constexpr u8 SERIAL_VECTOR      = 0x58;
constexpr u8 JOYPAD_VECTOR      = 0x60;

constexpr u8 RST_0x00           = 0x0000;
constexpr u8 RST_0x08           = 0x0008;
constexpr u8 RST_0x10           = 0x0010;
constexpr u8 RST_0x18           = 0x0018;
constexpr u8 RST_0x20           = 0x0020;
constexpr u8 RST_0x28           = 0x0028;
constexpr u8 RST_0x30           = 0x0030;
constexpr u8 RST_0x38           = 0x0038;


//LCD Info
constexpr u8  STAT_LCY_LY_BIT   = 2;
constexpr u8  STAT_HBLANK_BIT   = 3;
constexpr u8  STAT_VBLANK_BIT   = 4;
constexpr u8  STAT_OAM_BIT      = 5;
constexpr u8  STAT_LYC_BIT      = 6;

//Graphics Data
constexpr u16 TILE_DATA_LOW     = 0x8800;
constexpr u16 TILE_DATA_HIGH    = 0x8000;

constexpr u8  TILE_ONE_OFFSET   = 128;

constexpr u16 TILE_MAP_LOW      = 0x9800;
constexpr u16 TILE_MAP_HIGH     = 0x9C00;

constexpr u16 BG_WIDTH          = 0x0100;
constexpr u16 BG_HEIGHT         = 0x0100;

constexpr u8  TILE_WIDTH        = 8;
constexpr u8  TILE_HEIGHT       = 8;
constexpr u8  TILES_PER_LINE    = 32;
constexpr u8  BYTES_PER_TILE    = 16;

constexpr u8  BYTES_PER_SPRITE  = 4;

constexpr u8  GAMEBOY_WIDTH      = 160;
constexpr u8  GAMEBOY_HEIGHT     = 144;
constexpr u8  VBLANK_HEIGHT      = 154;

constexpr u32 FRAME_BUFFER_SIZE = GAMEBOY_WIDTH * GAMEBOY_HEIGHT * 4;

//Joypad Info
/**
    https://gbdev.io/pandocs/Joypad_Input.html
**/

constexpr u8  P10_INPUT_BIT     = 0; //Right or A        (0=Pressed) (Read Only)
constexpr u8  P11_INPUT_BIT     = 1; //Left  or B        (0=Pressed) (Read Only)
constexpr u8  P12_INPUT_BIT     = 2; //Up    or Select   (0=Pressed) (Read Only)
constexpr u8  P13_INPUT_BIT     = 3; //Down  or Start    (0=Pressed) (Read Only)
constexpr u8  P14_INPUT_BIT     = 4; //Direction buttons (0=Select)
constexpr u8  P15_INPUT_BIT     = 5; //Action buttons    (0=Select)

//Cartridge Header
constexpr u16 CART_TITLE        = 0x134;
constexpr u16 CART_TITLE_SIZE   = 16;

constexpr u16 CART_TYPE         = 0x0147;

//MMU Addresses
constexpr u32 ROM_SIZE          = 0x8000;
constexpr u32 RAM_SIZE          = 0x8000;
constexpr u32 ROM_BANK_SIZE     = 0x4000;
constexpr u32 ROM_BANK_OFFSET   = 0x4000;
constexpr u32 RAM_BANK_SIZE     = 0x2000;
constexpr u32 RAM_BANK_OFFSET   = 0xA000;
constexpr u32 INTERNAL_RAM_SIZE = 0x2000;

constexpr u16 ROM_ADDR          = 0x8000;
constexpr u16 VRAM_ADDR         = 0xA000;
constexpr u16 RAM_BANK_ADDR     = 0xC000;
constexpr u16 INTERNAL_RAM_ADDR = 0xE000;
constexpr u16 ECHO_RAM_ADDR     = 0xFE00;
constexpr u16 OAM_ADDR          = 0xFEA0;
constexpr u16 UNUSABLE_ADDR     = 0xFF00;
constexpr u16 IO_ADDR           = 0xFF80;
constexpr u16 IF_REGISTER       = 0xFF0F;
constexpr u16 IE_REGISTER       = 0xFFFF;

// IO Registers

constexpr u16 JOYPAD_REGISTER           = 0xFF00;

constexpr u16 SERIAL_DATA_REGISTER      = 0xFF01;
constexpr u16 SERIAL_CONTROL_REGISTER   = 0xFF02;

// 0xFF03 UNUSED

constexpr u16 TIMER_DIV_REGISTER        = 0xFF04;
constexpr u16 TIMER_TIMA_REGISTER       = 0xFF05;
constexpr u16 TIMER_TMA_REGISTER        = 0xFF06;
constexpr u16 TIMER_TAC_REGISTER        = 0xFF07;

// 0xFF08 UNUSED
// 0xFF09 UNUSED
// 0xFF0A UNUSED
// 0xFF0B UNUSED
// 0xFF0C UNUSED
// 0xFF0D UNUSED
// 0xFF0E UNUSED
// 0xFF0F UNUSED

constexpr u16 CHANNEL1_SWEEP_REGISTER   = 0xFF10;
constexpr u16 CHANNEL1_LENGTH_REGISTER  = 0xFF11;
constexpr u16 CHANNEL1_VOLUME_REGISTER  = 0xFF12;
constexpr u16 CHANNEL1_FREQ_LO_REGISTER = 0xFF13;
constexpr u16 CHANNEL1_FREQ_HI_REGISTER = 0xFF14;

// 0xFF15 UNUSED

constexpr u16 CHANNEL2_LENGTH_REGISTER  = 0xFF16;
constexpr u16 CHANNEL2_VOLUME_REGISTER  = 0xFF17;
constexpr u16 CHANNEL2_FREQ_LO_REGISTER = 0xFF18;
constexpr u16 CHANNEL2_FREQ_HI_REGISTER = 0xFF19;

constexpr u16 CHANNEL3_TOGGLE_REGISTER  = 0xFF1A;
constexpr u16 CHANNEL3_LENGTH_REGISTER  = 0xFF1B;
constexpr u16 CHANNEL3_VOLUME_REGISTER  = 0xFF1C;
constexpr u16 CHANNEL3_FREQ_LO_REGISTER = 0xFF1D;
constexpr u16 CHANNEL3_FREQ_HI_REGISTER = 0xFF1E;

// 0xFF1F UNUSED

constexpr u16 CHANNEL4_LENGTH_REGISTER  = 0xFF20;
constexpr u16 CHANNEL4_VOLUME_REGISTER  = 0xFF21;
constexpr u16 CHANNEL4_POLY_REGISTER    = 0xFF22;
constexpr u16 CHANNEL4_COUNTER_REGISTER = 0xFF23;

constexpr u16 CHANNEL_CONTROL_REGISTER  = 0xFF24;
constexpr u16 SOUND_OUTPUT_REGISTER     = 0xFF25;
constexpr u16 SOUND_TOGGLE_REGISTER     = 0xFF26;

// 0xFF27 UNUSED
// 0xFF28 UNUSED
// 0xFF29 UNUSED
// 0xFF2A UNUSED
// 0xFF2B UNUSED
// 0xFF2C UNUSED
// 0xFF2D UNUSED
// 0xFF2E UNUSED
// 0xFF2F UNUSED

// 0xFF30 - 0xFF3F WAVE_PATTERN_RAM

constexpr u16 LCD_CONTROL_REGISTER      = 0xFF40;
constexpr u16 LCD_STAT_REGISTER         = 0xFF41;

constexpr u16 SCY_REGISTER              = 0xFF42;
constexpr u16 SCX_REGISTER              = 0xFF43;

constexpr u16 LY_REGISTER               = 0xFF44;
constexpr u16 LYC_REGISTER              = 0xFF45;

constexpr u16 DMA_TRANSFER_REGISTER     = 0xFF46;

constexpr u16 BG_PALLETTE_REGISTER      = 0xFF47;
constexpr u16 OBJ_0_PALLETTE_REGISTER   = 0xFF48;
constexpr u16 OBJ_1_PALLETTE_REGISTER   = 0xFF49;

constexpr u16 WY_REGISTER               = 0xFF4A;
constexpr u16 WX_REGISTER               = 0xFF4B;

// 0xFF4C UNUSED

//Gameboy Reset Values
constexpr u16 AF_RESET = 0x01B0;
constexpr u16 BC_RESET = 0x0013;
constexpr u16 DE_RESET = 0x00D8;
constexpr u16 HL_RESET = 0x014D;
constexpr u16 SP_RESET = 0xFFFE;
constexpr u16 PC_RESET = 0x0100;
