#pragma once

#include "core.hpp"

#include <array>
#include <memory>

#include "cart/romonly.hpp"
#include "cart/mbc1.hpp"
#include "cart/mbc3.hpp"

class Gameboy;

class MMU
{
    public:
        MMU(Gameboy& gb);

        /**
         * @brief Loads a rom into memory
         * 
         * @param path The filepath to the rom
         */
        void load(const char* path);

        /**
         * @brief Reads a byte from the specified memory address
         * 
         * @param address The address to read from
         * @return The value stored at that address
         */
        [[nodiscard]] auto read(u16 address) const -> u8;

        /**
         * @brief Writes a byte at the specified memory address
         * 
         * @param address The address to write to
         * @param val The value to write
         */
        void write(u16 address, u8 val);
    private:
        void dmaTransfer(u8 val);
    private:
        Gameboy& m_Gameboy;
        std::unique_ptr<MBC> m_Rom;

        std::array<u8, RAM_SIZE> m_Memory {{}};
};
