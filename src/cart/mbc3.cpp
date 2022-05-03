#include "core.hpp"

#include "mbc3.hpp"

MBC3::MBC3()
    : m_RomBankNumber(1), m_RamBankNumber(0), m_RamEnabled(false) {}

auto MBC3::read(u16 address) const -> u8
{
    switch(address & 0xE000)
    {
        case 0x4000:
        case 0x6000:
            return m_Data[(address - BANK_SIZE) + BANK_SIZE * m_RomBankNumber];
        case 0xA000:
            return  m_Ram[(address - 0xA000)    + BANK_SIZE * m_RamBankNumber];
        default:
            return m_Data[address];
    }
    
}

void MBC3::write(u16 address, u8 val)
{
    switch(address & 0xE000)
    {
        case 0x0000: // RAM Enable
            m_RamEnabled = (val == 0x0A);
            break;
        case 0x2000: // ROM Bank Switching
            m_RomBankNumber = (val & 0x1F);
            break;
        case 0x4000: // RAM Bank Switching
            m_RamBankNumber = (val & 0x03);
            break;
        case 0x6000: // TODO:
            break;
        case 0xA000:
            if(m_RamEnabled)
            {
                m_Ram[(address - 0xA000) + BANK_SIZE * m_RamBankNumber] = val;
            }
    }
}
