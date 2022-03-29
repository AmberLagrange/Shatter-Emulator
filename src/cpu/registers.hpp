#pragma once

#include "core.hpp"

#include <array>
#include <variant>

class Registers
{
    private:
        mutable std::variant<std::array<u8, 2>, u16> af;
        mutable std::variant<std::array<u8, 2>, u16> bc;
        mutable std::variant<std::array<u8, 2>, u16> de;
        mutable std::variant<std::array<u8, 2>, u16> hl;
        u16                                          sp;
        u16                                          pc;

    public:
        [[nodiscard]] auto A()        -> u8&;
        [[nodiscard]] auto F()        -> u8&;
        [[nodiscard]] auto AF()       -> u16&;

        [[nodiscard]] auto A()  const -> u8;
        [[nodiscard]] auto F()  const -> u8;
        [[nodiscard]] auto AF() const -> u16;

        [[nodiscard]] auto B()        -> u8&;
        [[nodiscard]] auto C()        -> u8&;
        [[nodiscard]] auto BC()       -> u16&;

        [[nodiscard]] auto B()  const -> u8;
        [[nodiscard]] auto C()  const -> u8;
        [[nodiscard]] auto BC() const -> u16;

        [[nodiscard]] auto D()        -> u8&;
        [[nodiscard]] auto E()        -> u8&;
        [[nodiscard]] auto DE()       -> u16&;

        [[nodiscard]] auto D()  const -> u8;
        [[nodiscard]] auto E()  const -> u8;
        [[nodiscard]] auto DE() const -> u16;

        [[nodiscard]] auto H()        -> u8&;
        [[nodiscard]] auto L()        -> u8&;
        [[nodiscard]] auto HL()       -> u16&;

        [[nodiscard]] auto H()  const -> u8;
        [[nodiscard]] auto L()  const -> u8;
        [[nodiscard]] auto HL() const -> u16;

        [[nodiscard]] inline auto SP() -> u16& { return sp; }
        [[nodiscard]] inline auto PC() -> u16& { return pc; }
};
