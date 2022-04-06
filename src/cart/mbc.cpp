#include "mbc.hpp"

auto MBC::loadRom(const char* path) -> std::vector<u8>
{
    std::vector<u8> data;
    std::ifstream in(path, std::ios::binary);
    data.assign((std::istreambuf_iterator<char>(in)), {});

    return data;
}

void MBC::load(const char* path)
{
    m_Data = loadRom(path);
}

void MBC::load(const Rom& data)
{
    m_Data = data;
}

auto MBC::read(u16 address) const -> u8
{
    return m_Data.at(address);
}

void MBC::write([[maybe_unused]] u16 address, [[maybe_unused]] u8 val)
{
    // nop
}