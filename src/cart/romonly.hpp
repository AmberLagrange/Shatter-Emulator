#pragma once

#include "core.hpp"

#include "mbc.hpp"

class RomOnly final : public MBC
{
    public:
        RomOnly(std::vector<u8>&& rom);
        ~RomOnly() final;
        
        /**
         * @brief Reads a byte from the specified memory address
         * 
         * @param address The address to read from
         * @return The value stored at that address
         */
        [[nodiscard]] virtual auto read(u16 address) const -> u8 final;

        /**
         * @brief Writes a byte at the specified memory address
         * 
         * @param address The address to write to
         * @param val The value to write
         */
        virtual void write(u16 address, u8 val) final;
};
