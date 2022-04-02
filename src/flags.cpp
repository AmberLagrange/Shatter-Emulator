#include "flags.hpp"

//--------------------------------------Register Flags--------------------------------------//

auto operator|(const Flags::Register& flag1, const Flags::Register& flag2) -> Flags::Register            
{
    return static_cast<Flags::Register>(static_cast<u8>(flag1) | static_cast<u8>(flag2));
}

auto operator&(const Flags::Register& flag1, const Flags::Register& flag2) -> Flags::Register            
{
    return static_cast<Flags::Register>(static_cast<u8>(flag1) & static_cast<u8>(flag2));
}

auto operator^(const Flags::Register& flag1, const Flags::Register& flag2) -> Flags::Register            
{
    return static_cast<Flags::Register>(static_cast<u8>(flag1) ^ static_cast<u8>(flag2));
}

auto operator|=(Flags::Register& flag1, const Flags::Register flag2) -> Flags::Register&            
{
    return flag1 = flag1 | flag2;
}

auto operator&=(Flags::Register& flag1, const Flags::Register flag2) -> Flags::Register&            
{
    return flag1 = flag1 & flag2;
}

auto operator^=(Flags::Register& flag1, const Flags::Register flag2) -> Flags::Register&            
{
    return flag1 = flag1 ^ flag2;
}

//--------------------------------------Interrupt Flags--------------------------------------//

auto operator|(const Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt            
{
    return static_cast<Flags::Interrupt>(static_cast<u8>(flag1) | static_cast<u8>(flag2));
}

auto operator&(const Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt            
{
    return static_cast<Flags::Interrupt>(static_cast<u8>(flag1) & static_cast<u8>(flag2));
}

auto operator^(const Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt            
{
    return static_cast<Flags::Interrupt>(static_cast<u8>(flag1) ^ static_cast<u8>(flag2));
}

auto operator|=(Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt&            
{
    return flag1 = flag1 | flag2;
}

auto operator&=(Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt&            
{
    return flag1 = flag1 & flag2;
}

auto operator^=(Flags::Interrupt& flag1, const Flags::Interrupt& flag2) -> Flags::Interrupt&            
{
    return flag1 = flag1 ^ flag2;
}
