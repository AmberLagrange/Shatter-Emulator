#include "core.hpp"

#include "mbc3.hpp"

MBC3::MBC3()
    : m_BankNumber(0) {}

auto MBC3::read(u16 address) const -> u8
{
    return (address < BANK_SIZE) ? m_Data.at(address) : m_Data.at(address + BANK_SIZE * m_BankNumber);
}

void MBC3::write(u16 address, u8 val)
{
    if(address < BANK_SIZE) // ROM Bank Swapping
    {
        m_BankNumber = (val & 0x1F) - 1; //NOLINT(cppcoreguidelines-avoid-magic-numbers)
    }
}
