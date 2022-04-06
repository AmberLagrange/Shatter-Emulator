#pragma once

#include "core.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

using Rom = std::vector<u8>;

namespace Cart
{
    enum Type
    {
        ROM_ONLY     = 0x00,
        MBC1         = 0x01,
        MBC1_RAM     = 0x02,
        MBC1_RAM_BAT = 0x03,
        MBC2         = 0x05,
        MBC2_BAT     = 0x06,
    };
}

class MBC
{
    public:
        static auto loadRom(const char* path) -> Rom;
    public:
        MBC() = default;

        /**
         * @brief Load a rom file into memory
         * 
         * @param path The filepath to the rom 
         */
        virtual void load(const char* path);

        /**
         * @brief Load a previously loaded rom file into the MBC
         * 
         * @param data The data already loaded
         */
        virtual void load(const Rom& data);

        /**
         * @brief Reads a byte from the specified memory address
         * 
         * @param address The address to read from
         * @return The value stored at that address
         */
        [[nodiscard]] virtual auto read(u16 address) const -> u8;

        /**
         * @brief Writes a byte at the specified memory address
         * 
         * @param address The address to write to
         * @param val The value to write
         */
        virtual void write(u16 address, u8 val);
    protected:
        Rom m_Data;
};
