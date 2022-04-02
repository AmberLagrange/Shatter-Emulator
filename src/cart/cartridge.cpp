#include "cartridge.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>

Cartridge::Cartridge(const char* path)
{
    load(path);
}

void Cartridge::load(const char* path)
{
    std::ifstream in(path, std::ios::binary);
    m_Data.assign((std::istreambuf_iterator<char>(in)), {});
    m_BankNumber = 0;
}

void Cartridge::swapBank(u8 bankNumber)
{
    // Subtract 1 from the bankNumber to eumlate 0 indexing, as bank 0 will never swap.
    m_BankNumber = bankNumber - 1;
}

auto Cartridge::read(u16 address) const -> u8
{
    return (address < BANK_SIZE) ? m_Data.at(address) : m_Data.at(address + BANK_SIZE * m_BankNumber);
}
