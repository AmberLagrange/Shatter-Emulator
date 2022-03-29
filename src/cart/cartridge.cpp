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
    contents.assign((std::istreambuf_iterator<char>(in)), {});

    std::copy_n(contents.begin(), BANK_SIZE, m_Rom0.begin());
    swapBank(1);
}

void Cartridge::swapBank(u8 bankNumber)
{
    //bankNumber %= (contents.size() / 0x8000);
    std::copy_n(contents.begin() + (BANK_SIZE * bankNumber), BANK_SIZE, m_Rom1.begin());
}

auto Cartridge::read(u16 address) const -> u8
{
    return (address < BANK_SIZE) ? m_Rom0.at(address) : m_Rom1.at(address - BANK_SIZE);
}
