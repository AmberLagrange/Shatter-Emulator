#include "mbc.hpp"

auto MBC::getCartType(const char* path) -> Cart::Type
{
    std::ifstream rom(path, std::ios::in | std::ios::binary);
    rom.seekg(CART_TYPE);

    char cartType = -1;
    rom.read(&cartType, 1);

    return static_cast<Cart::Type>(cartType);
}

void MBC::load(const char* path)
{
    std::ifstream rom(path, std::ios::in | std::ios::binary);
    m_Data.assign((std::istreambuf_iterator<char>(rom)), {});
}

auto MBC::read(u16 address) const -> u8
{
    return m_Data.at(address);
}

void MBC::write([[maybe_unused]] u16 address, [[maybe_unused]] u8 val)
{
    // nop
}