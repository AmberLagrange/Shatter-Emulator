#pragma once

#include "core.hpp"

#include <functional>

#ifndef NDEBUG
    #include <string>
    #define INSTRUCTION(mnemonic, op, length, cyclesBranch, cyclesNoBranch) { mnemonic, op, length, cyclesBranch, cyclesNoBranch } //NOLINT(cppcoreguidelines-macro-usage)
#else
    #define INSTRUCTION(mnemonic, op, length, cyclesBranch, cyclesNoBranch) { op, length, cyclesBranch, cyclesNoBranch } //NOLINT(cppcoreguidelines-macro-usage)
#endif

struct Instruction
{
    #ifndef NDEBUG
        std::string mnemonic;
    #endif

    std::function<void(void)> op;
    u8 length;
    u8 cyclesBranch;
    u8 cyclesNoBranch;
};