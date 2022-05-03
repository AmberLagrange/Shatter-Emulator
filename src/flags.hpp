#pragma once

#include "core.hpp"

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

namespace Flags
{
    enum Register : u8
    {
        Zero        = 0b10000000,
        Negative    = 0b01000000,
        HalfCarry   = 0b00100000,
        Carry       = 0b00010000,
        FlagNone    = 0b00000000
    };

    enum Interrupt : u8
    {
        VBlank      = 0b00000001,
        LCD_STAT    = 0b00000010,
        Timer       = 0b00000100,
        Serial      = 0b00001000,
        Joypad      = 0b00010000,
        IntNone     = 0b00000000
    };
}

//--------------------------------------Register Flags--------------------------------------//


/**
 * @brief ORs two Register Flags together
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The or'd result
 */
auto operator|(const Flags::Register& flag1, const Flags::Register& flag2) -> Flags::Register;

/**
 * @brief ANDs two Register Flags together
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The and'd result
 */
auto operator&(const Flags::Register& flag1, const Flags::Register& flag2) -> Flags::Register;

/**
 * @brief XORs two Register Flags together
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The xor'd result
 */
auto operator^(const Flags::Register& flag1, const Flags::Register& flag2) -> Flags::Register;

/**
 * @brief ORs two Register Flags together and stores the result in the first flag
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The updated flag
 */
auto operator|=(Flags::Register& flag1, const Flags::Register flag2) -> Flags::Register&;

/**
 * @brief ANDs two Register Flags together and stores the result in the first flag
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The updated flag
 */
auto operator&=(Flags::Register& flag1, const Flags::Register flag2) -> Flags::Register&;

/**
 * @brief XORs two Register Flags together and stores the result in the first flag
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The updated flag
 */
auto operator^=(Flags::Register& flag1, const Flags::Register flag2) -> Flags::Register&;

//--------------------------------------Interrupt Flags--------------------------------------//

/**
 * @brief ORs two Interrupt Flags together
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The or'd result
 */
auto operator|(const Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt;

/**
 * @brief ANDs two Interrupt Flags together
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The and'd result
 */
auto operator&(const Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt;

/**
 * @brief XORs two Interrupt Flags together
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The xor'd result
 */
auto operator^(const Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt;

/**
 * @brief ORs two Interrupt Flags together and stores the result in the first flag
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The updated flag
 */
auto operator|=(Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt&;

/**
 * @brief ANDs two Interrupt Flags together and stores the result in the first flag
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The updated flag
 */
auto operator&=(Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt&;

/**
 * @brief XORs two Interrupt Flags together and stores the result in the first flag
 * 
 * @param flag1 The first flag
 * @param flag2 The second flag
 * @return The updated flag
 */
auto operator^=(Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt&;
