#include "core.hpp"

#include "mbc1.hpp"

MBC1::MBC1()
    : m_BankNumber(0) {}

auto MBC1::read(u16 address) const -> u8
{
    return (address < BANK_SIZE) ? m_Data.at(address) : m_Data.at(address + BANK_SIZE * m_BankNumber);
}

void MBC1::write(u16 address, u8 val)
{
    if(address < BANK_SIZE) // ROM Bank Swapping
    {
        m_BankNumber = (val & 0x1F) - 1; //NOLINT(cppcoreguidelines-avoid-magic-numbers)
    }
}
