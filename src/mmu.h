#pragma once

#include "core.h"

#include <array>

#include "cart/cartridge.h"

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

        std::array<u8, 0x8000> m_Memory {{}};
};