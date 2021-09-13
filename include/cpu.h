#pragma once

#include "core.h"

#include "instruction.h"

/**
 * Flag Register
 * 7 6 5 4 3 2 1 0
 * Z N H C x x x x
 * 
 * Z : Zero Flag
 * N : Subtract Flag
 * H : Half Carry Flag
 * C : Carry Flag
**/

class Gameboy;

class CPU
{
    public:
        CPU(Gameboy* gb);
        void tick();

    enum Flags
    {
        Zero = 1 << 7,
        Negative = 1 << 6,
        HalfCarry = 1 << 5,
        Carry = 1 << 4,
        None = 0
    };

    private:
        struct Registers
        {
            union
            {
                struct
                {
                    u8 A;
                    u8 F;
                };
                u16 AF;
            };

            union
            {
                struct
                {
                    u8 B;
                    u8 C;
                };
                u16 BC;
            };

            union
            {
                struct
                {
                    u8 D;
                    u8 E;
                };
                u16 DE;
            };

            union
            {
                struct
                {
                    u8 H;
                    u8 L;
                };
                u16 HL;
            };

            u16 SP;
            u16 PC;
        };

        void reset();
        void handleInterrupts();

        void clearFlags();
        void setFlags(const Flags& flag);
        void toggleFlag(const Flags& flag);
        void untoggleFlag(const Flags& flag);
        void flipFlag(const Flags& flag);
        bool isFlagSet(const Flags& flag);
        void toggleZeroFromVal(const u8& val);

        Gameboy* m_Gameboy;
        Registers m_Registers;
        bool m_InterruptEnabled;

        friend Instruction;
};