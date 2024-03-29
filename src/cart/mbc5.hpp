#pragma once

#include "core.hpp"

#include "mbc.hpp"

class MBC5 final : public MBC
{
    public:
        MBC5(std::vector<u8>&& rom, std::vector<u8>&& ram);
        ~MBC5() final;
        
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
    private:
        u16 m_RomBankNumber;
        u8  m_RamBankNumber;

        bool m_RamEnabled;
        bool m_RTCEnabled;
};
