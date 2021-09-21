#pragma once

#include "core.h"

#include <vector>

class Cartridge
{
    public:
        Cartridge() = default;
        Cartridge(const char* path);

        void load(const char* path);
        void swapBank(u8 bankNumber);

        u8 read(u16 address);

    private:
        std::vector<u8> contents;

        u8 m_Rom0[0x4000];
        u8 m_Rom1[0x4000];
};