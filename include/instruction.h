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

//--------------------------------------Opcode Helpers--------------------------------------//

    static void opcodeInc(Gameboy* gb, u8& reg);
    static void opcodeDec(Gameboy* gb, u8& reg);
    
    static void opcodeLoadu8(Gameboy* gb, u8& reg);
    static void opcodeLoadA(Gameboy* gb, const u8& val);

    static void opcodeXOR(Gameboy* gb, const u8& val);

    static void opcodeJP(Gameboy* gb, bool condition);
    static void opcodeJPOffset(Gameboy* gb, bool condition);

//--------------------------------------Opcodes--------------------------------------//

    //0x00

    static void op00(Gameboy* gb);
    static void op05(Gameboy* gb);
    static void op06(Gameboy* gb);
    static void op0d(Gameboy* gb);
    static void op0E(Gameboy* gb);

    //0x10

    //0x20

    static void op20(Gameboy* gb);
    static void op21(Gameboy* gb);

    //0x30

    static void op32(Gameboy* gb);
    static void op3E(Gameboy* gb);

    //0x40

    //0x50

    //0x60

    //0x70

    //0x80

    //0x90

    //0xA0

    static void opAF(Gameboy* gb);

    //0xB0

    //0xC0

    static void opC3(Gameboy* gb);

    //0xD0

    //0xE0

    static void opE0(Gameboy* gb);

    //0xF0

    static void opF0(Gameboy* gb);
    static void opF3(Gameboy* gb);
    static void opFB(Gameboy* gb);
};