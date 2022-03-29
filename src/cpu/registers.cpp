#include "registers.hpp"

auto Registers::A() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->af).at(0);
        }
        else
        {
            u16 af = std::get<u16>(this->af);
            u8   a = static_cast<u8>(af >> CHAR_BIT);
            u8   f = static_cast<u8>(af & UINT8_MAX);
            this->af = std::array<u8, 2> {a, f};
            return std::get<std::array<u8, 2>>(this->af).at(0);
        }
    }, this->af);
}

auto Registers::F() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->af).at(1);
        }
        else
        {
            u16 af = std::get<u16>(this->af);
            u8   a = static_cast<u8>(af >> CHAR_BIT);
            u8   f = static_cast<u8>(af & UINT8_MAX);
            this->af = std::array<u8, 2> {a, f};
            return std::get<std::array<u8, 2>>(this->af).at(1);
        }
    }, this->af);
}

auto Registers::AF() -> u16&
{
    return std::visit([this] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& af = std::get<std::array<u8, 2>>(this->af);
            u16 a = static_cast<u16>(af.at(0) << CHAR_BIT);
            u16 f = static_cast<u16>(af.at(1)            );
            this->af = static_cast<u16>(a | f);
            return std::get<u16>(this->af);
        }
        else
        {
            return std::get<u16>(this->af);
        }
    }, this->af);
}

auto Registers::A() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->af).at(0);
        }
        else
        {
            u16 af = std::get<u16>(this->af);
            u8   a = static_cast<u8>(af >> CHAR_BIT);
            u8   f = static_cast<u8>(af & UINT8_MAX);
            this->af = std::array<u8, 2> {a, f};
            return std::get<std::array<u8, 2>>(this->af).at(0);
        }
    }, this->af);
}

auto Registers::F() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->af).at(1);
        }
        else
        {
            u16 af = std::get<u16>(this->af);
            u8   a = static_cast<u8>(af >> CHAR_BIT);
            u8   f = static_cast<u8>(af & UINT8_MAX);
            this->af = std::array<u8, 2> {a, f};
            return std::get<std::array<u8, 2>>(this->af).at(1);
        }
    }, this->af);
}

auto Registers::AF() const -> u16
{
    return std::visit([this] (auto&& arg) -> u16
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& af = std::get<std::array<u8, 2>>(this->af);
            u16 a = static_cast<u16>(af.at(0) << CHAR_BIT);
            u16 f = static_cast<u16>(af.at(1)            );
            this->af = static_cast<u16>(a | f);
            return std::get<u16>(this->af);
        }
        else
        {
            return std::get<u16>(this->af);
        }
    }, this->af);
}

auto Registers::B() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->bc).at(0);
        }
        else
        {
            u16 bc = std::get<u16>(this->bc);
            u8   b = static_cast<u8>(bc >> CHAR_BIT);
            u8   c = static_cast<u8>(bc & UINT8_MAX);
            this->bc = std::array<u8, 2> {b, c};
            return std::get<std::array<u8, 2>>(this->bc).at(0);
        }
    }, this->bc);
}

auto Registers::C() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->bc).at(1);
        }
        else
        {
            u16 bc = std::get<u16>(this->bc);
            u8   b = static_cast<u8>(bc >> CHAR_BIT);
            u8   c = static_cast<u8>(bc & UINT8_MAX);
            this->bc = std::array<u8, 2> {b, c};
            return std::get<std::array<u8, 2>>(this->bc).at(1);
        }
    }, this->bc);
}

auto Registers::BC() -> u16&
{
    return std::visit([this] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& bc = std::get<std::array<u8, 2>>(this->bc);
            u16 b = static_cast<u16>(bc.at(0) << CHAR_BIT);
            u16 c = static_cast<u16>(bc.at(1)            );
            this->bc = static_cast<u16>(b | c);
            return std::get<u16>(this->bc);
        }
        else
        {
            return std::get<u16>(this->bc);
        }
    }, this->bc);
}

auto Registers::B() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->bc).at(0);
        }
        else
        {
            u16 bc = std::get<u16>(this->bc);
            u8   b = static_cast<u8>(bc >> CHAR_BIT);
            u8   c = static_cast<u8>(bc & UINT8_MAX);
            this->bc = std::array<u8, 2> {b, c};
            return std::get<std::array<u8, 2>>(this->bc).at(0);
        }
    }, this->bc);
}

auto Registers::C() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->bc).at(1);
        }
        else
        {
            u16 bc = std::get<u16>(this->bc);
            u8   b = static_cast<u8>(bc >> CHAR_BIT);
            u8   c = static_cast<u8>(bc & UINT8_MAX);
            this->bc = std::array<u8, 2> {b, c};
            return std::get<std::array<u8, 2>>(this->bc).at(1);
        }
    }, this->bc);
}

auto Registers::BC() const -> u16
{
    return std::visit([this] (auto&& arg) -> u16
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& bc = std::get<std::array<u8, 2>>(this->bc);
            u16 b = static_cast<u16>(bc.at(0) << CHAR_BIT);
            u16 c = static_cast<u16>(bc.at(1)            );
            this->bc = static_cast<u16>(b | c);
            return std::get<u16>(this->bc);
        }
        else
        {
            return std::get<u16>(this->bc);
        }
    }, this->bc);
}

auto Registers::D() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->de).at(0);
        }
        else
        {
            u16 de = std::get<u16>(this->de);
            u8   d = static_cast<u8>(de >> CHAR_BIT);
            u8   e = static_cast<u8>(de & UINT8_MAX);
            this->de = std::array<u8, 2> {d, e};
            return std::get<std::array<u8, 2>>(this->de).at(0);
        }
    }, this->de);
}

auto Registers::E() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->de).at(1);
        }
        else
        {
            u16 de = std::get<u16>(this->de);
            u8   d = static_cast<u8>(de >> CHAR_BIT);
            u8   e = static_cast<u8>(de & UINT8_MAX);
            this->de = std::array<u8, 2> {d, e};
            return std::get<std::array<u8, 2>>(this->de).at(1);
        }
    }, this->de);
}

auto Registers::DE() -> u16&
{
    return std::visit([this] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& de = std::get<std::array<u8, 2>>(this->de);
            u16 d = static_cast<u16>(de.at(0) << CHAR_BIT);
            u16 e = static_cast<u16>(de.at(1)            );
            this->de = static_cast<u16>(d | e);
            return std::get<u16>(this->de);
        }
        else
        {
            return std::get<u16>(this->de);
        }
    }, this->de);
}

auto Registers::D() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->de).at(0);
        }
        else
        {
            u16 de = std::get<u16>(this->de);
            u8   d = static_cast<u8>(de >> CHAR_BIT);
            u8   e = static_cast<u8>(de & UINT8_MAX);
            this->de = std::array<u8, 2> {d, e};
            return std::get<std::array<u8, 2>>(this->de).at(0);
        }
    }, this->de);
}

auto Registers::E() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->de).at(1);
        }
        else
        {
            u16 de = std::get<u16>(this->de);
            u8   d = static_cast<u8>(de >> CHAR_BIT);
            u8   e = static_cast<u8>(de & UINT8_MAX);
            this->de = std::array<u8, 2> {d, e};
            return std::get<std::array<u8, 2>>(this->de).at(1);
        }
    }, this->de);
}

auto Registers::DE() const -> u16
{
    return std::visit([this] (auto&& arg) -> u16
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& de = std::get<std::array<u8, 2>>(this->de);
            u16 d = static_cast<u16>(de.at(0) << CHAR_BIT);
            u16 e = static_cast<u16>(de.at(1)            );
            this->de = static_cast<u16>(d | e);
            return std::get<u16>(this->de);
        }
        else
        {
            return std::get<u16>(this->de);
        }
    }, this->de);
}

auto Registers::H() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->hl).at(0);
        }
        else
        {
            u16 hl = std::get<u16>(this->hl);
            u8   h = static_cast<u8>(hl >> CHAR_BIT);
            u8   l = static_cast<u8>(hl & UINT8_MAX);
            this->hl = std::array<u8, 2> {h, l};
            return std::get<std::array<u8, 2>>(this->hl).at(0);
        }
    }, this->hl);
}

auto Registers::L() -> u8&
{
    return std::visit([this] (auto&& arg) -> u8&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->hl).at(1);
        }
        else
        {
            u16 hl = std::get<u16>(this->hl);
            u8   h = static_cast<u8>(hl >> CHAR_BIT);
            u8   l = static_cast<u8>(hl & UINT8_MAX);
            this->hl = std::array<u8, 2> {h, l};
            return std::get<std::array<u8, 2>>(this->hl).at(1);
        }
    }, this->hl);
}

auto Registers::HL() -> u16&
{
    return std::visit([this] (auto&& arg) -> u16&
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& hl = std::get<std::array<u8, 2>>(this->hl);
            u16 h = static_cast<u16>(hl.at(0) << CHAR_BIT);
            u16 l = static_cast<u16>(hl.at(1)            );
            this->hl = static_cast<u16>(h | l);
            return std::get<u16>(this->hl);
        }
        else
        {
            return std::get<u16>(this->hl);
        }
    }, this->hl);
}

auto Registers::H() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->hl).at(0);
        }
        else
        {
            u16 hl = std::get<u16>(this->hl);
            u8   h = static_cast<u8>(hl >> CHAR_BIT);
            u8   l = static_cast<u8>(hl & UINT8_MAX);
            this->hl = std::array<u8, 2> {h, l};
            return std::get<std::array<u8, 2>>(this->hl).at(0);
        }
    }, this->hl);
}

auto Registers::L() const -> u8
{
    return std::visit([this] (auto&& arg) -> u8
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            return std::get<std::array<u8, 2>>(this->hl).at(1);
        }
        else
        {
            u16 hl = std::get<u16>(this->hl);
            u8   h = static_cast<u8>(hl >> CHAR_BIT);
            u8   l = static_cast<u8>(hl & UINT8_MAX);
            this->hl = std::array<u8, 2> {h, l};
            return std::get<std::array<u8, 2>>(this->hl).at(1);
        }
    }, this->hl);
}

auto Registers::HL() const -> u16
{
    return std::visit([this] (auto&& arg) -> u16
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::array<u8, 2>>)
        {
            std::array<u8, 2>& hl = std::get<std::array<u8, 2>>(this->hl);
            u16 h = static_cast<u16>(hl.at(0) << CHAR_BIT);
            u16 l = static_cast<u16>(hl.at(1)            );
            this->hl = static_cast<u16>(h | l);
            return std::get<u16>(this->hl);
        }
        else
        {
            return std::get<u16>(this->hl);
        }
    }, this->hl);
}
