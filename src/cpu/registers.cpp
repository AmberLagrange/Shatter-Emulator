#include "core.hpp"

#include "registers.hpp"

Registers::Registers()
    #ifndef UNSAFE
        : af({}), bc({}), de({}), hl({}), sp(0), pc(0)
    #else
        : af(0), bc(0), de(0), hl(0), sp(0), pc(0)
    #endif
{
    #ifndef UNSAFE
        
    #else
        a = 0;
        f = 0;
        b = 0;
        c = 0;
        d = 0;
        e = 0;
    #endif
}

Registers::~Registers() = default;

//-------------------------Helper Functions-------------------------//

#ifndef UNSAFE

auto Registers::getHigh(Register& reg) -> u8&
{
    return std::visit([&reg] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(reg).at(0);
        }
        else
        {
            u16 full = std::get<u16>(reg);
            u8  high = static_cast<u8>(full >> CHAR_BIT);
            u8   low = static_cast<u8>(full & UINT8_MAX);
            reg = std::array<u8, 2> {high, low};
            return std::get<std::array<u8, 2>>(reg).at(0);
        }
    }, reg);
}

auto Registers::getLow(Register& reg) -> u8&
{
    return std::visit([&reg] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(reg).at(1);
        }
        else
        {
            u16 full = std::get<u16>(reg);
            u8  high = static_cast<u8>(full >> CHAR_BIT);
            u8   low = static_cast<u8>(full & UINT8_MAX);
            reg = std::array<u8, 2> {high, low};
            return std::get<std::array<u8, 2>>(reg).at(1);
        }
    }, reg);
}

auto Registers::getFull(Register& reg) -> u16&
{
    return std::visit([&reg] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& full = std::get<std::array<u8, 2>>(reg);
            u16 high = static_cast<u16>(full.at(0) << CHAR_BIT);
            u16  low = static_cast<u16>(full.at(1)            );
            reg = static_cast<u16>(high | low);
            return std::get<u16>(reg);
        }
        else
        {
            return std::get<u16>(reg);
        }
    }, reg);
}

//-------------------------Non-Const Versions-------------------------//

auto Registers::A() -> u8&
{
    return getHigh(af);
}

auto Registers::F() -> u8&
{
    return getLow(af);
}

auto Registers::AF() -> u16&
{
    return getFull(af);
}

auto Registers::B() -> u8&
{
    return getHigh(bc);
}

auto Registers::C() -> u8&
{
    return getLow(bc);
}

auto Registers::BC() -> u16&
{
    return getFull(bc);
}

auto Registers::D() -> u8&
{
    return getHigh(de);
}

auto Registers::E() -> u8&
{
    return getLow(de);
}

auto Registers::DE() -> u16&
{
    return getFull(de);
}

auto Registers::H() -> u8&
{
    return getHigh(hl);
}

auto Registers::L() -> u8&
{
    return getLow(hl);
}

auto Registers::HL() -> u16&
{
    return getFull(hl);
}

auto Registers::SP() -> u16&
{
    return sp;
}

auto Registers::PC() -> u16&
{
    return pc;
}

#else

auto Registers::A() -> u8&
{
    return a;
}

auto Registers::F() -> u8&
{
    return f;
}

auto Registers::AF() -> u16&
{
    return af;
}

auto Registers::B() -> u8&
{
    return b;
}

auto Registers::C() -> u8&
{
    return c;
}

auto Registers::BC() -> u16&
{
    return bc;
}

auto Registers::D() -> u8&
{
    return d;
}

auto Registers::E() -> u8&
{
    return e;
}

auto Registers::DE() -> u16&
{
    return de;
}

auto Registers::H() -> u8&
{
    return h;
}

auto Registers::L() -> u8&
{
    return l;
}

auto Registers::HL() -> u16&
{
    return hl;
}

auto Registers::SP() -> u16&
{
    return sp;
}

auto Registers::PC() -> u16&
{
    return pc;
}

#endif

//-------------------------Const Versions-------------------------//

auto Registers::A() const -> u8
{
    return const_cast<Registers*>(this)->A();
}

auto Registers::F() const -> u8
{
    return const_cast<Registers*>(this)->F();
}

auto Registers::AF() const -> u16
{
    return const_cast<Registers*>(this)->AF();
}

auto Registers::B() const -> u8
{
    return const_cast<Registers*>(this)->B();
}

auto Registers::C() const -> u8
{
    return const_cast<Registers*>(this)->C();
}

auto Registers::BC() const -> u16
{
    return const_cast<Registers*>(this)->BC();
}

auto Registers::D() const -> u8
{
    return const_cast<Registers*>(this)->D();
}

auto Registers::E() const -> u8
{
    return const_cast<Registers*>(this)->E();
}

auto Registers::DE() const -> u16
{
    return const_cast<Registers*>(this)->DE();
}

auto Registers::H() const -> u8
{
    return const_cast<Registers*>(this)->H();
}

auto Registers::L() const -> u8
{
    return const_cast<Registers*>(this)->L();
}

auto Registers::HL() const -> u16
{
    return const_cast<Registers*>(this)->HL();
}

auto Registers::SP() const -> u16
{
    return const_cast<Registers*>(this)->SP();
}

auto Registers::PC() const -> u16
{
    return const_cast<Registers*>(this)->PC();
}
