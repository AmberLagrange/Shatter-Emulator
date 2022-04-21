#pragma once

#include "core.hpp"

#include <array>

#include "mbc.hpp"

class MBC3 : public MBC
{
    public:
        MBC3();
        
        /**
         * @brief Reads a byte from the specified memory address
         * 
         * @param address The address to read from
         * @return The value stored at that address
         */
        [[nodiscard]] virtual auto read(u16 address) const -> u8 override;

        /**
         * @brief Writes a byte at the specified memory address
         * 
         * @param address The address to write to
         * @param val The value to write
         */
        virtual void write(u16 address, u8 val) override;

    private:
        u8 m_RomBankNumber;

        u8 m_RamBankNumber;
        bool m_RamEnabled = false;

        std::array<u8, 0x8000 * 4> m_Ram {{}};
};
