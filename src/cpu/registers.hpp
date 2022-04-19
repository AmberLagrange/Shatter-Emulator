#pragma once

#include "core.hpp"

#include <array>
#include <variant>

#define UNSAFE

class Registers
{
    public:

        //-------------------------Non-Const Versions-------------------------//

        /**
         * @brief Gets the A register by reference
         * 
         * @return The register by reference
         */
        [[nodiscard]] auto A() -> u8&;

        /**
         * @brief Gets the F register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto F() -> u8&;

        /**
         * @brief Gets the AF register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto AF() -> u16&;

        /**
         * @brief Gets the B register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto B() -> u8&;

        /**
         * @brief Gets the C register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto C() -> u8&;

        /**
         * @brief Gets the BC register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto BC() -> u16&;

        /**
         * @brief Gets the D register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto D() -> u8&;

        /**
         * @brief Gets the E register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto E() -> u8&;

        /**
         * @brief Gets the DE register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto DE() -> u16&;

        /**
         * @brief Gets the H register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto H() -> u8&;

        /**
         * @brief Gets the L register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto L() -> u8&;

        /**
         * @brief Gets the HL register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto HL() -> u16&;

        /**
         * @brief Gets the SP register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto SP() -> u16&;

        /**
         * @brief Gets the PC register by reference
         * 
         * @return The register by reference 
         */
        [[nodiscard]] auto PC() -> u16&;

        //-------------------------Const Versions-------------------------//

        /**
         * @brief Gets the A register by value
         * 
         * @return The value in the register
         */
        [[nodiscard]] auto A() const -> u8;

        /**
         * @brief Gets the F register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto F() const -> u8;

        /**
         * @brief Gets the AF register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto AF() const -> u16;

        /**
         * @brief Gets the B register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto B() const -> u8;

        /**
         * @brief Gets the C register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto C() const -> u8;

        /**
         * @brief Gets the BC register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto BC() const -> u16;

        /**
         * @brief Gets the D register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto D() const -> u8;

        /**
         * @brief Gets the E register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto E() const -> u8;

        /**
         * @brief Gets the DE register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto DE() const -> u16;

        /**
         * @brief Gets the H register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto H() const -> u8;

        /**
         * @brief Gets the L register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto L() const -> u8;

        /**
         * @brief Gets the HL register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto HL() const -> u16;

        /**
         * @brief Gets the SP register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto SP() const -> u16;

        /**
         * @brief Gets the PC register by value
         * 
         * @return The value in the register 
         */
        [[nodiscard]] auto PC() const -> u16;

        #ifndef UNSAFE
        private:
            using Register = std::variant<std::array<u8, 2>, u16>;

            mutable Register af;
            mutable Register bc;
            mutable Register de;
            mutable Register hl;
            u16              sp;
            u16              pc;

        private:
            /**
             * @brief Get the high byte of the register by reference
             * 
             * @param reg The register to get the high byte from
             * @return The updated register
             */
            [[nodiscard]] auto getHigh(Register& reg) -> u8&;

            /**
             * @brief Get the low byte of the register by reference
             * 
             * @param reg The register to get the low byte from
             * @return The updated register
             */
            [[nodiscard]] auto getLow(Register& reg) -> u8&;

            /**
             * @brief Get both bytes from the register by reference
             * 
             * @param reg The register to get both bytes from
             * @return The updated register
             */
            [[nodiscard]] auto getFull(Register& reg) -> u16&;

        #else
        private:
            union
            {
                struct
                {
                    u8 f;
                    u8 a;
                };
                u16 af;
            };

            union
            {
                struct
                {
                    u8 c;
                    u8 b;
                };
                u16 bc;
            };

            union
            {
                struct
                {
                    u8 e;
                    u8 d;
                };
                u16 de;
            };

            union
            {
                struct
                {
                    u8 l;
                    u8 h;
                };
                u16 hl;
            };

            u16 sp;
            u16 pc;

        #endif
};
