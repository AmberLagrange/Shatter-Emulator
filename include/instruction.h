#pragma once

#include "core.h"

#include <functional>

class Gameboy;

struct Instruction
{
    #ifndef NDEBUG
        std::string mnemonic;
    #endif

    std::function<void (Gameboy*)> op;
    u8 length;
    u8 cyclesBranch;
    u8 cyclesNoBranch;

    static const Instruction instructions[0x100];
    static const Instruction instructionsCB[0x100];

    //opcodes

    //0x00

    static void op00(Gameboy* gb);

    //0x10

    //0x20

    //0x30

    //0x40

    //0x50

    //0x60

    //0x70

    //0x80

    //0x90

    //0xA0

    //0xB0

    //0xC0

    static void opC3(Gameboy* gb);

    //0xD0

    //0xE0

    //0xF0
};