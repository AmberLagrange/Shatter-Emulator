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

    enum Flag
    {
        Zero        = 0b10000000,
        Negative    = 0b01000000,
        HalfCarry   = 0b00100000,
        Carry       = 0b00010000
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

        void setFlag(const Flag& flag);
        void clearFlag(const Flag& flag);
        void flipFlag(const Flag& flag);
        bool isFlagSet(const Flag& flag);

        void clearAllFlags();
        void setZeroFromVal(const u8& val);

        Gameboy* m_Gameboy;
        Registers m_Registers;
        bool m_InterruptEnabled;

        friend Instruction;
};

inline CPU::Flag operator|(const CPU::Flag& flag1, const CPU::Flag& flag2)
{
    return static_cast<CPU::Flag>(static_cast<int>(flag1) | static_cast<int>(flag2));
}