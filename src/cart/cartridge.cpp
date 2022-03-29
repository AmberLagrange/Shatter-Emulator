#include "cartridge.h"

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iterator>

Cartridge::Cartridge(const char* path)
{
    load(path);
}

void Cartridge::load(const char* path)
{
    std::ifstream in(path, std::ios::binary);
    contents.assign((std::istreambuf_iterator<char>(in)), {});

    std::copy_n(contents.begin(), 0x4000, m_Rom0.begin());
    swapBank(1);
}

void Cartridge::swapBank(u8 bankNumber)
{
    //bankNumber %= (contents.size() / 0x8000);
    std::copy_n(contents.begin() + (0x4000 * bankNumber), 0x4000, m_Rom1.begin());
}

auto Cartridge::read(u16 address) const -> u8
{
    return (address < 0x4000) ? m_Rom0[address] : m_Rom1[address - 0x4000];
}