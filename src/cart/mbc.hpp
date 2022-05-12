#pragma once

#include "core.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

namespace Cart
{
    enum Type
    {
        ROM_ONLY                       = 0x00,
        MBC1                           = 0x01,
        MBC1_RAM                       = 0x02,
        MBC1_RAM_BATTERY               = 0x03,
        MBC2                           = 0x05,
        MBC2_BATTERY                   = 0x06,
        ROM_RAM_1                      = 0x08,
        ROM_RAM_BATTERY_1              = 0x09,
        MMM01                          = 0x0B,
        MMM01_RAM                      = 0x0C,
        MMM01_RAM_BATTERY              = 0x0D,
        MBC3_TIMER_BATTERY             = 0x0F,
        MBC3_TIMER_RAM_BATTERY_2       = 0x10,
        MBC3                           = 0x11,
        MBC3_RAM_2                     = 0x12,
        MBC3_RAM_BATTERY_2             = 0x13,
        MBC5                           = 0x19,
        MBC5_RAM                       = 0x1A,
        MBC5_RAM_BATTERY               = 0x1B,
        MBC5_RUMBLE                    = 0x1C,
        MBC5_RUMBLE_RAM                = 0x1D,
        MBC5_RUMBLE_RAM_BATTERY        = 0x1E,
        MBC6                           = 0x20,
        MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,
        POCKET_CAMERA                  = 0xFC,
        BANDAI_TAMA5                   = 0xFD,
        HUC3                           = 0xFE,
        HUC1_RAM_BATTERY               = 0xFF
    };
}

class MBC
{
    public:
        /**
         * @brief Get the MBC of the cart
         * 
         * @param data The rom's data
         */
        [[nodiscard]] static auto getCartType(const std::vector<u8>& data) -> Cart::Type;

        /**
         * @brief Get the title of the cart
         * 
         * @param data The rom's data
         */
        [[nodiscard]] static auto getCartTitle(const std::vector<u8>& data) -> const std::string;

        /**
         * @brief Load a rom into memory
         * 
         * @param path The path to the rom
         */
        [[nodiscard]] static auto load(const char* path) -> std::vector<u8>;
    public:
        MBC(std::vector<u8>&& rom);
        virtual ~MBC();

        /**
         * @brief Reads a byte from the specified memory address
         * 
         * @param address The address to read from
         * @return The value stored at that address
         */
        [[nodiscard]] virtual auto read(u16 address) const -> u8 = 0;

        /**
         * @brief Writes a byte at the specified memory address
         * 
         * @param address The address to write to
         * @param val The value to write
         */
        virtual void write(u16 address, u8 val) = 0;
    protected:
        std::vector<u8> m_Rom;
};
