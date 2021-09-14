#pragma once

#include "core.h"

#include "instruction.h"
#include "flags.h"

class Gameboy;

class CPU
{
    public:
        CPU(Gameboy* gb);
        void tick();

    private:
        struct Registers
        {
            union
            {
                struct
                {
                    #ifdef IS_BIG_ENDIAN
                        u8 A;
                        u8 F;
                    #else
                        u8 F;
                        u8 A;
                    #endif
                };
                u16 AF;
            };

            union
            {
                struct
                {
                    #ifdef IS_BIG_ENDIAN
                        u8 B;
                        u8 C;
                    #else
                        u8 C;
                        u8 B;
                    #endif
                };
                u16 BC;
            };

            union
            {
                struct
                {
                    #ifdef IS_BIG_ENDIAN
                        u8 D;
                        u8 E;
                    #else
                        u8 E;
                        u8 D;
                    #endif
                };
                u16 DE;
            };

            union
            {
                struct
                {
                    #ifdef IS_BIG_ENDIAN
                        u8 H;
                        u8 L;
                    #else
                        u8 L;
                        u8 H;
                    #endif
                };
                u16 HL;
            };

            u16 SP;
            u16 PC;
        };

        void reset();
        void handleInterrupts();

        void setFlag(const Flags::Register& flag);
        void clearFlag(const Flags::Register& flag);
        void flipFlag(const Flags::Register& flag);
        bool isFlagSet(const Flags::Register& flag);

        void clearAllFlags();
        void setZeroFromVal(const u8& val);

        Gameboy* m_Gameboy;
        Registers m_Registers;
        bool m_IME;

        friend Instruction;
};