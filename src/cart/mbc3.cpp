#include "core.hpp"

#include "mbc3.hpp"

MBC3::MBC3(std::vector<u8>&& rom, std::vector<u8>&& ram)
    : MBC(std::move(rom), std::move(ram)),
      m_RomBankNumber(1), m_RamBankNumber(0),
      m_RamEnabled(false), m_RTCEnabled(false) {}

MBC3::~MBC3() = default;

auto MBC3::read(u16 address) const -> u8
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

void MBC3::write(u16 address, u8 val)
{
    switch(address & 0xE000)
    {
        case 0x0000: // RAM Enable
            if(val == 0x0A) { m_RamEnabled = true; }
            else if(val == 0x00) { m_RamEnabled = false; }
            break;
        case 0x2000: // ROM Bank Switching
            if(val == 0x00) {val = 0x01; }
            m_RomBankNumber = (val & 0x7F);
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
