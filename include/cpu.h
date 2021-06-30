#pragma once

#include "core.h"

#include "registers.h"
#include "instruction.h"

class Gameboy;

class CPU
{
    public:
        CPU(Gameboy* gb);
        void tick();
    private:
        void reset();

        Gameboy* m_Gameboy;
        Registers m_Registers;

        friend Instruction;
};