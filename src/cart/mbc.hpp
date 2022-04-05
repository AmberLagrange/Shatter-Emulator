#pragma once

#include "core.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>

class MBC
{
    public:
        MBC() = default;

        /**
         * @brief Load a rom file into memory
         * 
         * @param path The filepath to the rom 
         */
        virtual void load(const char* path);

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
        std::vector<u8> m_Data;
};
