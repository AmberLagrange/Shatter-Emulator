#include "core.hpp"

#include "mbc5.hpp"

MBC5::MBC5(std::vector<u8>&& rom, std::vector<u8>&& ram)
    : MBC(std::move(rom), std::move(ram)),
      m_RomBankNumber(0), m_RamBankNumber(0),
      m_RamEnabled(false), m_RTCEnabled(false) {}

MBC5::~MBC5() = default;

auto MBC5::read(u16 address) const -> u8
{
    switch(address & 0xE000)
    {
        case 0x0000:
        case 0x2000:
            return m_Rom[address];
        case 0x4000:
        case 0x6000:
            return m_Rom[(address - ROM_BANK_OFFSET) + ROM_BANK_SIZE * m_RomBankNumber];
        case 0xA000:
            return m_Ram[(address - RAM_BANK_OFFSET) + RAM_BANK_SIZE * m_RamBankNumber];
        default:
            WARN("Trying to read from address 0x" << std::hex << std::setw(4) << std::setfill('0') << address << '!');
            return 0xFF;
    }
}

void MBC5::write(u16 address, u8 val)
{
    switch(address & 0xE000)
    {
        case 0x0000: // RAM Enable
            if(val == 0x0A) { m_RamEnabled = true; }
            else if(val == 0x00) { m_RamEnabled = false; }
            break;
        case 0x2000: // ROM Bank Switching: Bits 0-7
            m_RomBankNumber = (m_RomBankNumber & 0x10) | val;
            break;
        case 0x3000: // Rom Bank Switching: Bit 8
            if(val) { m_RomBankNumber = m_RomBankNumber | 0b100000000; }
            break;
        case 0x4000: // RAM Bank Switching
            if(val <= 0x03)
            {
                m_RamBankNumber = val;
                m_RTCEnabled = false;
            }
            else if(0x80 <= val && val <= 0x0C)
            {
                m_RTCEnabled = true;
                // TODO: RTC
            }
            break;
        case 0x6000: // TODO: RTC
            break;
        case 0xA000:
            if(!m_RamEnabled) return;

            if(m_RTCEnabled)
            {
                // TODO: RTC
            }
            else
            {
                m_Ram[(address - RAM_BANK_OFFSET) + RAM_BANK_SIZE * m_RamBankNumber] = val;
            }
            break;
        default:
            WARN("Trying to write 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(val)
                  << " to address 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(val) << '!');
    }
}
