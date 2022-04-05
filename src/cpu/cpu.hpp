#pragma once

#include "core.hpp"

#include <array>

#include "instruction.hpp"
#include "registers.hpp"

#include "flags.hpp"

class Gameboy;

class CPU
{
    public:
        CPU(Gameboy& gb);

        /**
         * @brief Interrupt Master Enable (IME) Getter
         * 
         * @return m_IME
         */
        [[nodiscard]] auto getIME() const   -> bool;

        /**
         * @brief Interrupt Master Enable (IME) Setter
         * 
         * @param ime The value to update the IME to
         */
        void setIME(bool ime);

        /**
         * @brief Emulates a single instruction being executed
         * 
         * @return The number of cycles the instruction took
         */
        auto tick() -> u8;

        /**
         * @brief Raise an interrupt with a given flag
         * 
         * @param flag The interrupt flag to raise
         */
        void raiseInterrupt(const Flags::Interrupt& flag);

        /**
         * @brief Handle any interrupts that may have been risen
         * 
         * @param cycles The  number of cycles the instruction took to execute
         */
        void handleInterrupts(u8& cycles);

    private:
        /**
         * @brief Reset the CPU to its startup state
         * 
         */
        void reset();

        /**
         * @brief Check if a given register flag is set
         * 
         * @param flag The register flag to check
         * @return the state of the flag
         */
        [[nodiscard]] auto isFlagSet(const Flags::Register& flag) const -> bool;

        /**
         * @brief Set a given flag
         * 
         * @param flag The register flag to set
         */
        void setFlag(const Flags::Register& flag);

        /**
         * @brief Clear a given flag
         * 
         * @param flag The register flag to clear
         */
        void clearFlag(const Flags::Register& flag);

        /**
         * @brief Flip a given flag
         * 
         * @param flag The given flag to flip
         */
        void  flipFlag(const Flags::Register& flag);

        /**
         * @brief Clear all register flags
         * 
         */
        void clearAllFlags();

        /**
         * @brief Set the Zero flag if val is zero
         * 
         * @param val The value to check if zero
         */
        void setZeroFromVal(u8 val);

    private:
        Registers m_Registers;

        Gameboy& m_Gameboy;

        bool m_Halted;
        bool m_IME;
        bool m_Branched;

    private:
        //--------------------------------------Opcode Helpers--------------------------------------//

        /**
         * @brief Push a value to the stack
         * 
         * @param val The value to push to the stack
         */
        void pushStack(u16 val);

        /**
         * @brief Pop a value from the stack
         * 
         * @param reg The value popped from the stack
         */
        void popStack(u16& reg);

        /**
         * @brief INC opcode helper function. Increments a given register
         * 
         * @param reg The register to increment
         */
        void opcodeINC(u8& reg);

        /**
         * @brief DEC opcode helper function. Decrements a given register
         * 
         * @param reg The register to decrement
         */
        void opcodeDEC(u8& reg);
    
        /**
         * @brief ADD opcode helper function. Adds to the A register
         * 
         * @param val The value to add to A
         */
        void opcodeADD(u8 val);

        /**
         * @brief ADC opcode helper function. Adds with carry to the A register
         * 
         * @param val The value to add with carry to A
         */
        void opcodeADC(u8 val);

        /**
         * @brief SUB opcode helper function. Subtracts from the A register
         * 
         * @param val The value to subtract from A
         */
        void opcodeSUB(u8 val);

        /**
         * @brief SBC opcode helper function. Subtracts with carry from the A register
         * 
         * @param val The value to subtract with carry from A
         */
        void opcodeSBC(u8 val);

        /**
         * @brief AND opcode helper function. Ands with the A register
         * 
         * @param val The value to and with A
         */
        void opcodeAND(u8 val);

        /**
         * @brief XOR opcode helper function. Xors with the A register
         * 
         * @param val The value to xor with A
         */
        void opcodeXOR(u8 val);

        /**
         * @brief OR opcode helper function. Ors with the A register
         * 
         * @param val The value to or with A
         */
        void opcodeOR(u8 val);

        /**
         * @brief CP opcode helper function. Compares with the A register
         * (subtracts to set flags but doesn't update A)
         * 
         * @param val The value to compare with A
         */
        void opcodeCP(u8 val);

        /**
         * @brief JP opcode helper function. Jumps to a memory address if a given
         * condition is true
         * 
         * @param condition The condition to check
         */
        void opcodeJP(bool condition);

        /**
         * @brief JR opcode helper function. Jumps relatively to a memory address
         * if a given condition is true
         * 
         * @param condition The condition to check
         */
        void opcodeJR(bool condition);

        /**
         * @brief CALL opcode helper function. Calls a subroutine at a memory address
         * if a given condition is true
         * 
         * @param condition The condition to check
         */
        void opcodeCALL(bool condition);

        /**
         * @brief RET opcode helper function. Returns from a subroutine if a given
         * condition is true
         * 
         * @param condiiton The condition to check
         */
        void opcodeRET(bool condiiton);

        /**
         * @brief RST helper function. Jumps to a specified reset vector
         * 
         * @param val The reset vector to jump to
         */
        void opcodeRST(u16 val);

        //-------------------------16 bit variants-------------------------//

        /**
         * @brief ADD opcode helper function. Adds to the HL register
         * 
         * @param val The value to add to HL
         */
        void opcodeADD_HL(u16 val);

        /**
         * @brief ADD opcode helper function. Adds the next byte in memory
         * to the the value of the SP register. Does not modify SP
         * 
         * @return u16 The SP register with the byte added to it
         */
        auto opcodeADD_SP() -> u16;

        //--------------------------------------CB Opcode Helpers--------------------------------------//

        /**
         * @brief RLC opcode helper function. Rotates left circularly
         * the given register
         * 
         * @param reg The register to rotate left circularly
         */
        void opcodeRLC(u8& reg);

        /**
         * @brief RRC opcode helper function. Rotates right circularly
         * the given register
         * 
         * @param reg The register to rotate right circularly
         */
        void opcodeRRC(u8& reg);

        /**
         * @brief RL opcode helper function. Rotates left the given register
         * 
         * @param reg The register to rotate left
         */
        void opcodeRL(u8& reg);

        /**
         * @brief RR opcode helper function. Rotates right the given register
         * 
         * @param reg The register to rotate right
         */
        void opcodeRR(u8& reg);

        /**
         * @brief SLA opcode helper function. Shifts the given register to
         * the left (with bit 7 shifted into the carry) and zeroes bit 0
         * 
         * @param reg The register to shift left
         */
        void opcodeSLA(u8& reg);

        /**
         * @brief SRA opcode helper function. Shifts the given register to
         * the right (with bit 0 shifted into the carry) and retains bit 7's
         * original value
         * 
         * @param reg 
         */
        void opcodeSRA(u8& reg);

        /**
         * @brief SWAP opcode helper function. Swaps the high nibble with
         * the low nibble of the given register
         * 
         * @param reg The register to swap
         */
        void opcodeSWAP(u8& reg);

        /**
         * @brief SRL opcode helper function. Shifts the given register to
         * the right (with bit 0 shifted into the carry) and zeroes bit 7
         * 
         * @param reg 
         */
        void opcodeSRL(u8& reg);

        /**
         * @brief BIT opcode helper function. Checks if the requested
         * bit is zero in a given register, then updates the zero flag
         * 
         * @param bit The bit to check
         * @param reg The register to check
         */
        void opcodeBIT(u8 bit, u8& reg);

        /**
         * @brief RES opcode helper function. Sets the requested bit
         * in the given register to 0
         * 
         * @param bit The bit to set
         * @param reg The register to update
         */
        void opcodeRES(u8 bit, u8& reg);

        /**
         * @brief SET opcode helper function. Sets the requested bit
         * in a given register to 1
         * 
         * @param bit The bit to set
         * @param reg The register to update
         */
        void opcodeSET(u8 bit, u8& reg);

        // 16 bit variants

        /**
         * @brief BIT opcode helper function. Checks if the requested
         * bit is zero in the memory location specified by HL is zero,
         * then updates the zero flag
         * 
         * @param bit The bit to check
         */
        void opcodeBIT_HL(u8 bit);

        /**
         * @brief RES opcode helper function. Sets the requested bit
         * at the memory location specified by HL to 0
         * 
         * @param bit The bit to set
         */
        void opcodeRES_HL(u8 bit);

        /**
         * @brief SET opcode helper function. Sets the requested bit
         * at the memory location specified by HL to 1
         * 
         * @param bit The bit to set
         */
        void opcodeSET_HL(u8 bit);

        //--------------------------------------Opcodes--------------------------------------//

        //0x00

        void opcode0x00(); // NOP
        void opcode0x01(); // LD BC,u16
        void opcode0x02(); // LD (BC),A
        void opcode0x03(); // INC BC
        void opcode0x04(); // INC B
        void opcode0x05(); // DEC B
        void opcode0x06(); // LD B,u8
        void opcode0x07(); // RLCA
        void opcode0x08(); // LD (u16),SP
        void opcode0x09(); // ADD HL,BC
        void opcode0x0A(); // LD A,(BC)
        void opcode0x0B(); // DEC BC
        void opcode0x0C(); // INC C
        void opcode0x0D(); // DEC C
        void opcode0x0E(); // LD C,u8
        void opcode0x0F(); // RRCA

        //0x10

        void opcode0x10(); // STOP
        void opcode0x11(); // LD DE,u16
        void opcode0x12(); // LD (DE),A
        void opcode0x13(); // INC DE
        void opcode0x14(); // INC D
        void opcode0x15(); // DEC D
        void opcode0x16(); // LD D,u8
        void opcode0x17(); // RLA
        void opcode0x18(); // JR i8
        void opcode0x19(); // ADD HL,DE
        void opcode0x1A(); // LD A,(DE)
        void opcode0x1B(); // DEC DE
        void opcode0x1C(); // INC E
        void opcode0x1D(); // DEC E
        void opcode0x1E(); // LD E,u8
        void opcode0x1F(); // RRA

        //0x20

        void opcode0x20(); // JR NZ,i8
        void opcode0x21(); // LD HL,u16
        void opcode0x22(); // LD (HL+),A
        void opcode0x23(); // INC HL
        void opcode0x24(); // INC H
        void opcode0x25(); // DEC H
        void opcode0x26(); // LD H,u8
        void opcode0x27(); // DAA
        void opcode0x28(); // JR Z,i8
        void opcode0x29(); // ADD HL,HL
        void opcode0x2A(); // LD A,(HL+)
        void opcode0x2B(); // DEC HL
        void opcode0x2C(); // INC L
        void opcode0x2D(); // DEC L
        void opcode0x2E(); // LD L,u8
        void opcode0x2F(); // CPL

        //0x30

        void opcode0x30(); // JR NC,i8
        void opcode0x31(); // LD SP,u16
        void opcode0x32(); // LD (HL-),A
        void opcode0x33(); // INC SP
        void opcode0x34(); // INC (HL)
        void opcode0x35(); // DEC (HL)
        void opcode0x36(); // LD (HL),u8
        void opcode0x37(); // SCF
        void opcode0x38(); // JR C,i8
        void opcode0x39(); // ADD HL,SP
        void opcode0x3A(); // LD A,(HL-)
        void opcode0x3B(); // DEC SP
        void opcode0x3C(); // INC A
        void opcode0x3D(); // DEC A
        void opcode0x3E(); // LD A,u8
        void opcode0x3F(); // CCF

        //0x40

        void opcode0x40(); // LD B,B
        void opcode0x41(); // LD B,C
        void opcode0x42(); // LD B,D
        void opcode0x43(); // LD B,E
        void opcode0x44(); // LD B,H
        void opcode0x45(); // LD B,L
        void opcode0x46(); // LD B,(HL)
        void opcode0x47(); // LD B,A
        void opcode0x48(); // LD C,B
        void opcode0x49(); // LD C,C
        void opcode0x4A(); // LD C,D
        void opcode0x4B(); // LD C,E
        void opcode0x4C(); // LD C,H
        void opcode0x4D(); // LD C,L
        void opcode0x4E(); // LD C,(HL)
        void opcode0x4F(); // LD C,A

        //0x50

        void opcode0x50(); // LD D,B
        void opcode0x51(); // LD D,C
        void opcode0x52(); // LD D,D
        void opcode0x53(); // LD D,E
        void opcode0x54(); // LD D,H
        void opcode0x55(); // LD D,L
        void opcode0x56(); // LD D,(HL)
        void opcode0x57(); // LD D,A
        void opcode0x58(); // LD E,B
        void opcode0x59(); // LD E,C
        void opcode0x5A(); // LD E,D
        void opcode0x5B(); // LD E,E
        void opcode0x5C(); // LD E,H
        void opcode0x5D(); // LD E,L
        void opcode0x5E(); // LD E,(HL)
        void opcode0x5F(); // LD E,A

        //0x60

        void opcode0x60(); // LD H,B
        void opcode0x61(); // LD H,C
        void opcode0x62(); // LD H,D
        void opcode0x63(); // LD H,E
        void opcode0x64(); // LD H,H
        void opcode0x65(); // LD H,L
        void opcode0x66(); // LD H,(HL)
        void opcode0x67(); // LD H,A
        void opcode0x68(); // LD L,B
        void opcode0x69(); // LD L,C
        void opcode0x6A(); // LD L,D
        void opcode0x6B(); // LD L,E
        void opcode0x6C(); // LD L,H
        void opcode0x6D(); // LD L,L
        void opcode0x6E(); // LD L,(HL)
        void opcode0x6F(); // LD L,A

        //0x70

        void opcode0x70(); // LD (HL),B
        void opcode0x71(); // LD (HL),C
        void opcode0x72(); // LD (HL),D
        void opcode0x73(); // LD (HL),E
        void opcode0x74(); // LD (HL),H
        void opcode0x75(); // LD (HL),L
        void opcode0x76(); // HALT
        void opcode0x77(); // LD (HL),A
        void opcode0x78(); // LD A,B
        void opcode0x79(); // LD A,C
        void opcode0x7A(); // LD A,D
        void opcode0x7B(); // LD A,E
        void opcode0x7C(); // LD A,H
        void opcode0x7D(); // LD A,L
        void opcode0x7E(); // LD A,(HL)
        void opcode0x7F(); // LD A,A

        //0x80

        void opcode0x80(); // ADD A,B
        void opcode0x81(); // ADD A,C
        void opcode0x82(); // ADD A,D
        void opcode0x83(); // ADD A,E
        void opcode0x84(); // ADD A,H
        void opcode0x85(); // ADD A,L
        void opcode0x86(); // ADD A,(HL)
        void opcode0x87(); // ADD A,A
        void opcode0x88(); // ADC A,B
        void opcode0x89(); // ADC A,C
        void opcode0x8A(); // ADC A,D
        void opcode0x8B(); // ADC A,E
        void opcode0x8C(); // ADC A,H
        void opcode0x8D(); // ADC A,L
        void opcode0x8E(); // ADC A,(HL)
        void opcode0x8F(); // ADC A,A

        //0x90

        void opcode0x90(); // SUB A,B
        void opcode0x91(); // SUB A,C
        void opcode0x92(); // SUB A,D
        void opcode0x93(); // SUB A,E
        void opcode0x94(); // SUB A,H
        void opcode0x95(); // SUB A,L
        void opcode0x96(); // SUB A,(HL)
        void opcode0x97(); // SUB A,A
        void opcode0x98(); // SBC A,B
        void opcode0x99(); // SBC A,C
        void opcode0x9A(); // SBC A,D
        void opcode0x9B(); // SBC A,E
        void opcode0x9C(); // SBC A,H
        void opcode0x9D(); // SBC A,L
        void opcode0x9E(); // SBC A,(HL)
        void opcode0x9F(); // SBC A,A

        //0xA0

        void opcode0xA0(); // AND A,B
        void opcode0xA1(); // AND A,C
        void opcode0xA2(); // AND A,D
        void opcode0xA3(); // AND A,E
        void opcode0xA4(); // AND A,H
        void opcode0xA5(); // AND A,L
        void opcode0xA6(); // AND A,(HL)
        void opcode0xA7(); // AND A,A
        void opcode0xA8(); // XOR A,B
        void opcode0xA9(); // XOR A,C
        void opcode0xAA(); // XOR A,D
        void opcode0xAB(); // XOR A,E
        void opcode0xAC(); // XOR A,H
        void opcode0xAD(); // XOR A,L
        void opcode0xAE(); // XOR A,(HL)
        void opcode0xAF(); // XOR A,A

        //0xB0

        void opcode0xB0(); // OR A,B
        void opcode0xB1(); // OR A,C
        void opcode0xB2(); // OR A,D
        void opcode0xB3(); // OR A,E
        void opcode0xB4(); // OR A,H
        void opcode0xB5(); // OR A,L
        void opcode0xB6(); // OR A,(HL)
        void opcode0xB7(); // OR A,A
        void opcode0xB8(); // CP A,B
        void opcode0xB9(); // CP A,C
        void opcode0xBA(); // CP A,D
        void opcode0xBB(); // CP A,E
        void opcode0xBC(); // CP A,H
        void opcode0xBD(); // CP A,L
        void opcode0xBE(); // CP A,(HL)
        void opcode0xBF(); // CP A,A

        //0xC0

        void opcode0xC0(); // RET NZ
        void opcode0xC1(); // POP BC
        void opcode0xC2(); // JP NZ,u16
        void opcode0xC3(); // JP u16
        void opcode0xC4(); // CALL NZ,u16
        void opcode0xC5(); // PUSH BC
        void opcode0xC6(); // ADD A,u8
        void opcode0xC7(); // RST 00h
        void opcode0xC8(); // RET Z
        void opcode0xC9(); // RET
        void opcode0xCA(); // JP Z,u16
        // void opcode0xCB(); // PREFIX CB
        void opcode0xCC(); // CALL Z,u16
        void opcode0xCD(); // CALL u16
        void opcode0xCE(); // ADC A,u8
        void opcode0xCF(); // RST 08h

        //0xD0

        void opcode0xD0(); // RET NC
        void opcode0xD1(); // POP DE
        void opcode0xD2(); // JP NC,u16
        void opcode0xD3(); // UNUSED
        void opcode0xD4(); // CALL NC,u16
        void opcode0xD5(); // PUSH DE
        void opcode0xD6(); // SUB A,u8
        void opcode0xD7(); // RST 10h
        void opcode0xD8(); // RET C
        void opcode0xD9(); // RETI
        void opcode0xDA(); // JP C,u16
        void opcode0xDB(); // UNUSED
        void opcode0xDC(); // CALL C,u16
        void opcode0xDD(); // UNUSED
        void opcode0xDE(); // SBC A,u8
        void opcode0xDF(); // RST 18h

        //0xE0

        void opcode0xE0(); // LD (FF00+u8),A
        void opcode0xE1(); // POP HL
        void opcode0xE2(); // LD (FF00+C),A
        void opcode0xE3(); // UNUSED
        void opcode0xE4(); // UNUSED
        void opcode0xE5(); // PUSH HL
        void opcode0xE6(); // AND A,u8
        void opcode0xE7(); // RST 20h
        void opcode0xE8(); // ADD SP,i8
        void opcode0xE9(); // JP HL
        void opcode0xEA(); // LD (u16),A
        void opcode0xEB(); // UNUSED
        void opcode0xEC(); // UNUSED
        void opcode0xED(); // UNUSED
        void opcode0xEE(); // XOR A,u8
        void opcode0xEF(); // RST 28h

        //0xF0

        void opcode0xF0(); // LD A,(FF00+u8)
        void opcode0xF1(); // POP AF
        void opcode0xF2(); // LD A,(FF00+C)
        void opcode0xF3(); // DI
        void opcode0xF4(); // UNUSED
        void opcode0xF5(); // PUSH AF
        void opcode0xF6(); // OR A,u8
        void opcode0xF7(); // RST 30h
        void opcode0xF8(); // LD HL,SP+i8
        void opcode0xF9(); // LD SP,HL
        void opcode0xFA(); // LD A,(u16)
        void opcode0xFB(); // EI
        void opcode0xFC(); // UNUSED
        void opcode0xFD(); // UNUSED
        void opcode0xFE(); // CP A,u8
        void opcode0xFF(); // RST 38h

        //--------------------------------------CB Opcodes--------------------------------------//

        //0x00

        void opcodeCB0x00(); // RLC B
        void opcodeCB0x01(); // RLC C
        void opcodeCB0x02(); // RLC D
        void opcodeCB0x03(); // RLC E
        void opcodeCB0x04(); // RLC H
        void opcodeCB0x05(); // RLC L
        void opcodeCB0x06(); // RLC (HL)
        void opcodeCB0x07(); // RLC A
        void opcodeCB0x08(); // RRC B
        void opcodeCB0x09(); // RRC C
        void opcodeCB0x0A(); // RRC D
        void opcodeCB0x0B(); // RRC E
        void opcodeCB0x0C(); // RRC H
        void opcodeCB0x0D(); // RRC L
        void opcodeCB0x0E(); // RRC (HL)
        void opcodeCB0x0F(); // RRC A

        //0x10

        void opcodeCB0x10(); // RL B
        void opcodeCB0x11(); // RL C
        void opcodeCB0x12(); // RL D
        void opcodeCB0x13(); // RL E
        void opcodeCB0x14(); // RL H
        void opcodeCB0x15(); // RL L
        void opcodeCB0x16(); // RL (HL)
        void opcodeCB0x17(); // RL A
        void opcodeCB0x18(); // RR B
        void opcodeCB0x19(); // RR C
        void opcodeCB0x1A(); // RR D
        void opcodeCB0x1B(); // RR E
        void opcodeCB0x1C(); // RR H
        void opcodeCB0x1D(); // RR L
        void opcodeCB0x1E(); // RR (HL)
        void opcodeCB0x1F(); // RR A

        //0x20

        void opcodeCB0x20(); // SLA B
        void opcodeCB0x21(); // SLA C
        void opcodeCB0x22(); // SLA D
        void opcodeCB0x23(); // SLA E
        void opcodeCB0x24(); // SLA H
        void opcodeCB0x25(); // SLA L
        void opcodeCB0x26(); // SLA (HL)
        void opcodeCB0x27(); // SLA A
        void opcodeCB0x28(); // SRA B
        void opcodeCB0x29(); // SRA C
        void opcodeCB0x2A(); // SRA D
        void opcodeCB0x2B(); // SRA E
        void opcodeCB0x2C(); // SRA H
        void opcodeCB0x2D(); // SRA L
        void opcodeCB0x2E(); // SRA (HL)
        void opcodeCB0x2F(); // SRA A

        //0x30

        void opcodeCB0x30(); // SWAP B
        void opcodeCB0x31(); // SWAP C
        void opcodeCB0x32(); // SWAP D
        void opcodeCB0x33(); // SWAP E
        void opcodeCB0x34(); // SWAP H
        void opcodeCB0x35(); // SWAP L
        void opcodeCB0x36(); // SWAP (HL)
        void opcodeCB0x37(); // SWAP A
        void opcodeCB0x38(); // SRL B
        void opcodeCB0x39(); // SRL C
        void opcodeCB0x3A(); // SRL D
        void opcodeCB0x3B(); // SRL E
        void opcodeCB0x3C(); // SRL H
        void opcodeCB0x3D(); // SRL L
        void opcodeCB0x3E(); // SRL (HL)
        void opcodeCB0x3F(); // SRL A

        //0x40

        void opcodeCB0x40(); // BIT 0,B
        void opcodeCB0x41(); // BIT 0,C
        void opcodeCB0x42(); // BIT 0,D
        void opcodeCB0x43(); // BIT 0,E
        void opcodeCB0x44(); // BIT 0,H
        void opcodeCB0x45(); // BIT 0,L
        void opcodeCB0x46(); // BIT 0,(HL)
        void opcodeCB0x47(); // BIT 0,A
        void opcodeCB0x48(); // BIT 1,B
        void opcodeCB0x49(); // BIT 1,C
        void opcodeCB0x4A(); // BIT 1,D
        void opcodeCB0x4B(); // BIT 1,E
        void opcodeCB0x4C(); // BIT 1,H
        void opcodeCB0x4D(); // BIT 1,L
        void opcodeCB0x4E(); // BIT 1,(HL)
        void opcodeCB0x4F(); // BIT 1,A

        //0x50

        void opcodeCB0x50(); // BIT 2,B
        void opcodeCB0x51(); // BIT 2,C
        void opcodeCB0x52(); // BIT 2,D
        void opcodeCB0x53(); // BIT 2,E
        void opcodeCB0x54(); // BIT 2,H
        void opcodeCB0x55(); // BIT 2,L
        void opcodeCB0x56(); // BIT 2,(HL)
        void opcodeCB0x57(); // BIT 2,A
        void opcodeCB0x58(); // BIT 3,B
        void opcodeCB0x59(); // BIT 3,C
        void opcodeCB0x5A(); // BIT 3,D
        void opcodeCB0x5B(); // BIT 3,E
        void opcodeCB0x5C(); // BIT 3,H
        void opcodeCB0x5D(); // BIT 3,L
        void opcodeCB0x5E(); // BIT 3,(HL)
        void opcodeCB0x5F(); // BIT 3,A

        //0x60

        void opcodeCB0x60(); // BIT 4,B
        void opcodeCB0x61(); // BIT 4,C
        void opcodeCB0x62(); // BIT 4,D
        void opcodeCB0x63(); // BIT 4,E
        void opcodeCB0x64(); // BIT 4,H
        void opcodeCB0x65(); // BIT 4,L
        void opcodeCB0x66(); // BIT 4,(HL)
        void opcodeCB0x67(); // BIT 4,A
        void opcodeCB0x68(); // BIT 5,B
        void opcodeCB0x69(); // BIT 5,C
        void opcodeCB0x6A(); // BIT 5,D
        void opcodeCB0x6B(); // BIT 5,E
        void opcodeCB0x6C(); // BIT 5,H
        void opcodeCB0x6D(); // BIT 5,L
        void opcodeCB0x6E(); // BIT 5,(HL)
        void opcodeCB0x6F(); // BIT 5,A

        //0x70

        void opcodeCB0x70(); // BIT 6,B
        void opcodeCB0x71(); // BIT 6,C
        void opcodeCB0x72(); // BIT 6,D
        void opcodeCB0x73(); // BIT 6,E
        void opcodeCB0x74(); // BIT 6,H
        void opcodeCB0x75(); // BIT 6,L
        void opcodeCB0x76(); // BIT 6,(HL)
        void opcodeCB0x77(); // BIT 6,A
        void opcodeCB0x78(); // BIT 7,B
        void opcodeCB0x79(); // BIT 7,C
        void opcodeCB0x7A(); // BIT 7,D
        void opcodeCB0x7B(); // BIT 7,E
        void opcodeCB0x7C(); // BIT 7,H
        void opcodeCB0x7D(); // BIT 7,L
        void opcodeCB0x7E(); // BIT 7,(HL)
        void opcodeCB0x7F(); // BIT 7,A

        //0x80

        void opcodeCB0x80(); // RES 0,B
        void opcodeCB0x81(); // RES 0,C
        void opcodeCB0x82(); // RES 0,D
        void opcodeCB0x83(); // RES 0,E
        void opcodeCB0x84(); // RES 0,H
        void opcodeCB0x85(); // RES 0,L
        void opcodeCB0x86(); // RES 0,(HL)
        void opcodeCB0x87(); // RES 0,A
        void opcodeCB0x88(); // RES 1,B
        void opcodeCB0x89(); // RES 1,C
        void opcodeCB0x8A(); // RES 1,D
        void opcodeCB0x8B(); // RES 1,E
        void opcodeCB0x8C(); // RES 1,H
        void opcodeCB0x8D(); // RES 1,L
        void opcodeCB0x8E(); // RES 1,(HL)
        void opcodeCB0x8F(); // RES 1,A

        //0x90

        void opcodeCB0x90(); // RES 2,B
        void opcodeCB0x91(); // RES 2,C
        void opcodeCB0x92(); // RES 2,D
        void opcodeCB0x93(); // RES 2,E
        void opcodeCB0x94(); // RES 2,H
        void opcodeCB0x95(); // RES 2,L
        void opcodeCB0x96(); // RES 2,(HL)
        void opcodeCB0x97(); // RES 2,A
        void opcodeCB0x98(); // RES 3,B
        void opcodeCB0x99(); // RES 3,C
        void opcodeCB0x9A(); // RES 3,D
        void opcodeCB0x9B(); // RES 3,E
        void opcodeCB0x9C(); // RES 3,H
        void opcodeCB0x9D(); // RES 3,L
        void opcodeCB0x9E(); // RES 3,(HL)
        void opcodeCB0x9F(); // RES 3,A

        //0xA0

        void opcodeCB0xA0(); // RES 4,B
        void opcodeCB0xA1(); // RES 4,C
        void opcodeCB0xA2(); // RES 4,D
        void opcodeCB0xA3(); // RES 4,E
        void opcodeCB0xA4(); // RES 4,H
        void opcodeCB0xA5(); // RES 4,L
        void opcodeCB0xA6(); // RES 4,(HL)
        void opcodeCB0xA7(); // RES 4,A
        void opcodeCB0xA8(); // RES 5,B
        void opcodeCB0xA9(); // RES 5,C
        void opcodeCB0xAA(); // RES 5,D
        void opcodeCB0xAB(); // RES 5,E
        void opcodeCB0xAC(); // RES 5,H
        void opcodeCB0xAD(); // RES 5,L
        void opcodeCB0xAE(); // RES 5,(HL)
        void opcodeCB0xAF(); // RES 5,A

        //0xB0

        void opcodeCB0xB0(); // RES 6,B
        void opcodeCB0xB1(); // RES 6,C
        void opcodeCB0xB2(); // RES 6,D
        void opcodeCB0xB3(); // RES 6,E
        void opcodeCB0xB4(); // RES 6,H
        void opcodeCB0xB5(); // RES 6,L
        void opcodeCB0xB6(); // RES 6,(HL)
        void opcodeCB0xB7(); // RES 6,A
        void opcodeCB0xB8(); // RES 7,B
        void opcodeCB0xB9(); // RES 7,C
        void opcodeCB0xBA(); // RES 7,D
        void opcodeCB0xBB(); // RES 7,E
        void opcodeCB0xBC(); // RES 7,H
        void opcodeCB0xBD(); // RES 7,L
        void opcodeCB0xBE(); // RES 7,(HL)
        void opcodeCB0xBF(); // RES 7,A

        //0xC0

        void opcodeCB0xC0(); // SET 0,B
        void opcodeCB0xC1(); // SET 0,C
        void opcodeCB0xC2(); // SET 0,D
        void opcodeCB0xC3(); // SET 0,E
        void opcodeCB0xC4(); // SET 0,H
        void opcodeCB0xC5(); // SET 0,L
        void opcodeCB0xC6(); // SET 0,(HL)
        void opcodeCB0xC7(); // SET 0,A
        void opcodeCB0xC8(); // SET 1,B
        void opcodeCB0xC9(); // SET 1,C
        void opcodeCB0xCA(); // SET 1,D
        void opcodeCB0xCB(); // SET 1,E
        void opcodeCB0xCC(); // SET 1,H
        void opcodeCB0xCD(); // SET 1,L
        void opcodeCB0xCE(); // SET 1,(HL)
        void opcodeCB0xCF(); // SET 1,A

        //0xD0

        void opcodeCB0xD0(); // SET 2,B
        void opcodeCB0xD1(); // SET 2,C
        void opcodeCB0xD2(); // SET 2,D
        void opcodeCB0xD3(); // SET 2,E
        void opcodeCB0xD4(); // SET 2,H
        void opcodeCB0xD5(); // SET 2,L
        void opcodeCB0xD6(); // SET 2,(HL)
        void opcodeCB0xD7(); // SET 2,A
        void opcodeCB0xD8(); // SET 3,B
        void opcodeCB0xD9(); // SET 3,C
        void opcodeCB0xDA(); // SET 3,D
        void opcodeCB0xDB(); // SET 3,E
        void opcodeCB0xDC(); // SET 3,H
        void opcodeCB0xDD(); // SET 3,L
        void opcodeCB0xDE(); // SET 3,(HL)
        void opcodeCB0xDF(); // SET 3,A

        //0xE0

        void opcodeCB0xE0(); // SET 4,B
        void opcodeCB0xE1(); // SET 4,C
        void opcodeCB0xE2(); // SET 4,D
        void opcodeCB0xE3(); // SET 4,E
        void opcodeCB0xE4(); // SET 4,H
        void opcodeCB0xE5(); // SET 4,L
        void opcodeCB0xE6(); // SET 4,(HL)
        void opcodeCB0xE7(); // SET 4,A
        void opcodeCB0xE8(); // SET 5,B
        void opcodeCB0xE9(); // SET 5,C
        void opcodeCB0xEA(); // SET 5,D
        void opcodeCB0xEB(); // SET 5,E
        void opcodeCB0xEC(); // SET 5,H
        void opcodeCB0xED(); // SET 5,L
        void opcodeCB0xEE(); // SET 5,(HL)
        void opcodeCB0xEF(); // SET 5,A

        //0xF0

        void opcodeCB0xF0(); // SET 6,B
        void opcodeCB0xF1(); // SET 6,C
        void opcodeCB0xF2(); // SET 6,D
        void opcodeCB0xF3(); // SET 6,E
        void opcodeCB0xF4(); // SET 6,H
        void opcodeCB0xF5(); // SET 6,L
        void opcodeCB0xF6(); // SET 6,(HL)
        void opcodeCB0xF7(); // SET 6,A
        void opcodeCB0xF8(); // SET 7,B
        void opcodeCB0xF9(); // SET 7,C
        void opcodeCB0xFA(); // SET 7,D
        void opcodeCB0xFB(); // SET 7,E
        void opcodeCB0xFC(); // SET 7,H
        void opcodeCB0xFD(); // SET 7,L
        void opcodeCB0xFE(); // SET 7,(HL)
        void opcodeCB0xFF(); // SET 7,A

        //--------------------------------------Opcode Tables--------------------------------------//

        const std::array<Instruction, 0x100> instructions 
        {{
            //0x00
            INSTRUCTION("NOP",              [this] { CPU::opcode0x00(); }, 1,  4,  4),
            INSTRUCTION("LD BC,u16",        [this] { CPU::opcode0x01(); }, 3, 12, 12),
            INSTRUCTION("LD (BC),A",        [this] { CPU::opcode0x02(); }, 1,  8,  8),
            INSTRUCTION("INC BC",           [this] { CPU::opcode0x03(); }, 1,  8,  8),
            INSTRUCTION("INC B",            [this] { CPU::opcode0x04(); }, 1,  4,  4),
            INSTRUCTION("DEC B",            [this] { CPU::opcode0x05(); }, 1,  4,  4),
            INSTRUCTION("LD B,u8",          [this] { CPU::opcode0x06(); }, 2,  8,  8),
            INSTRUCTION("RLCA",             [this] { CPU::opcode0x07(); }, 1,  4,  4),
            INSTRUCTION("LD (u16),SP",      [this] { CPU::opcode0x08(); }, 3, 20, 20),
            INSTRUCTION("ADD HL,BC",        [this] { CPU::opcode0x09(); }, 1,  8,  8),
            INSTRUCTION("LD A,(BC)",        [this] { CPU::opcode0x0A(); }, 1,  8,  8),
            INSTRUCTION("DEC BC",           [this] { CPU::opcode0x0B(); }, 1,  8,  8),
            INSTRUCTION("INC C",            [this] { CPU::opcode0x0C(); }, 1,  4,  4),
            INSTRUCTION("DEC C",            [this] { CPU::opcode0x0D(); }, 1,  4,  4),
            INSTRUCTION("LD C,u8",          [this] { CPU::opcode0x0E(); }, 2,  8,  8),
            INSTRUCTION("RRCA",             [this] { CPU::opcode0x0F(); }, 1,  4,  4),

            //0x10
            INSTRUCTION("STOP",             [this] { CPU::opcode0x10(); }, 2,  4,  4),
            INSTRUCTION("LD DE,u16",        [this] { CPU::opcode0x11(); }, 3, 12, 12),
            INSTRUCTION("LD (DE),A",        [this] { CPU::opcode0x12(); }, 1,  8,  8),
            INSTRUCTION("INC DE",           [this] { CPU::opcode0x13(); }, 1,  8,  8),
            INSTRUCTION("INC D",            [this] { CPU::opcode0x14(); }, 1,  4,  4),
            INSTRUCTION("DEC D",            [this] { CPU::opcode0x15(); }, 1,  4,  4),
            INSTRUCTION("LD D,u8",          [this] { CPU::opcode0x16(); }, 2,  8,  8),
            INSTRUCTION("RLA",              [this] { CPU::opcode0x17(); }, 1,  4,  4),
            INSTRUCTION("JR i8",            [this] { CPU::opcode0x18(); }, 2, 12, 12),
            INSTRUCTION("ADD HL,DE",        [this] { CPU::opcode0x19(); }, 1,  8,  8),
            INSTRUCTION("LD A,(DE)",        [this] { CPU::opcode0x1A(); }, 1,  8,  8),
            INSTRUCTION("DEC DE",           [this] { CPU::opcode0x1B(); }, 1,  8,  8),
            INSTRUCTION("INC E",            [this] { CPU::opcode0x1C(); }, 1,  4,  4),
            INSTRUCTION("DEC E",            [this] { CPU::opcode0x1D(); }, 1,  4,  4),
            INSTRUCTION("LD E,u8",          [this] { CPU::opcode0x1E(); }, 2,  8,  8),
            INSTRUCTION("RRA",              [this] { CPU::opcode0x1F(); }, 1,  4,  4),

            //0x20
            INSTRUCTION("JR NZ,i8",         [this] { CPU::opcode0x20(); }, 2, 12,  8),
            INSTRUCTION("LD HL,u16",        [this] { CPU::opcode0x21(); }, 3, 12, 12),
            INSTRUCTION("LD (HL+),A",       [this] { CPU::opcode0x22(); }, 1,  8,  8),
            INSTRUCTION("INC HL",           [this] { CPU::opcode0x23(); }, 1,  8,  8),
            INSTRUCTION("INC H",            [this] { CPU::opcode0x24(); }, 1,  4,  4),
            INSTRUCTION("DEC H",            [this] { CPU::opcode0x25(); }, 1,  4,  4),
            INSTRUCTION("LD H,u8",          [this] { CPU::opcode0x26(); }, 2,  8,  8),
            INSTRUCTION("DAA",              [this] { CPU::opcode0x27(); }, 1,  4,  4),
            INSTRUCTION("JR Z,i8",          [this] { CPU::opcode0x28(); }, 2, 12,  8),
            INSTRUCTION("ADD HL,HL",        [this] { CPU::opcode0x29(); }, 1,  8,  8),
            INSTRUCTION("LD A,(HL+)",       [this] { CPU::opcode0x2A(); }, 1,  8,  8),
            INSTRUCTION("DEC HL",           [this] { CPU::opcode0x2B(); }, 1,  8,  8),
            INSTRUCTION("INC L",            [this] { CPU::opcode0x2C(); }, 1,  4,  4),
            INSTRUCTION("DEC L",            [this] { CPU::opcode0x2D(); }, 1,  4,  4),
            INSTRUCTION("LD L,u8",          [this] { CPU::opcode0x2E(); }, 2,  8,  8),
            INSTRUCTION("CPL",              [this] { CPU::opcode0x2F(); }, 1,  4,  4),

            //0x30
            INSTRUCTION("JR NC,i8",         [this] { CPU::opcode0x30(); }, 2, 12,  8),
            INSTRUCTION("LD SP,u16",        [this] { CPU::opcode0x31(); }, 3, 12, 12),
            INSTRUCTION("LD (HL-),A",       [this] { CPU::opcode0x32(); }, 1,  8,  8),
            INSTRUCTION("INC SP",           [this] { CPU::opcode0x33(); }, 1,  8,  8),
            INSTRUCTION("INC (HL)",         [this] { CPU::opcode0x34(); }, 1, 12, 12),
            INSTRUCTION("DEC (HL)",         [this] { CPU::opcode0x35(); }, 1, 12, 12),
            INSTRUCTION("LD (HL),u8",       [this] { CPU::opcode0x36(); }, 2, 12, 12),
            INSTRUCTION("SCF",              [this] { CPU::opcode0x37(); }, 1,  4,  4),
            INSTRUCTION("JR C,i8",          [this] { CPU::opcode0x38(); }, 2, 12,  8),
            INSTRUCTION("ADD HL,SP",        [this] { CPU::opcode0x39(); }, 1,  8,  8),
            INSTRUCTION("LD A,(HL-)",       [this] { CPU::opcode0x3A(); }, 1,  8,  8),
            INSTRUCTION("DEC SP",           [this] { CPU::opcode0x3B(); }, 1,  8,  8),
            INSTRUCTION("INC A",            [this] { CPU::opcode0x3C(); }, 1,  4,  4),
            INSTRUCTION("DEC A",            [this] { CPU::opcode0x3D(); }, 1,  4,  4),
            INSTRUCTION("LD A,u8",          [this] { CPU::opcode0x3E(); }, 2,  8,  8),
            INSTRUCTION("CCF",              [this] { CPU::opcode0x3F(); }, 1,  4,  4),

            //0x40
            INSTRUCTION("LD B,B",           [this] { CPU::opcode0x40(); }, 1,  4,  4),
            INSTRUCTION("LD B,C",           [this] { CPU::opcode0x41(); }, 1,  4,  4),
            INSTRUCTION("LD B,D",           [this] { CPU::opcode0x42(); }, 1,  4,  4),
            INSTRUCTION("LD B,E",           [this] { CPU::opcode0x43(); }, 1,  4,  4),
            INSTRUCTION("LD B,H",           [this] { CPU::opcode0x44(); }, 1,  4,  4),
            INSTRUCTION("LD B,L",           [this] { CPU::opcode0x45(); }, 1,  4,  4),
            INSTRUCTION("LD B,(HL)",        [this] { CPU::opcode0x46(); }, 1,  8,  8),
            INSTRUCTION("LD B,A",           [this] { CPU::opcode0x47(); }, 1,  4,  4),
            INSTRUCTION("LD C,B",           [this] { CPU::opcode0x48(); }, 1,  4,  4),
            INSTRUCTION("LD C,C",           [this] { CPU::opcode0x49(); }, 1,  4,  4),
            INSTRUCTION("LD C,D",           [this] { CPU::opcode0x4A(); }, 1,  4,  4),
            INSTRUCTION("LD C,E",           [this] { CPU::opcode0x4B(); }, 1,  4,  4),
            INSTRUCTION("LD C,H",           [this] { CPU::opcode0x4C(); }, 1,  4,  4),
            INSTRUCTION("LD C,L",           [this] { CPU::opcode0x4D(); }, 1,  4,  4),
            INSTRUCTION("LD C,(HL)",        [this] { CPU::opcode0x4E(); }, 1,  8,  8),
            INSTRUCTION("LD C,A",           [this] { CPU::opcode0x4F(); }, 1,  4,  4),

            //0x50
            INSTRUCTION("LD D,B",           [this] { CPU::opcode0x50(); }, 1,  4,  4),
            INSTRUCTION("LD D,C",           [this] { CPU::opcode0x51(); }, 1,  4,  4),
            INSTRUCTION("LD D,D",           [this] { CPU::opcode0x52(); }, 1,  4,  4),
            INSTRUCTION("LD D,E",           [this] { CPU::opcode0x53(); }, 1,  4,  4),
            INSTRUCTION("LD D,H",           [this] { CPU::opcode0x54(); }, 1,  4,  4),
            INSTRUCTION("LD D,L",           [this] { CPU::opcode0x55(); }, 1,  4,  4),
            INSTRUCTION("LD D,(HL)",        [this] { CPU::opcode0x56(); }, 1,  8,  8),
            INSTRUCTION("LD D,A",           [this] { CPU::opcode0x57(); }, 1,  4,  4),
            INSTRUCTION("LD E,B",           [this] { CPU::opcode0x58(); }, 1,  4,  4),
            INSTRUCTION("LD E,C",           [this] { CPU::opcode0x59(); }, 1,  4,  4),
            INSTRUCTION("LD E,D",           [this] { CPU::opcode0x5A(); }, 1,  4,  4),
            INSTRUCTION("LD E,E",           [this] { CPU::opcode0x5B(); }, 1,  4,  4),
            INSTRUCTION("LD E,H",           [this] { CPU::opcode0x5C(); }, 1,  4,  4),
            INSTRUCTION("LD E,L",           [this] { CPU::opcode0x5D(); }, 1,  4,  4),
            INSTRUCTION("LD E,(HL)",        [this] { CPU::opcode0x5E(); }, 1,  8,  8),
            INSTRUCTION("LD E,A",           [this] { CPU::opcode0x5F(); }, 1,  4,  4),

            //0x60
            INSTRUCTION("LD H,B",           [this] { CPU::opcode0x60(); }, 1,  4,  4),
            INSTRUCTION("LD H,C",           [this] { CPU::opcode0x61(); }, 1,  4,  4),
            INSTRUCTION("LD H,D",           [this] { CPU::opcode0x62(); }, 1,  4,  4),
            INSTRUCTION("LD H,E",           [this] { CPU::opcode0x63(); }, 1,  4,  4),
            INSTRUCTION("LD H,H",           [this] { CPU::opcode0x64(); }, 1,  4,  4),
            INSTRUCTION("LD H,L",           [this] { CPU::opcode0x65(); }, 1,  4,  4),
            INSTRUCTION("LD H,(HL)",        [this] { CPU::opcode0x66(); }, 1,  8,  8),
            INSTRUCTION("LD H,A",           [this] { CPU::opcode0x67(); }, 1,  4,  4),
            INSTRUCTION("LD L,B",           [this] { CPU::opcode0x68(); }, 1,  4,  4),
            INSTRUCTION("LD L,C",           [this] { CPU::opcode0x69(); }, 1,  4,  4),
            INSTRUCTION("LD L,D",           [this] { CPU::opcode0x6A(); }, 1,  4,  4),
            INSTRUCTION("LD L,E",           [this] { CPU::opcode0x6B(); }, 1,  4,  4),
            INSTRUCTION("LD L,H",           [this] { CPU::opcode0x6C(); }, 1,  4,  4),
            INSTRUCTION("LD L,L",           [this] { CPU::opcode0x6D(); }, 1,  4,  4),
            INSTRUCTION("LD L,(HL)",        [this] { CPU::opcode0x6E(); }, 1,  8,  8),
            INSTRUCTION("LD L,A",           [this] { CPU::opcode0x6F(); }, 1,  4,  4),

            //0x70
            INSTRUCTION("LD (HL),B",        [this] { CPU::opcode0x70(); }, 1,  8,  8),
            INSTRUCTION("LD (HL),C",        [this] { CPU::opcode0x71(); }, 1,  8,  8),
            INSTRUCTION("LD (HL),D",        [this] { CPU::opcode0x72(); }, 1,  8,  8),
            INSTRUCTION("LD (HL),E",        [this] { CPU::opcode0x73(); }, 1,  8,  8),
            INSTRUCTION("LD (HL),H",        [this] { CPU::opcode0x74(); }, 1,  8,  8),
            INSTRUCTION("LD (HL),L",        [this] { CPU::opcode0x75(); }, 1,  8,  8),
            INSTRUCTION("HALT",             [this] { CPU::opcode0x76(); }, 1,  4,  4),
            INSTRUCTION("LD (HL),A",        [this] { CPU::opcode0x77(); }, 1,  8,  8),
            INSTRUCTION("LD A,B",           [this] { CPU::opcode0x78(); }, 1,  4,  4),
            INSTRUCTION("LD A,C",           [this] { CPU::opcode0x79(); }, 1,  4,  4),
            INSTRUCTION("LD A,D",           [this] { CPU::opcode0x7A(); }, 1,  4,  4),
            INSTRUCTION("LD A,E",           [this] { CPU::opcode0x7B(); }, 1,  4,  4),
            INSTRUCTION("LD A,H",           [this] { CPU::opcode0x7C(); }, 1,  4,  4),
            INSTRUCTION("LD A,L",           [this] { CPU::opcode0x7D(); }, 1,  4,  4),
            INSTRUCTION("LD A,(HL)",        [this] { CPU::opcode0x7E(); }, 1,  8,  8),
            INSTRUCTION("LD A,A",           [this] { CPU::opcode0x7F(); }, 1,  4,  4),

            //0x80  
            INSTRUCTION("ADD A,B",          [this] { CPU::opcode0x80(); }, 1,  4,  4),
            INSTRUCTION("ADD A,C",          [this] { CPU::opcode0x81(); }, 1,  4,  4),
            INSTRUCTION("ADD A,D",          [this] { CPU::opcode0x82(); }, 1,  4,  4),
            INSTRUCTION("ADD A,E",          [this] { CPU::opcode0x83(); }, 1,  4,  4),
            INSTRUCTION("ADD A,H",          [this] { CPU::opcode0x84(); }, 1,  4,  4),
            INSTRUCTION("ADD A,L",          [this] { CPU::opcode0x85(); }, 1,  4,  4),
            INSTRUCTION("ADD A,(HL)",       [this] { CPU::opcode0x86(); }, 1,  8,  8),
            INSTRUCTION("ADD A,A",          [this] { CPU::opcode0x87(); }, 1,  4,  4),
            INSTRUCTION("ADC A,B",          [this] { CPU::opcode0x88(); }, 1,  4,  4),
            INSTRUCTION("ADC A,C",          [this] { CPU::opcode0x89(); }, 1,  4,  4),
            INSTRUCTION("ADC A,D",          [this] { CPU::opcode0x8A(); }, 1,  4,  4),
            INSTRUCTION("ADC A,E",          [this] { CPU::opcode0x8B(); }, 1,  4,  4),
            INSTRUCTION("ADC A,H",          [this] { CPU::opcode0x8C(); }, 1,  4,  4),
            INSTRUCTION("ADC A,L",          [this] { CPU::opcode0x8D(); }, 1,  4,  4),
            INSTRUCTION("ADC A,(HL)",       [this] { CPU::opcode0x8E(); }, 1,  8,  8),
            INSTRUCTION("ADC A,A",          [this] { CPU::opcode0x8F(); }, 1,  4,  4),

            //0x90
            INSTRUCTION("SUB A,B",          [this] { CPU::opcode0x90(); }, 1,  4,  4),
            INSTRUCTION("SUB A,C",          [this] { CPU::opcode0x91(); }, 1,  4,  4),
            INSTRUCTION("SUB A,D",          [this] { CPU::opcode0x92(); }, 1,  4,  4),
            INSTRUCTION("SUB A,E",          [this] { CPU::opcode0x93(); }, 1,  4,  4),
            INSTRUCTION("SUB A,H",          [this] { CPU::opcode0x94(); }, 1,  4,  4),
            INSTRUCTION("SUB A,L",          [this] { CPU::opcode0x95(); }, 1,  4,  4),
            INSTRUCTION("SUB A,(HL)",       [this] { CPU::opcode0x96(); }, 1,  8,  8),
            INSTRUCTION("SUB A,A",          [this] { CPU::opcode0x97(); }, 1,  4,  4),
            INSTRUCTION("SBC A,B",          [this] { CPU::opcode0x98(); }, 1,  4,  4),
            INSTRUCTION("SBC A,C",          [this] { CPU::opcode0x99(); }, 1,  4,  4),
            INSTRUCTION("SBC A,D",          [this] { CPU::opcode0x9A(); }, 1,  4,  4),
            INSTRUCTION("SBC A,E",          [this] { CPU::opcode0x9B(); }, 1,  4,  4),
            INSTRUCTION("SBC A,H",          [this] { CPU::opcode0x9C(); }, 1,  4,  4),
            INSTRUCTION("SBC A,L",          [this] { CPU::opcode0x9D(); }, 1,  4,  4),
            INSTRUCTION("SBC A,(HL)",       [this] { CPU::opcode0x9E(); }, 1,  8,  8),
            INSTRUCTION("SBC A,A",          [this] { CPU::opcode0x9F(); }, 1,  4,  4),

            //0xA0
            INSTRUCTION("AND A,B",          [this] { CPU::opcode0xA0(); }, 1,  4,  4),
            INSTRUCTION("AND A,C",          [this] { CPU::opcode0xA1(); }, 1,  4,  4),
            INSTRUCTION("AND A,D",          [this] { CPU::opcode0xA2(); }, 1,  4,  4),
            INSTRUCTION("AND A,E",          [this] { CPU::opcode0xA3(); }, 1,  4,  4),
            INSTRUCTION("AND A,H",          [this] { CPU::opcode0xA4(); }, 1,  4,  4),
            INSTRUCTION("AND A,L",          [this] { CPU::opcode0xA5(); }, 1,  4,  4),
            INSTRUCTION("AND A,(HL)",       [this] { CPU::opcode0xA6(); }, 1,  8,  8),
            INSTRUCTION("AND A,A",          [this] { CPU::opcode0xA7(); }, 1,  4,  4),
            INSTRUCTION("XOR A,B",          [this] { CPU::opcode0xA8(); }, 1,  4,  4),
            INSTRUCTION("XOR A,C",          [this] { CPU::opcode0xA9(); }, 1,  4,  4),
            INSTRUCTION("XOR A,D",          [this] { CPU::opcode0xAA(); }, 1,  4,  4),
            INSTRUCTION("XOR A,E",          [this] { CPU::opcode0xAB(); }, 1,  4,  4),
            INSTRUCTION("XOR A,H",          [this] { CPU::opcode0xAC(); }, 1,  4,  4),
            INSTRUCTION("XOR A,L",          [this] { CPU::opcode0xAD(); }, 1,  4,  4),
            INSTRUCTION("XOR A,(HL)",       [this] { CPU::opcode0xAE(); }, 1,  8,  8),
            INSTRUCTION("XOR A,A",          [this] { CPU::opcode0xAF(); }, 1,  4,  4),

            //0xB0
            INSTRUCTION("OR A,B",           [this] { CPU::opcode0xB0(); }, 1,  4,  4),
            INSTRUCTION("OR A,C",           [this] { CPU::opcode0xB1(); }, 1,  4,  4),
            INSTRUCTION("OR A,D",           [this] { CPU::opcode0xB2(); }, 1,  4,  4),
            INSTRUCTION("OR A,E",           [this] { CPU::opcode0xB3(); }, 1,  4,  4),
            INSTRUCTION("OR A,H",           [this] { CPU::opcode0xB4(); }, 1,  4,  4),
            INSTRUCTION("OR A,L",           [this] { CPU::opcode0xB5(); }, 1,  4,  4),
            INSTRUCTION("OR A,(HL)",        [this] { CPU::opcode0xB6(); }, 1,  8,  8),
            INSTRUCTION("OR A,A",           [this] { CPU::opcode0xB7(); }, 1,  4,  4),
            INSTRUCTION("CP A,B",           [this] { CPU::opcode0xB8(); }, 1,  4,  4),
            INSTRUCTION("CP A,C",           [this] { CPU::opcode0xB9(); }, 1,  4,  4),
            INSTRUCTION("CP A,D",           [this] { CPU::opcode0xBA(); }, 1,  4,  4),
            INSTRUCTION("CP A,E",           [this] { CPU::opcode0xBB(); }, 1,  4,  4),
            INSTRUCTION("CP A,H",           [this] { CPU::opcode0xBC(); }, 1,  4,  4),
            INSTRUCTION("CP A,L",           [this] { CPU::opcode0xBD(); }, 1,  4,  4),
            INSTRUCTION("CP A,(HL)",        [this] { CPU::opcode0xBE(); }, 1,  8,  8),
            INSTRUCTION("CP A,A",           [this] { CPU::opcode0xBF(); }, 1,  4,  4),

            //0xC0
            INSTRUCTION("RET NZ",           [this] { CPU::opcode0xC0(); }, 1, 20,  8),
            INSTRUCTION("POP BC",           [this] { CPU::opcode0xC1(); }, 1, 12, 12),
            INSTRUCTION("JP NZ,u16",        [this] { CPU::opcode0xC2(); }, 3, 16, 12),
            INSTRUCTION("JP u16",           [this] { CPU::opcode0xC3(); }, 3, 16, 16),
            INSTRUCTION("CALL NZ,u16",      [this] { CPU::opcode0xC4(); }, 3, 24, 12),
            INSTRUCTION("PUSH BC",          [this] { CPU::opcode0xC5(); }, 1, 16, 16),
            INSTRUCTION("ADD A,u8",         [this] { CPU::opcode0xC6(); }, 2,  8,  8),
            INSTRUCTION("RST 00h",          [this] { CPU::opcode0xC7(); }, 1, 16, 16),
            INSTRUCTION("RET Z",            [this] { CPU::opcode0xC8(); }, 1, 20,  8),
            INSTRUCTION("RET",              [this] { CPU::opcode0xC9(); }, 1, 16, 16),
            INSTRUCTION("JP Z,u16",         [this] { CPU::opcode0xCA(); }, 3, 16, 12),
            INSTRUCTION("PREFIX CB",        nullptr,                       1,  4,  4),
            INSTRUCTION("CALL Z,u16",       [this] { CPU::opcode0xCC(); }, 3, 24, 12),
            INSTRUCTION("CALL u16",         [this] { CPU::opcode0xCD(); }, 3, 24, 24),
            INSTRUCTION("ADC A,u8",         [this] { CPU::opcode0xCE(); }, 2,  8,  8),
            INSTRUCTION("RST 08h",          [this] { CPU::opcode0xCF(); }, 1, 16, 16),

            //0xD0
            INSTRUCTION("RET NC",           [this] { CPU::opcode0xD0(); }, 1, 20,  8),
            INSTRUCTION("POP DE",           [this] { CPU::opcode0xD1(); }, 1, 12, 12),
            INSTRUCTION("JP NC,u16",        [this] { CPU::opcode0xD2(); }, 3, 16, 12),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("CALL NC,u16",      [this] { CPU::opcode0xD4(); }, 3, 24, 12),
            INSTRUCTION("PUSH DE",          [this] { CPU::opcode0xD5(); }, 1, 16, 16),
            INSTRUCTION("SUB A,u8",         [this] { CPU::opcode0xD6(); }, 2,  8,  8),
            INSTRUCTION("RST 10h",          [this] { CPU::opcode0xD7(); }, 1, 16, 16),
            INSTRUCTION("RET C",            [this] { CPU::opcode0xD8(); }, 1, 20,  8),
            INSTRUCTION("RETI",             [this] { CPU::opcode0xD9(); }, 1, 16, 16),
            INSTRUCTION("JP C,u16",         [this] { CPU::opcode0xDA(); }, 3, 16, 12),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("CALL C,u16",       [this] { CPU::opcode0xDC(); }, 3, 24, 12),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("SBC A,u8",         [this] { CPU::opcode0xDE(); }, 2,  8,  8),
            INSTRUCTION("RST 18h",          [this] { CPU::opcode0xDF(); }, 1, 16, 16),

            //0xE0
            INSTRUCTION("LD (FF00+u8),A",   [this] { CPU::opcode0xE0(); }, 2, 12, 12),
            INSTRUCTION("POP HL",           [this] { CPU::opcode0xE1(); }, 1, 12, 12),
            INSTRUCTION("LD (FF00+C),A",    [this] { CPU::opcode0xE2(); }, 1,  8,  8),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("PUSH HL",          [this] { CPU::opcode0xE5(); }, 1, 16, 16),
            INSTRUCTION("AND A,u8",         [this] { CPU::opcode0xE6(); }, 2,  8,  8),
            INSTRUCTION("RST 20h",          [this] { CPU::opcode0xE7(); }, 1, 16, 16),
            INSTRUCTION("ADD SP,i8",        [this] { CPU::opcode0xE8(); }, 2, 16, 16),
            INSTRUCTION("JP HL",            [this] { CPU::opcode0xE9(); }, 1,  4,  4),
            INSTRUCTION("LD (u16),A",       [this] { CPU::opcode0xEA(); }, 3, 16, 16),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("XOR A,u8",         [this] { CPU::opcode0xEE(); }, 2,  8,  8),
            INSTRUCTION("RST 28h",          [this] { CPU::opcode0xEF(); }, 1, 16, 16),

            //0xF0
            INSTRUCTION("LD A,(FF00+u8)",   [this] { CPU::opcode0xF0(); }, 2, 12, 12),
            INSTRUCTION("POP AF",           [this] { CPU::opcode0xF1(); }, 1, 12, 12),
            INSTRUCTION("LD A,(FF00+C)",    [this] { CPU::opcode0xF2(); }, 1,  8,  8),
            INSTRUCTION("DI",               [this] { CPU::opcode0xF3(); }, 1,  4,  4),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("PUSH AF",          [this] { CPU::opcode0xF5(); }, 1, 16, 16),
            INSTRUCTION("OR A,u8",          [this] { CPU::opcode0xF6(); }, 2,  8,  8),
            INSTRUCTION("RST 30h",          [this] { CPU::opcode0xF7(); }, 1, 16, 16),
            INSTRUCTION("LD HL,SP+i8",      [this] { CPU::opcode0xF8(); }, 2, 12, 12),
            INSTRUCTION("LD SP,HL",         [this] { CPU::opcode0xF9(); }, 1,  8,  8),
            INSTRUCTION("LD A,(u16)",       [this] { CPU::opcode0xFA(); }, 3, 16, 16),
            INSTRUCTION("EI",               [this] { CPU::opcode0xFB(); }, 1,  4,  4),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("UNUSED",           nullptr,                       1,  0,  0),
            INSTRUCTION("CP A,u8",          [this] { CPU::opcode0xFE(); }, 2,  8,  8),
            INSTRUCTION("RST 38h",          [this] { CPU::opcode0xFF(); }, 1, 16, 16)
        }};
        
        const std::array<Instruction, 0x100> instructionsCB 
        {{
            //0x00
            INSTRUCTION("RLC B",            [this] { CPU::opcodeCB0x00(); }, 2,  8,  8),
            INSTRUCTION("RLC C",            [this] { CPU::opcodeCB0x01(); }, 2,  8,  8),
            INSTRUCTION("RLC D",            [this] { CPU::opcodeCB0x02(); }, 2,  8,  8),
            INSTRUCTION("RLC E",            [this] { CPU::opcodeCB0x03(); }, 2,  8,  8),
            INSTRUCTION("RLC H",            [this] { CPU::opcodeCB0x04(); }, 2,  8,  8),
            INSTRUCTION("RLC L",            [this] { CPU::opcodeCB0x05(); }, 2,  8,  8),
            INSTRUCTION("RLC (HL)",         [this] { CPU::opcodeCB0x06(); }, 2, 16, 16),
            INSTRUCTION("RLC A",            [this] { CPU::opcodeCB0x07(); }, 2,  8,  8),
            INSTRUCTION("RRC B",            [this] { CPU::opcodeCB0x08(); }, 2,  8,  8),
            INSTRUCTION("RRC C",            [this] { CPU::opcodeCB0x09(); }, 2,  8,  8),
            INSTRUCTION("RRC D",            [this] { CPU::opcodeCB0x0A(); }, 2,  8,  8),
            INSTRUCTION("RRC E",            [this] { CPU::opcodeCB0x0B(); }, 2,  8,  8),
            INSTRUCTION("RRC H",            [this] { CPU::opcodeCB0x0C(); }, 2,  8,  8),
            INSTRUCTION("RRC L",            [this] { CPU::opcodeCB0x0D(); }, 2,  8,  8),
            INSTRUCTION("RRC (HL)",         [this] { CPU::opcodeCB0x0E(); }, 2, 16, 16),
            INSTRUCTION("RRC A",            [this] { CPU::opcodeCB0x0F(); }, 2,  8,  8),

            //0x10
            INSTRUCTION("RL B",             [this] { CPU::opcodeCB0x10(); }, 2,  8,  8),
            INSTRUCTION("RL C",             [this] { CPU::opcodeCB0x11(); }, 2,  8,  8),
            INSTRUCTION("RL D",             [this] { CPU::opcodeCB0x12(); }, 2,  8,  8),
            INSTRUCTION("RL E",             [this] { CPU::opcodeCB0x13(); }, 2,  8,  8),
            INSTRUCTION("RL H",             [this] { CPU::opcodeCB0x14(); }, 2,  8,  8),
            INSTRUCTION("RL L",             [this] { CPU::opcodeCB0x15(); }, 2,  8,  8),
            INSTRUCTION("RL (HL)",          [this] { CPU::opcodeCB0x16(); }, 2, 16, 16),
            INSTRUCTION("RL A",             [this] { CPU::opcodeCB0x17(); }, 2,  8,  8),
            INSTRUCTION("RR B",             [this] { CPU::opcodeCB0x18(); }, 2,  8,  8),
            INSTRUCTION("RR C",             [this] { CPU::opcodeCB0x19(); }, 2,  8,  8),
            INSTRUCTION("RR D",             [this] { CPU::opcodeCB0x1A(); }, 2,  8,  8),
            INSTRUCTION("RR E",             [this] { CPU::opcodeCB0x1B(); }, 2,  8,  8),
            INSTRUCTION("RR H",             [this] { CPU::opcodeCB0x1C(); }, 2,  8,  8),
            INSTRUCTION("RR L",             [this] { CPU::opcodeCB0x1D(); }, 2,  8,  8),
            INSTRUCTION("RR (HL)",          [this] { CPU::opcodeCB0x1E(); }, 2, 16, 16),
            INSTRUCTION("RR A",             [this] { CPU::opcodeCB0x1F(); }, 2,  8,  8),

            //0x20
            INSTRUCTION("SLA B",            [this] { CPU::opcodeCB0x20(); }, 2,  8,  8),
            INSTRUCTION("SLA C",            [this] { CPU::opcodeCB0x21(); }, 2,  8,  8),
            INSTRUCTION("SLA D",            [this] { CPU::opcodeCB0x22(); }, 2,  8,  8),
            INSTRUCTION("SLA E",            [this] { CPU::opcodeCB0x23(); }, 2,  8,  8),
            INSTRUCTION("SLA H",            [this] { CPU::opcodeCB0x24(); }, 2,  8,  8),
            INSTRUCTION("SLA L",            [this] { CPU::opcodeCB0x25(); }, 2,  8,  8),
            INSTRUCTION("SLA (HL)",         [this] { CPU::opcodeCB0x26(); }, 2, 16, 16),
            INSTRUCTION("SLA A",            [this] { CPU::opcodeCB0x27(); }, 2,  8,  8),
            INSTRUCTION("SRA B",            [this] { CPU::opcodeCB0x28(); }, 2,  8,  8),
            INSTRUCTION("SRA C",            [this] { CPU::opcodeCB0x29(); }, 2,  8,  8),
            INSTRUCTION("SRA D",            [this] { CPU::opcodeCB0x2A(); }, 2,  8,  8),
            INSTRUCTION("SRA E",            [this] { CPU::opcodeCB0x2B(); }, 2,  8,  8),
            INSTRUCTION("SRA H",            [this] { CPU::opcodeCB0x2C(); }, 2,  8,  8),
            INSTRUCTION("SRA L",            [this] { CPU::opcodeCB0x2D(); }, 2,  8,  8),
            INSTRUCTION("SRA (HL)",         [this] { CPU::opcodeCB0x2E(); }, 2, 16, 16),
            INSTRUCTION("SRA A",            [this] { CPU::opcodeCB0x2F(); }, 2,  8,  8),

            //0x30
            INSTRUCTION("SWAP B",           [this] { CPU::opcodeCB0x30(); }, 2,  8,  8),
            INSTRUCTION("SWAP C",           [this] { CPU::opcodeCB0x31(); }, 2,  8,  8),
            INSTRUCTION("SWAP D",           [this] { CPU::opcodeCB0x32(); }, 2,  8,  8),
            INSTRUCTION("SWAP E",           [this] { CPU::opcodeCB0x33(); }, 2,  8,  8),
            INSTRUCTION("SWAP H",           [this] { CPU::opcodeCB0x34(); }, 2,  8,  8),
            INSTRUCTION("SWAP L",           [this] { CPU::opcodeCB0x35(); }, 2,  8,  8),
            INSTRUCTION("SWAP (HL)",        [this] { CPU::opcodeCB0x36(); }, 2, 16, 16),
            INSTRUCTION("SWAP A",           [this] { CPU::opcodeCB0x37(); }, 2,  8,  8),
            INSTRUCTION("SRL B",            [this] { CPU::opcodeCB0x38(); }, 2,  8,  8),
            INSTRUCTION("SRL C",            [this] { CPU::opcodeCB0x39(); }, 2,  8,  8),
            INSTRUCTION("SRL D",            [this] { CPU::opcodeCB0x3A(); }, 2,  8,  8),
            INSTRUCTION("SRL E",            [this] { CPU::opcodeCB0x3B(); }, 2,  8,  8),
            INSTRUCTION("SRL H",            [this] { CPU::opcodeCB0x3C(); }, 2,  8,  8),
            INSTRUCTION("SRL L",            [this] { CPU::opcodeCB0x3D(); }, 2,  8,  8),
            INSTRUCTION("SRL (HL)",         [this] { CPU::opcodeCB0x3E(); }, 2, 16, 16),
            INSTRUCTION("SRL A",            [this] { CPU::opcodeCB0x3F(); }, 2,  8,  8),

            //0x40
            INSTRUCTION("BIT 0,B",          [this] { CPU::opcodeCB0x40(); }, 2,  8,  8),
            INSTRUCTION("BIT 0,C",          [this] { CPU::opcodeCB0x41(); }, 2,  8,  8),
            INSTRUCTION("BIT 0,D",          [this] { CPU::opcodeCB0x42(); }, 2,  8,  8),
            INSTRUCTION("BIT 0,E",          [this] { CPU::opcodeCB0x43(); }, 2,  8,  8),
            INSTRUCTION("BIT 0,H",          [this] { CPU::opcodeCB0x44(); }, 2,  8,  8),
            INSTRUCTION("BIT 0,L",          [this] { CPU::opcodeCB0x45(); }, 2,  8,  8),
            INSTRUCTION("BIT 0,(HL)",       [this] { CPU::opcodeCB0x46(); }, 2, 12, 12),
            INSTRUCTION("BIT 0,A",          [this] { CPU::opcodeCB0x47(); }, 2,  8,  8),
            INSTRUCTION("BIT 1,B",          [this] { CPU::opcodeCB0x48(); }, 2,  8,  8),
            INSTRUCTION("BIT 1,C",          [this] { CPU::opcodeCB0x49(); }, 2,  8,  8),
            INSTRUCTION("BIT 1,D",          [this] { CPU::opcodeCB0x4A(); }, 2,  8,  8),
            INSTRUCTION("BIT 1,E",          [this] { CPU::opcodeCB0x4B(); }, 2,  8,  8),
            INSTRUCTION("BIT 1,H",          [this] { CPU::opcodeCB0x4C(); }, 2,  8,  8),
            INSTRUCTION("BIT 1,L",          [this] { CPU::opcodeCB0x4D(); }, 2,  8,  8),
            INSTRUCTION("BIT 1,(HL)",       [this] { CPU::opcodeCB0x4E(); }, 2, 12, 12),
            INSTRUCTION("BIT 1,A",          [this] { CPU::opcodeCB0x4F(); }, 2,  8,  8),

            //0x50
            INSTRUCTION("BIT 2,B",          [this] { CPU::opcodeCB0x50(); }, 2,  8,  8),
            INSTRUCTION("BIT 2,C",          [this] { CPU::opcodeCB0x51(); }, 2,  8,  8),
            INSTRUCTION("BIT 2,D",          [this] { CPU::opcodeCB0x52(); }, 2,  8,  8),
            INSTRUCTION("BIT 2,E",          [this] { CPU::opcodeCB0x53(); }, 2,  8,  8),
            INSTRUCTION("BIT 2,H",          [this] { CPU::opcodeCB0x54(); }, 2,  8,  8),
            INSTRUCTION("BIT 2,L",          [this] { CPU::opcodeCB0x55(); }, 2,  8,  8),
            INSTRUCTION("BIT 2,(HL)",       [this] { CPU::opcodeCB0x56(); }, 2, 12, 12),
            INSTRUCTION("BIT 2,A",          [this] { CPU::opcodeCB0x57(); }, 2,  8,  8),
            INSTRUCTION("BIT 3,B",          [this] { CPU::opcodeCB0x58(); }, 2,  8,  8),
            INSTRUCTION("BIT 3,C",          [this] { CPU::opcodeCB0x59(); }, 2,  8,  8),
            INSTRUCTION("BIT 3,D",          [this] { CPU::opcodeCB0x5A(); }, 2,  8,  8),
            INSTRUCTION("BIT 3,E",          [this] { CPU::opcodeCB0x5B(); }, 2,  8,  8),
            INSTRUCTION("BIT 3,H",          [this] { CPU::opcodeCB0x5C(); }, 2,  8,  8),
            INSTRUCTION("BIT 3,L",          [this] { CPU::opcodeCB0x5D(); }, 2,  8,  8),
            INSTRUCTION("BIT 3,(HL)",       [this] { CPU::opcodeCB0x5E(); }, 2, 12, 12),
            INSTRUCTION("BIT 3,A",          [this] { CPU::opcodeCB0x5F(); }, 2,  8,  8),

            //0x60
            INSTRUCTION("BIT 4,B",          [this] { CPU::opcodeCB0x60(); }, 2,  8,  8),
            INSTRUCTION("BIT 4,C",          [this] { CPU::opcodeCB0x61(); }, 2,  8,  8),
            INSTRUCTION("BIT 4,D",          [this] { CPU::opcodeCB0x62(); }, 2,  8,  8),
            INSTRUCTION("BIT 4,E",          [this] { CPU::opcodeCB0x63(); }, 2,  8,  8),
            INSTRUCTION("BIT 4,H",          [this] { CPU::opcodeCB0x64(); }, 2,  8,  8),
            INSTRUCTION("BIT 4,L",          [this] { CPU::opcodeCB0x65(); }, 2,  8,  8),
            INSTRUCTION("BIT 4,(HL)",       [this] { CPU::opcodeCB0x66(); }, 2, 12, 12),
            INSTRUCTION("BIT 4,A",          [this] { CPU::opcodeCB0x67(); }, 2,  8,  8),
            INSTRUCTION("BIT 5,B",          [this] { CPU::opcodeCB0x68(); }, 2,  8,  8),
            INSTRUCTION("BIT 5,C",          [this] { CPU::opcodeCB0x69(); }, 2,  8,  8),
            INSTRUCTION("BIT 5,D",          [this] { CPU::opcodeCB0x6A(); }, 2,  8,  8),
            INSTRUCTION("BIT 5,E",          [this] { CPU::opcodeCB0x6B(); }, 2,  8,  8),
            INSTRUCTION("BIT 5,H",          [this] { CPU::opcodeCB0x6C(); }, 2,  8,  8),
            INSTRUCTION("BIT 5,L",          [this] { CPU::opcodeCB0x6D(); }, 2,  8,  8),
            INSTRUCTION("BIT 5,(HL)",       [this] { CPU::opcodeCB0x6E(); }, 2, 12, 12),
            INSTRUCTION("BIT 5,A",          [this] { CPU::opcodeCB0x6F(); }, 2,  8,  8),

            //0x70
            INSTRUCTION("BIT 6,B",          [this] { CPU::opcodeCB0x70(); }, 2,  8,  8),
            INSTRUCTION("BIT 6,C",          [this] { CPU::opcodeCB0x71(); }, 2,  8,  8),
            INSTRUCTION("BIT 6,D",          [this] { CPU::opcodeCB0x72(); }, 2,  8,  8),
            INSTRUCTION("BIT 6,E",          [this] { CPU::opcodeCB0x73(); }, 2,  8,  8),
            INSTRUCTION("BIT 6,H",          [this] { CPU::opcodeCB0x74(); }, 2,  8,  8),
            INSTRUCTION("BIT 6,L",          [this] { CPU::opcodeCB0x75(); }, 2,  8,  8),
            INSTRUCTION("BIT 6,(HL)",       [this] { CPU::opcodeCB0x76(); }, 2, 12, 12),
            INSTRUCTION("BIT 6,A",          [this] { CPU::opcodeCB0x77(); }, 2,  8,  8),
            INSTRUCTION("BIT 7,B",          [this] { CPU::opcodeCB0x78(); }, 2,  8,  8),
            INSTRUCTION("BIT 7,C",          [this] { CPU::opcodeCB0x79(); }, 2,  8,  8),
            INSTRUCTION("BIT 7,D",          [this] { CPU::opcodeCB0x7A(); }, 2,  8,  8),
            INSTRUCTION("BIT 7,E",          [this] { CPU::opcodeCB0x7B(); }, 2,  8,  8),
            INSTRUCTION("BIT 7,H",          [this] { CPU::opcodeCB0x7C(); }, 2,  8,  8),
            INSTRUCTION("BIT 7,L",          [this] { CPU::opcodeCB0x7D(); }, 2,  8,  8),
            INSTRUCTION("BIT 7,(HL)",       [this] { CPU::opcodeCB0x7E(); }, 2, 12, 12),
            INSTRUCTION("BIT 7,A",          [this] { CPU::opcodeCB0x7F(); }, 2,  8,  8),

            //0x80
            INSTRUCTION("RES 0,B",          [this] { CPU::opcodeCB0x80(); }, 2,  8,  8),
            INSTRUCTION("RES 0,C",          [this] { CPU::opcodeCB0x81(); }, 2,  8,  8),
            INSTRUCTION("RES 0,D",          [this] { CPU::opcodeCB0x82(); }, 2,  8,  8),
            INSTRUCTION("RES 0,E",          [this] { CPU::opcodeCB0x83(); }, 2,  8,  8),
            INSTRUCTION("RES 0,H",          [this] { CPU::opcodeCB0x84(); }, 2,  8,  8),
            INSTRUCTION("RES 0,L",          [this] { CPU::opcodeCB0x85(); }, 2,  8,  8),
            INSTRUCTION("RES 0,(HL)",       [this] { CPU::opcodeCB0x86(); }, 2, 16, 16),
            INSTRUCTION("RES 0,A",          [this] { CPU::opcodeCB0x87(); }, 2,  8,  8),
            INSTRUCTION("RES 1,B",          [this] { CPU::opcodeCB0x88(); }, 2,  8,  8),
            INSTRUCTION("RES 1,C",          [this] { CPU::opcodeCB0x89(); }, 2,  8,  8),
            INSTRUCTION("RES 1,D",          [this] { CPU::opcodeCB0x8A(); }, 2,  8,  8),
            INSTRUCTION("RES 1,E",          [this] { CPU::opcodeCB0x8B(); }, 2,  8,  8),
            INSTRUCTION("RES 1,H",          [this] { CPU::opcodeCB0x8C(); }, 2,  8,  8),
            INSTRUCTION("RES 1,L",          [this] { CPU::opcodeCB0x8D(); }, 2,  8,  8),
            INSTRUCTION("RES 1,(HL)",       [this] { CPU::opcodeCB0x8E(); }, 2, 16, 16),
            INSTRUCTION("RES 1,A",          [this] { CPU::opcodeCB0x8F(); }, 2,  8,  8),

            //0x90
            INSTRUCTION("RES 2,B",          [this] { CPU::opcodeCB0x90(); }, 2,  8,  8),
            INSTRUCTION("RES 2,C",          [this] { CPU::opcodeCB0x91(); }, 2,  8,  8),
            INSTRUCTION("RES 2,D",          [this] { CPU::opcodeCB0x92(); }, 2,  8,  8),
            INSTRUCTION("RES 2,E",          [this] { CPU::opcodeCB0x93(); }, 2,  8,  8),
            INSTRUCTION("RES 2,H",          [this] { CPU::opcodeCB0x94(); }, 2,  8,  8),
            INSTRUCTION("RES 2,L",          [this] { CPU::opcodeCB0x95(); }, 2,  8,  8),
            INSTRUCTION("RES 2,(HL)",       [this] { CPU::opcodeCB0x96(); }, 2, 16, 16),
            INSTRUCTION("RES 2,A",          [this] { CPU::opcodeCB0x97(); }, 2,  8,  8),
            INSTRUCTION("RES 3,B",          [this] { CPU::opcodeCB0x98(); }, 2,  8,  8),
            INSTRUCTION("RES 3,C",          [this] { CPU::opcodeCB0x99(); }, 2,  8,  8),
            INSTRUCTION("RES 3,D",          [this] { CPU::opcodeCB0x9A(); }, 2,  8,  8),
            INSTRUCTION("RES 3,E",          [this] { CPU::opcodeCB0x9B(); }, 2,  8,  8),
            INSTRUCTION("RES 3,H",          [this] { CPU::opcodeCB0x9C(); }, 2,  8,  8),
            INSTRUCTION("RES 3,L",          [this] { CPU::opcodeCB0x9D(); }, 2,  8,  8),
            INSTRUCTION("RES 3,(HL)",       [this] { CPU::opcodeCB0x9E(); }, 2, 16, 16),
            INSTRUCTION("RES 3,A",          [this] { CPU::opcodeCB0x9F(); }, 2,  8,  8),

            //0xA0
            INSTRUCTION("RES 4,B",          [this] { CPU::opcodeCB0xA0(); }, 2,  8,  8),
            INSTRUCTION("RES 4,C",          [this] { CPU::opcodeCB0xA1(); }, 2,  8,  8),
            INSTRUCTION("RES 4,D",          [this] { CPU::opcodeCB0xA2(); }, 2,  8,  8),
            INSTRUCTION("RES 4,E",          [this] { CPU::opcodeCB0xA3(); }, 2,  8,  8),
            INSTRUCTION("RES 4,H",          [this] { CPU::opcodeCB0xA4(); }, 2,  8,  8),
            INSTRUCTION("RES 4,L",          [this] { CPU::opcodeCB0xA5(); }, 2,  8,  8),
            INSTRUCTION("RES 4,(HL)",       [this] { CPU::opcodeCB0xA6(); }, 2, 16, 16),
            INSTRUCTION("RES 4,A",          [this] { CPU::opcodeCB0xA7(); }, 2,  8,  8),
            INSTRUCTION("RES 5,B",          [this] { CPU::opcodeCB0xA8(); }, 2,  8,  8),
            INSTRUCTION("RES 5,C",          [this] { CPU::opcodeCB0xA9(); }, 2,  8,  8),
            INSTRUCTION("RES 5,D",          [this] { CPU::opcodeCB0xAA(); }, 2,  8,  8),
            INSTRUCTION("RES 5,E",          [this] { CPU::opcodeCB0xAB(); }, 2,  8,  8),
            INSTRUCTION("RES 5,H",          [this] { CPU::opcodeCB0xAC(); }, 2,  8,  8),
            INSTRUCTION("RES 5,L",          [this] { CPU::opcodeCB0xAD(); }, 2,  8,  8),
            INSTRUCTION("RES 5,(HL)",       [this] { CPU::opcodeCB0xAE(); }, 2, 16, 16),
            INSTRUCTION("RES 5,A",          [this] { CPU::opcodeCB0xAF(); }, 2,  8,  8),

            //0xB0
            INSTRUCTION("RES 6,B",          [this] { CPU::opcodeCB0xB0(); }, 2,  8,  8),
            INSTRUCTION("RES 6,C",          [this] { CPU::opcodeCB0xB1(); }, 2,  8,  8),
            INSTRUCTION("RES 6,D",          [this] { CPU::opcodeCB0xB2(); }, 2,  8,  8),
            INSTRUCTION("RES 6,E",          [this] { CPU::opcodeCB0xB3(); }, 2,  8,  8),
            INSTRUCTION("RES 6,H",          [this] { CPU::opcodeCB0xB4(); }, 2,  8,  8),
            INSTRUCTION("RES 6,L",          [this] { CPU::opcodeCB0xB5(); }, 2,  8,  8),
            INSTRUCTION("RES 6,(HL)",       [this] { CPU::opcodeCB0xB6(); }, 2, 16, 16),
            INSTRUCTION("RES 6,A",          [this] { CPU::opcodeCB0xB7(); }, 2,  8,  8),
            INSTRUCTION("RES 7,B",          [this] { CPU::opcodeCB0xB8(); }, 2,  8,  8),
            INSTRUCTION("RES 7,C",          [this] { CPU::opcodeCB0xB9(); }, 2,  8,  8),
            INSTRUCTION("RES 7,D",          [this] { CPU::opcodeCB0xBA(); }, 2,  8,  8),
            INSTRUCTION("RES 7,E",          [this] { CPU::opcodeCB0xBB(); }, 2,  8,  8),
            INSTRUCTION("RES 7,H",          [this] { CPU::opcodeCB0xBC(); }, 2,  8,  8),
            INSTRUCTION("RES 7,L",          [this] { CPU::opcodeCB0xBD(); }, 2,  8,  8),
            INSTRUCTION("RES 7,(HL)",       [this] { CPU::opcodeCB0xBE(); }, 2, 16, 16),
            INSTRUCTION("RES 7,A",          [this] { CPU::opcodeCB0xBF(); }, 2,  8,  8),

            //0xC0
            INSTRUCTION("SET 0,B",          [this] { CPU::opcodeCB0xC0(); }, 2,  8,  8),
            INSTRUCTION("SET 0,C",          [this] { CPU::opcodeCB0xC1(); }, 2,  8,  8),
            INSTRUCTION("SET 0,D",          [this] { CPU::opcodeCB0xC2(); }, 2,  8,  8),
            INSTRUCTION("SET 0,E",          [this] { CPU::opcodeCB0xC3(); }, 2,  8,  8),
            INSTRUCTION("SET 0,H",          [this] { CPU::opcodeCB0xC4(); }, 2,  8,  8),
            INSTRUCTION("SET 0,L",          [this] { CPU::opcodeCB0xC5(); }, 2,  8,  8),
            INSTRUCTION("SET 0,(HL)",       [this] { CPU::opcodeCB0xC6(); }, 2, 16, 16),
            INSTRUCTION("SET 0,A",          [this] { CPU::opcodeCB0xC7(); }, 2,  8,  8),
            INSTRUCTION("SET 1,B",          [this] { CPU::opcodeCB0xC8(); }, 2,  8,  8),
            INSTRUCTION("SET 1,C",          [this] { CPU::opcodeCB0xC9(); }, 2,  8,  8),
            INSTRUCTION("SET 1,D",          [this] { CPU::opcodeCB0xCA(); }, 2,  8,  8),
            INSTRUCTION("SET 1,E",          [this] { CPU::opcodeCB0xCB(); }, 2,  8,  8),
            INSTRUCTION("SET 1,H",          [this] { CPU::opcodeCB0xCC(); }, 2,  8,  8),
            INSTRUCTION("SET 1,L",          [this] { CPU::opcodeCB0xCD(); }, 2,  8,  8),
            INSTRUCTION("SET 1,(HL)",       [this] { CPU::opcodeCB0xCE(); }, 2, 16, 16),
            INSTRUCTION("SET 1,A",          [this] { CPU::opcodeCB0xCF(); }, 2,  8,  8),

            //0xD0
            INSTRUCTION("SET 2,B",          [this] { CPU::opcodeCB0xD0(); }, 2,  8,  8),
            INSTRUCTION("SET 2,C",          [this] { CPU::opcodeCB0xD1(); }, 2,  8,  8),
            INSTRUCTION("SET 2,D",          [this] { CPU::opcodeCB0xD2(); }, 2,  8,  8),
            INSTRUCTION("SET 2,E",          [this] { CPU::opcodeCB0xD3(); }, 2,  8,  8),
            INSTRUCTION("SET 2,H",          [this] { CPU::opcodeCB0xD4(); }, 2,  8,  8),
            INSTRUCTION("SET 2,L",          [this] { CPU::opcodeCB0xD5(); }, 2,  8,  8),
            INSTRUCTION("SET 2,(HL)",       [this] { CPU::opcodeCB0xD6(); }, 2, 16, 16),
            INSTRUCTION("SET 2,A",          [this] { CPU::opcodeCB0xD7(); }, 2,  8,  8),
            INSTRUCTION("SET 3,B",          [this] { CPU::opcodeCB0xD8(); }, 2,  8,  8),
            INSTRUCTION("SET 3,C",          [this] { CPU::opcodeCB0xD9(); }, 2,  8,  8),
            INSTRUCTION("SET 3,D",          [this] { CPU::opcodeCB0xDA(); }, 2,  8,  8),
            INSTRUCTION("SET 3,E",          [this] { CPU::opcodeCB0xDB(); }, 2,  8,  8),
            INSTRUCTION("SET 3,H",          [this] { CPU::opcodeCB0xDC(); }, 2,  8,  8),
            INSTRUCTION("SET 3,L",          [this] { CPU::opcodeCB0xDD(); }, 2,  8,  8),
            INSTRUCTION("SET 3,(HL)",       [this] { CPU::opcodeCB0xDE(); }, 2, 16, 16),
            INSTRUCTION("SET 3,A",          [this] { CPU::opcodeCB0xDF(); }, 2,  8,  8),

            //0xE0
            INSTRUCTION("SET 4,B",          [this] { CPU::opcodeCB0xE0(); }, 2,  8,  8),
            INSTRUCTION("SET 4,C",          [this] { CPU::opcodeCB0xE1(); }, 2,  8,  8),
            INSTRUCTION("SET 4,D",          [this] { CPU::opcodeCB0xE2(); }, 2,  8,  8),
            INSTRUCTION("SET 4,E",          [this] { CPU::opcodeCB0xE3(); }, 2,  8,  8),
            INSTRUCTION("SET 4,H",          [this] { CPU::opcodeCB0xE4(); }, 2,  8,  8),
            INSTRUCTION("SET 4,L",          [this] { CPU::opcodeCB0xE5(); }, 2,  8,  8),
            INSTRUCTION("SET 4,(HL)",       [this] { CPU::opcodeCB0xE6(); }, 2, 16, 16),
            INSTRUCTION("SET 4,A",          [this] { CPU::opcodeCB0xE7(); }, 2,  8,  8),
            INSTRUCTION("SET 5,B",          [this] { CPU::opcodeCB0xE8(); }, 2,  8,  8),
            INSTRUCTION("SET 5,C",          [this] { CPU::opcodeCB0xE9(); }, 2,  8,  8),
            INSTRUCTION("SET 5,D",          [this] { CPU::opcodeCB0xEA(); }, 2,  8,  8),
            INSTRUCTION("SET 5,E",          [this] { CPU::opcodeCB0xEB(); }, 2,  8,  8),
            INSTRUCTION("SET 5,H",          [this] { CPU::opcodeCB0xEC(); }, 2,  8,  8),
            INSTRUCTION("SET 5,L",          [this] { CPU::opcodeCB0xED(); }, 2,  8,  8),
            INSTRUCTION("SET 5,(HL)",       [this] { CPU::opcodeCB0xEE(); }, 2, 16, 16),
            INSTRUCTION("SET 5,A",          [this] { CPU::opcodeCB0xEF(); }, 2,  8,  8),

            //0xF0
            INSTRUCTION("SET 6,B",          [this] { CPU::opcodeCB0xF0(); }, 2,  8,  8),
            INSTRUCTION("SET 6,C",          [this] { CPU::opcodeCB0xF1(); }, 2,  8,  8),
            INSTRUCTION("SET 6,D",          [this] { CPU::opcodeCB0xF2(); }, 2,  8,  8),
            INSTRUCTION("SET 6,E",          [this] { CPU::opcodeCB0xF3(); }, 2,  8,  8),
            INSTRUCTION("SET 6,H",          [this] { CPU::opcodeCB0xF4(); }, 2,  8,  8),
            INSTRUCTION("SET 6,L",          [this] { CPU::opcodeCB0xF5(); }, 2,  8,  8),
            INSTRUCTION("SET 6,(HL)",       [this] { CPU::opcodeCB0xF6(); }, 2, 16, 16),
            INSTRUCTION("SET 6,A",          [this] { CPU::opcodeCB0xF7(); }, 2,  8,  8),
            INSTRUCTION("SET 7,B",          [this] { CPU::opcodeCB0xF8(); }, 2,  8,  8),
            INSTRUCTION("SET 7,C",          [this] { CPU::opcodeCB0xF9(); }, 2,  8,  8),
            INSTRUCTION("SET 7,D",          [this] { CPU::opcodeCB0xFA(); }, 2,  8,  8),
            INSTRUCTION("SET 7,E",          [this] { CPU::opcodeCB0xFB(); }, 2,  8,  8),
            INSTRUCTION("SET 7,H",          [this] { CPU::opcodeCB0xFC(); }, 2,  8,  8),
            INSTRUCTION("SET 7,L",          [this] { CPU::opcodeCB0xFD(); }, 2,  8,  8),
            INSTRUCTION("SET 7,(HL)",       [this] { CPU::opcodeCB0xFE(); }, 2, 16, 16),
            INSTRUCTION("SET 7,A",          [this] { CPU::opcodeCB0xFF(); }, 2,  8,  8)
        }};
};
