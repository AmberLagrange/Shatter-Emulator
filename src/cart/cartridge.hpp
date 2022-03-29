#pragma once

#include "core.hpp"

#include <array>
#include <vector>

class Cartridge
{
    public:
        Cartridge() = default;
        Cartridge(const char* path);

        void load(const char* path);
        void swapBank(u8 bankNumber);

        [[nodiscard]]
        auto read(u16 address) const -> u8;

    private:
        std::vector<u8> contents;

        std::array<u8, BANK_SIZE> m_Rom0 {{}};
        std::array<u8, BANK_SIZE> m_Rom1 {{}};
};