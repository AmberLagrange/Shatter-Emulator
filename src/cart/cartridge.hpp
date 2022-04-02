#pragma once

#include "core.hpp"

#include <array>
#include <vector>

class Cartridge
{
    public:
        Cartridge() = default;
        Cartridge(const char* path);

        /**
         * @brief Load a rom file into memory
         * 
         * @param path The filepath to the rom 
         */
        void load(const char* path);

        /**
         * @brief Swap the rom bank to the desired rom bank number
         * 
         * @param bankNumber The rom bank number to swap to
         */
        void swapBank(u8 bankNumber);

        /**
         * @brief Read a value from the rom at a specific location
         * 
         * @param address The address to read from
         * @return the value stored at the given address
         */
        [[nodiscard]] auto read(u16 address) const -> u8;

    private:
        std::vector<u8> contents;

        std::array<u8, BANK_SIZE> m_Rom0 {{}};
        std::array<u8, BANK_SIZE> m_Rom1 {{}};
};
