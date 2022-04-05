#include "mbc.hpp"

void MBC::load(const char* path)
{
    std::ifstream in(path, std::ios::binary);
    m_Data.assign((std::istreambuf_iterator<char>(in)), {});
}

auto MBC::read(u16 address) const -> u8
{
    return m_Data.at(address);
}

void MBC::write([[maybe_unused]] u16 address, [[maybe_unused]] u8 val)
{
    // nop
}