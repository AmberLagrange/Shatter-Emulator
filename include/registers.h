#pragma once

#include "core.h"

/**
 * Flag Register
 * 7 6 5 4 3 2 1 0
 * Z N H C 0 0 0 0
 * 
 * Z : Zero Flag
 * N : Subtract Flag
 * H : Half Carry Flag
 * C : Carry Flag
**/

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