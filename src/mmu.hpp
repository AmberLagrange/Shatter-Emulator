#pragma once

#include "core.hpp"

#include <array>

#include "cart/cartridge.hpp"

class Gameboy;

class MMU
{
    public:
        MMU(Gameboy& gb);

        void load(const char* path);

        [[nodiscard]]
        auto read(u16 address) const -> u8;
        void write(u16 address, u8 val);
    private:
        void reset();

    private:
        Gameboy& m_Gameboy;
        Cartridge m_Rom;

        std::array<u8, RAM_SIZE> m_Memory {{}};
};
