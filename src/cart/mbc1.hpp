#pragma once

#include "mbc.hpp"

class MBC1 : public MBC
{
    public:
        MBC1();
        
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
        u8 m_BankNumber;
};
