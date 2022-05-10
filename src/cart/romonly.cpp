#include "core.hpp"

#include "romonly.hpp"

RomOnly::RomOnly()  = default;
RomOnly::~RomOnly() = default;

[[nodiscard]] auto RomOnly::read(u16 address) const -> u8
{
    return m_Rom[address];
}

void RomOnly::write([[maybe_unused]] u16 address, [[maybe_unused]] u8 val)
{
    // nop
}
