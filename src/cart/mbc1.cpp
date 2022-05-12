#include "core.hpp"

#include "mbc1.hpp"

MBC1::MBC1(std::vector<u8>&& rom)
    : MBC(std::move(rom)),
      m_RomBankNumber(1), m_RamBankNumber(0), m_RamEnabled(false) {}

MBC1::~MBC1() = default;

auto MBC1::read(u16 address) const -> u8
{
    switch(address & 0xE000)
    {
        case 0x4000:
        case 0x6000:
            return m_Rom.at((address - ROM_BANK_OFFSET) + ROM_BANK_SIZE * m_RomBankNumber);
        case 0xA000:
            return m_Ram.at((address - RAM_BANK_OFFSET) + RAM_BANK_SIZE * m_RamBankNumber);
        default:
            return m_Rom.at(address);
    }
    
}

void MBC1::write(u16 address, u8 val)
{
    switch(address & 0xE000)
    {
        case 0x0000: //RAM Enable
            m_RamEnabled = (val == 0x0A);
            break;
        case 0x2000: //ROM Bank Switching
            m_RomBankNumber = (val & 0x1F);

            if(m_RomBankNumber == 0x00 || m_RomBankNumber == 0x20 ||
               m_RomBankNumber == 0x40 || m_RomBankNumber == 0x60)
            {
                m_RomBankNumber++;
            }
            break;
        case 0x4000:
            if(val <= 0x03) m_RamBankNumber = (val & 0x03);
            break;
        case 0x6000:
            break;
        case 0xA000:
            if(m_RamEnabled)
            {
                m_Ram.at((address - RAM_BANK_OFFSET) + RAM_BANK_SIZE * m_RamBankNumber) = val;
            }
    }
}
