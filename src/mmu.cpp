#include "mmu.h"
#include "cpu.h"

MMU::MMU()
{
    LOG("Initializing MMU!");
}

void MMU::load(const char* path)
{
    m_Rom.load(path);
}

u8 MMU::read(const u16& address)
{
    if(address < 0x8000) // ROM
    {
        return m_Rom.read(address);
    }

    if(address < 0xA000) //VRAM
    {
        return m_Memory[address - 0x8000];
    }

    if(address < 0xC000) // RAM Bank
    {
        return m_Memory[address - 0x8000];
    }

    if(address < 0xE000) // Internal RAM
    {
        return m_Memory[address - 0x8000];
    }

    if(address < 0xFE00) // Echo of Internal RAM
    {
        return m_Memory[address - 0x8000 - 0x2000]; // Map back into RAM
    }

    if(address < 0xFEA0) // OAM
    {
        return m_Memory[address - 0x8000];
    }

    if(address < 0xFF00) // Unusable
    {
        WARN("Reading from unmapped memory address 0x" << std::setw(4) << std::setfill('0') << std::hex << address << ".");
        return 0xFF;
    }

    if(address < 0xFF4C) // Input
    {
        return 0xAB;
    }

    if(address < 0xFF80) // Unusable
    {
        WARN("Reading from unmapped memory address 0x" << std::setw(4) << std::setfill('0') << std::hex << address << ".");
        return 0xFF;
    }

    if(address < 0xFFFF) // High RAM
    {
        return m_Memory[address - 0x8000];
    }

    if(address == 0xFFFF) // IME
    {
        return m_CPU->getIME();
    }

    WARN("Reading from unmapped memory address 0x" << std::setw(4) << std::setfill('0') << std::hex << address << ".");
    return 0xFF;
}

u8 MMU::write(const u16& address, const u8& val)
{
    if(address < 0x8000) // ROM
    {
        WARN("Writing 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(val) << " to rom address 0x"
                          << std::setw(4) << std::setfill('0') << std::hex << address << ".");
        return 0xFF;
    }

    if(address < 0xA000) //VRAM
    {
        return m_Memory[address - 0x8000] = val;
    }

    if(address < 0xC000) // RAM Bank
    {
        return m_Memory[address - 0x8000] = val;
    }

    if(address < 0xE000) // Internal RAM
    {
        return m_Memory[address - 0x8000] = val;
    }

    if(address < 0xFE00) // Echo of Internal RAM
    {
        return m_Memory[address - 0x8000 - 0x2000] = val; // Map back into RAM
    }

    if(address < 0xFEA0) // OAM
    {
        return m_Memory[address - 0x8000] = val;
    }

    if(address < 0xFF00) // Unusable
    {
        WARN("Writing 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(val) << " to unmapped memory address 0x"
                          << std::setw(4) << std::setfill('0') << std::hex << address << ".");
        return 0xFF;
    }

    if(address < 0xFF4C) // Input
    {
        return 0xAB;
    }

    if(address < 0xFF80) // Unusable
    {
        WARN("Writing 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(val) << " to unmapped memory address 0x"
                          << std::setw(4) << std::setfill('0') << std::hex << address << ".");
        return 0xFF;
    }

    if(address < 0xFFFF) // High RAM
    {
        return m_Memory[address - 0x8000] = val;
    }

    if(address == 0xFFFF) // IME
    {
        return m_CPU->setIME(val);
    }

    WARN("Writing 0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<u16>(val) << " to unmapped memory address 0x"
                      << std::setw(4) << std::setfill('0') << std::hex << address << ".");
    return 0xFF;
}