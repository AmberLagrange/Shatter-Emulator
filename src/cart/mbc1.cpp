#include "core.hpp"

#include "mbc1.hpp"
#include <stdexcept>

MBC1::MBC1(std::vector<u8>&& rom, std::vector<u8>&& ram)
    : MBC(std::move(rom), std::move(ram)),
      m_RomBankNumber(1), m_RamBankNumber(0), m_RamEnabled(false) {}

MBC1::~MBC1() = default;

auto MBC1::read(u16 address) const -> u8
{
    switch(address & 0xE000)
    {
        case 0x4000:
        case 0x6000:
            try
            {
                return m_Rom.at((address - ROM_BANK_OFFSET) + ROM_BANK_SIZE * m_RomBankNumber);
            }
            catch(std::out_of_range&)
            {
                ERROR("Tried to read out of bounds from ROM at address 0x" << std::setw(4) << std::hex << address << "!");
            }
            return 0xFF;

        case 0xA000:
            try
            {
                return m_Ram.at((address - RAM_BANK_OFFSET) + RAM_BANK_SIZE * m_RamBankNumber);
            }
            catch(std::out_of_range&)
            {
                ERROR("Tried to read out of bounds from RAM at address 0x"
                      << std::setw(4) << std::hex << address << "!");
            }
            return 0xFF;
            
        default:
            return m_Rom[address];
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
                try
                {
                    m_Ram.at((address - RAM_BANK_OFFSET) + RAM_BANK_SIZE * m_RamBankNumber) = val;
                }
                catch(std::out_of_range&)
                {
                    ERROR("Tried to write 0x" << std::setw(2) << std::setfill('0')
                          << std::hex << static_cast<u32>(val)
                          << " out of bounds from RAM at address 0x"
                          << std::setw(4) << std::setfill('0') << std::hex << address << "!");
                }
            }
    }
}
