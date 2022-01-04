#pragma once

#include "core.h"

#include "instruction.h"
#include "flags.h"

class MMU;

class CPU
{
    public:
        CPU();

        u8 tick();

        inline void setMMU(MMU* mmu) { m_MMU = mmu; }

        inline bool getIME() const   { return m_IME; }
        inline bool setIME(bool ime) { return m_IME = ime; }

        void raiseInterrupt(const Flags::Interrupt& flag);

    private:
        void handleInterrupts();

        void reset();

        inline void setFlag(const Flags::Register& flag)    { m_Registers.F |= flag; }
        inline void clearFlag(const Flags::Register& flag)  { m_Registers.F &= ~flag; }
        inline void flipFlag(const Flags::Register& flag)   { m_Registers.F ^= flag; }
        inline bool isFlagSet(const Flags::Register& flag) const  { return m_Registers.F & flag; }

        inline void clearAllFlags()                         { clearFlag(Flags::Register::Zero | Flags::Register::Negative | Flags::Register::HalfCarry | Flags::Register::Carry); }
        inline void setZeroFromVal(const u8& val)           { if(!val) setFlag(Flags::Register::Zero); }

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
        
        MMU* m_MMU;

        Registers m_Registers;

        bool m_Halted;
        bool m_IME;
        bool m_Branched;

    private:
        //--------------------------------------Opcode Helpers--------------------------------------//

        void pushStack(const u16& val);
        void popStack(u16& reg);

        void opcodeINC(u8& reg);
        void opcodeDEC(u8& reg);
    

        void opcodeADD(const u8& val);
        void opcodeADC(const u8& val);
        void opcodeSUB(const u8& val);
        void opcodeSBC(const u8& val);
        void opcodeAND(const u8& val);
        void opcodeXOR(const u8& val);
        void opcodeOR(const u8& val);
        void opcodeCP(const u8& val);

        void opcodeJP(bool condition);
        void opcodeJR(bool condition);

        void opcodeCALL(bool condition);
        void opcodeRET(bool condiiton);

        void opcodeRST(const u8& val);

        // 16 bit variants

        void opcodeADD_HL(const u16& val);
        u16  opcodeADD_SP();

        //--------------------------------------CB Opcode Helpers--------------------------------------//

        void opcodeRLC(u8& reg);
        void opcodeRRC(u8& reg);

        void opcodeRL(u8& reg);
        void opcodeRR(u8& reg);

        void opcodeSLA(u8& reg);
        void opcodeSRA(u8& reg);

        void opcodeSWAP(u8& reg);
        void opcodeSRL(u8& reg);

        void opcodeBIT(const u8& bit, u8& reg);
        void opcodeRES(const u8& bit, u8& reg);
        void opcodeSET(const u8& bit, u8& reg);

        // 16 bit variants

        void opcodeBIT_HL(const u8& bit);
        void opcodeRES_HL(const u8& bit);
        void opcodeSET_HL(const u8& bit);

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

        const Instruction instructions[0x100] = 
        {
            //0x00
            INSTRUCTION("NOP",              std::bind(&CPU::opcode0x00, this), 1,  4,  4),
            INSTRUCTION("LD BC,u16",        std::bind(&CPU::opcode0x01, this), 3, 12, 12),
            INSTRUCTION("LD (BC),A",        std::bind(&CPU::opcode0x02, this), 1,  8,  8),
            INSTRUCTION("INC BC",           std::bind(&CPU::opcode0x03, this), 1,  8,  8),
            INSTRUCTION("INC B",            std::bind(&CPU::opcode0x04, this), 1,  4,  4),
            INSTRUCTION("DEC B",            std::bind(&CPU::opcode0x05, this), 1,  4,  4),
            INSTRUCTION("LD B,u8",          std::bind(&CPU::opcode0x06, this), 2,  8,  8),
            INSTRUCTION("RLCA",             std::bind(&CPU::opcode0x07, this), 1,  4,  4),
            INSTRUCTION("LD (u16),SP",      std::bind(&CPU::opcode0x08, this), 3, 20, 20),
            INSTRUCTION("ADD HL,BC",        std::bind(&CPU::opcode0x09, this), 1,  8,  8),
            INSTRUCTION("LD A,(BC)",        std::bind(&CPU::opcode0x0A, this), 1,  8,  8),
            INSTRUCTION("DEC BC",           std::bind(&CPU::opcode0x0B, this), 1,  8,  8),
            INSTRUCTION("INC C",            std::bind(&CPU::opcode0x0C, this), 1,  4,  4),
            INSTRUCTION("DEC C",            std::bind(&CPU::opcode0x0D, this), 1,  4,  4),
            INSTRUCTION("LD C,u8",          std::bind(&CPU::opcode0x0E, this), 2,  8,  8),
            INSTRUCTION("RRCA",             std::bind(&CPU::opcode0x0F, this), 1,  4,  4),

            //0x10
            INSTRUCTION("STOP",             std::bind(&CPU::opcode0x10, this), 2,  4,  4),
            INSTRUCTION("LD DE,u16",        std::bind(&CPU::opcode0x11, this), 3, 12, 12),
            INSTRUCTION("LD (DE),A",        std::bind(&CPU::opcode0x12, this), 1,  8,  8),
            INSTRUCTION("INC DE",           std::bind(&CPU::opcode0x13, this), 1,  8,  8),
            INSTRUCTION("INC D",            std::bind(&CPU::opcode0x14, this), 1,  4,  4),
            INSTRUCTION("DEC D",            std::bind(&CPU::opcode0x15, this), 1,  4,  4),
            INSTRUCTION("LD D,u8",          std::bind(&CPU::opcode0x16, this), 2,  8,  8),
            INSTRUCTION("RLA",              std::bind(&CPU::opcode0x17, this), 1,  4,  4),
            INSTRUCTION("JR i8",            std::bind(&CPU::opcode0x18, this), 2, 12, 12),
            INSTRUCTION("ADD HL,DE",        std::bind(&CPU::opcode0x19, this), 1,  8,  8),
            INSTRUCTION("LD A,(DE)",        std::bind(&CPU::opcode0x1A, this), 1,  8,  8),
            INSTRUCTION("DEC DE",           std::bind(&CPU::opcode0x1B, this), 1,  8,  8),
            INSTRUCTION("INC E",            std::bind(&CPU::opcode0x1C, this), 1,  4,  4),
            INSTRUCTION("DEC E",            std::bind(&CPU::opcode0x1D, this), 1,  4,  4),
            INSTRUCTION("LD E,u8",          std::bind(&CPU::opcode0x1E, this), 2,  8,  8),
            INSTRUCTION("RRA",              std::bind(&CPU::opcode0x1F, this), 1,  4,  4),

            //0x20
            INSTRUCTION("JR NZ,i8",         std::bind(&CPU::opcode0x20, this), 2, 12,  8),
            INSTRUCTION("LD HL,u16",        std::bind(&CPU::opcode0x21, this), 3, 12, 12),
            INSTRUCTION("LD (HL+),A",       std::bind(&CPU::opcode0x22, this), 1,  8,  8),
            INSTRUCTION("INC HL",           std::bind(&CPU::opcode0x23, this), 1,  8,  8),
            INSTRUCTION("INC H",            std::bind(&CPU::opcode0x24, this), 1,  4,  4),
            INSTRUCTION("DEC H",            std::bind(&CPU::opcode0x25, this), 1,  4,  4),
            INSTRUCTION("LD H,u8",          std::bind(&CPU::opcode0x26, this), 2,  8,  8),
            INSTRUCTION("DAA",              std::bind(&CPU::opcode0x27, this), 1,  4,  4),
            INSTRUCTION("JR Z,i8",          std::bind(&CPU::opcode0x28, this), 2, 12,  8),
            INSTRUCTION("ADD HL,HL",        std::bind(&CPU::opcode0x29, this), 1,  8,  8),
            INSTRUCTION("LD A,(HL+)",       std::bind(&CPU::opcode0x2A, this), 1,  8,  8),
            INSTRUCTION("DEC HL",           std::bind(&CPU::opcode0x2B, this), 1,  8,  8),
            INSTRUCTION("INC L",            std::bind(&CPU::opcode0x2C, this), 1,  4,  4),
            INSTRUCTION("DEC L",            std::bind(&CPU::opcode0x2D, this), 1,  4,  4),
            INSTRUCTION("LD L,u8",          std::bind(&CPU::opcode0x2E, this), 2,  8,  8),
            INSTRUCTION("CPL",              std::bind(&CPU::opcode0x2F, this), 1,  4,  4),

            //0x30
            INSTRUCTION("JR NC,i8",         std::bind(&CPU::opcode0x30, this), 2, 12,  8),
            INSTRUCTION("LD SP,u16",        std::bind(&CPU::opcode0x31, this), 3, 12, 12),
            INSTRUCTION("LD (HL-),A",       std::bind(&CPU::opcode0x32, this), 1,  8,  8),
            INSTRUCTION("INC SP",           std::bind(&CPU::opcode0x33, this), 1,  8,  8),
            INSTRUCTION("INC (HL)",         std::bind(&CPU::opcode0x34, this), 1, 12, 12),
            INSTRUCTION("DEC (HL)",         std::bind(&CPU::opcode0x35, this), 1, 12, 12),
            INSTRUCTION("LD (HL),u8",       std::bind(&CPU::opcode0x36, this), 2, 12, 12),
            INSTRUCTION("SCF",              std::bind(&CPU::opcode0x37, this), 1,  4,  4),
            INSTRUCTION("JR C,i8",          std::bind(&CPU::opcode0x38, this), 2, 12,  8),
            INSTRUCTION("ADD HL,SP",        std::bind(&CPU::opcode0x39, this), 1,  8,  8),
            INSTRUCTION("LD A,(HL-)",       std::bind(&CPU::opcode0x3A, this), 1,  8,  8),
            INSTRUCTION("DEC SP",           std::bind(&CPU::opcode0x3B, this), 1,  8,  8),
            INSTRUCTION("INC A",            std::bind(&CPU::opcode0x3C, this), 1,  4,  4),
            INSTRUCTION("DEC A",            std::bind(&CPU::opcode0x3D, this), 1,  4,  4),
            INSTRUCTION("LD A,u8",          std::bind(&CPU::opcode0x3E, this), 2,  8,  8),
            INSTRUCTION("CCF",              std::bind(&CPU::opcode0x3F, this), 1,  4,  4),

            //0x40
            INSTRUCTION("LD B,B",           std::bind(&CPU::opcode0x40, this), 1,  4,  4),
            INSTRUCTION("LD B,C",           std::bind(&CPU::opcode0x41, this), 1,  4,  4),
            INSTRUCTION("LD B,D",           std::bind(&CPU::opcode0x42, this), 1,  4,  4),
            INSTRUCTION("LD B,E",           std::bind(&CPU::opcode0x43, this), 1,  4,  4),
            INSTRUCTION("LD B,H",           std::bind(&CPU::opcode0x44, this), 1,  4,  4),
            INSTRUCTION("LD B,L",           std::bind(&CPU::opcode0x45, this), 1,  4,  4),
            INSTRUCTION("LD B,(HL)",        std::bind(&CPU::opcode0x46, this), 1,  8,  8),
            INSTRUCTION("LD B,A",           std::bind(&CPU::opcode0x47, this), 1,  4,  4),
            INSTRUCTION("LD C,B",           std::bind(&CPU::opcode0x48, this), 1,  4,  4),
            INSTRUCTION("LD C,C",           std::bind(&CPU::opcode0x49, this), 1,  4,  4),
            INSTRUCTION("LD C,D",           std::bind(&CPU::opcode0x4A, this), 1,  4,  4),
            INSTRUCTION("LD C,E",           std::bind(&CPU::opcode0x4B, this), 1,  4,  4),
            INSTRUCTION("LD C,H",           std::bind(&CPU::opcode0x4C, this), 1,  4,  4),
            INSTRUCTION("LD C,L",           std::bind(&CPU::opcode0x4D, this), 1,  4,  4),
            INSTRUCTION("LD C,(HL)",        std::bind(&CPU::opcode0x4E, this), 1,  8,  8),
            INSTRUCTION("LD C,A",           std::bind(&CPU::opcode0x4F, this), 1,  4,  4),

            //0x50
            INSTRUCTION("LD D,B",           std::bind(&CPU::opcode0x50, this), 1,  4,  4),
            INSTRUCTION("LD D,C",           std::bind(&CPU::opcode0x51, this), 1,  4,  4),
            INSTRUCTION("LD D,D",           std::bind(&CPU::opcode0x52, this), 1,  4,  4),
            INSTRUCTION("LD D,E",           std::bind(&CPU::opcode0x53, this), 1,  4,  4),
            INSTRUCTION("LD D,H",           std::bind(&CPU::opcode0x54, this), 1,  4,  4),
            INSTRUCTION("LD D,L",           std::bind(&CPU::opcode0x55, this), 1,  4,  4),
            INSTRUCTION("LD D,(HL)",        std::bind(&CPU::opcode0x56, this), 1,  8,  8),
            INSTRUCTION("LD D,A",           std::bind(&CPU::opcode0x57, this), 1,  4,  4),
            INSTRUCTION("LD E,B",           std::bind(&CPU::opcode0x58, this), 1,  4,  4),
            INSTRUCTION("LD E,C",           std::bind(&CPU::opcode0x59, this), 1,  4,  4),
            INSTRUCTION("LD E,D",           std::bind(&CPU::opcode0x5A, this), 1,  4,  4),
            INSTRUCTION("LD E,E",           std::bind(&CPU::opcode0x5B, this), 1,  4,  4),
            INSTRUCTION("LD E,H",           std::bind(&CPU::opcode0x5C, this), 1,  4,  4),
            INSTRUCTION("LD E,L",           std::bind(&CPU::opcode0x5D, this), 1,  4,  4),
            INSTRUCTION("LD E,(HL)",        std::bind(&CPU::opcode0x5E, this), 1,  8,  8),
            INSTRUCTION("LD E,A",           std::bind(&CPU::opcode0x5F, this), 1,  4,  4),

            //0x60
            INSTRUCTION("LD H,B",           std::bind(&CPU::opcode0x60, this), 1,  4,  4),
            INSTRUCTION("LD H,C",           std::bind(&CPU::opcode0x61, this), 1,  4,  4),
            INSTRUCTION("LD H,D",           std::bind(&CPU::opcode0x62, this), 1,  4,  4),
            INSTRUCTION("LD H,E",           std::bind(&CPU::opcode0x63, this), 1,  4,  4),
            INSTRUCTION("LD H,H",           std::bind(&CPU::opcode0x64, this), 1,  4,  4),
            INSTRUCTION("LD H,L",           std::bind(&CPU::opcode0x65, this), 1,  4,  4),
            INSTRUCTION("LD H,(HL)",        std::bind(&CPU::opcode0x66, this), 1,  8,  8),
            INSTRUCTION("LD H,A",           std::bind(&CPU::opcode0x67, this), 1,  4,  4),
            INSTRUCTION("LD L,B",           std::bind(&CPU::opcode0x68, this), 1,  4,  4),
            INSTRUCTION("LD L,C",           std::bind(&CPU::opcode0x69, this), 1,  4,  4),
            INSTRUCTION("LD L,D",           std::bind(&CPU::opcode0x6A, this), 1,  4,  4),
            INSTRUCTION("LD L,E",           std::bind(&CPU::opcode0x6B, this), 1,  4,  4),
            INSTRUCTION("LD L,H",           std::bind(&CPU::opcode0x6C, this), 1,  4,  4),
            INSTRUCTION("LD L,L",           std::bind(&CPU::opcode0x6D, this), 1,  4,  4),
            INSTRUCTION("LD L,(HL)",        std::bind(&CPU::opcode0x6E, this), 1,  8,  8),
            INSTRUCTION("LD L,A",           std::bind(&CPU::opcode0x6F, this), 1,  4,  4),

            //0x70
            INSTRUCTION("LD (HL),B",        std::bind(&CPU::opcode0x70, this), 1,  8,  8),
            INSTRUCTION("LD (HL),C",        std::bind(&CPU::opcode0x71, this), 1,  8,  8),
            INSTRUCTION("LD (HL),D",        std::bind(&CPU::opcode0x72, this), 1,  8,  8),
            INSTRUCTION("LD (HL),E",        std::bind(&CPU::opcode0x73, this), 1,  8,  8),
            INSTRUCTION("LD (HL),H",        std::bind(&CPU::opcode0x74, this), 1,  8,  8),
            INSTRUCTION("LD (HL),L",        std::bind(&CPU::opcode0x75, this), 1,  8,  8),
            INSTRUCTION("HALT",             std::bind(&CPU::opcode0x76, this), 1,  4,  4),
            INSTRUCTION("LD (HL),A",        std::bind(&CPU::opcode0x77, this), 1,  8,  8),
            INSTRUCTION("LD A,B",           std::bind(&CPU::opcode0x78, this), 1,  4,  4),
            INSTRUCTION("LD A,C",           std::bind(&CPU::opcode0x79, this), 1,  4,  4),
            INSTRUCTION("LD A,D",           std::bind(&CPU::opcode0x7A, this), 1,  4,  4),
            INSTRUCTION("LD A,E",           std::bind(&CPU::opcode0x7B, this), 1,  4,  4),
            INSTRUCTION("LD A,H",           std::bind(&CPU::opcode0x7C, this), 1,  4,  4),
            INSTRUCTION("LD A,L",           std::bind(&CPU::opcode0x7D, this), 1,  4,  4),
            INSTRUCTION("LD A,(HL)",        std::bind(&CPU::opcode0x7E, this), 1,  8,  8),
            INSTRUCTION("LD A,A",           std::bind(&CPU::opcode0x7F, this), 1,  4,  4),

            //0x80  
            INSTRUCTION("ADD A,B",          std::bind(&CPU::opcode0x80, this), 1,  4,  4),
            INSTRUCTION("ADD A,C",          std::bind(&CPU::opcode0x81, this), 1,  4,  4),
            INSTRUCTION("ADD A,D",          std::bind(&CPU::opcode0x82, this), 1,  4,  4),
            INSTRUCTION("ADD A,E",          std::bind(&CPU::opcode0x83, this), 1,  4,  4),
            INSTRUCTION("ADD A,H",          std::bind(&CPU::opcode0x84, this), 1,  4,  4),
            INSTRUCTION("ADD A,L",          std::bind(&CPU::opcode0x85, this), 1,  4,  4),
            INSTRUCTION("ADD A,(HL)",       std::bind(&CPU::opcode0x86, this), 1,  8,  8),
            INSTRUCTION("ADD A,A",          std::bind(&CPU::opcode0x87, this), 1,  4,  4),
            INSTRUCTION("ADC A,B",          std::bind(&CPU::opcode0x88, this), 1,  4,  4),
            INSTRUCTION("ADC A,C",          std::bind(&CPU::opcode0x89, this), 1,  4,  4),
            INSTRUCTION("ADC A,D",          std::bind(&CPU::opcode0x8A, this), 1,  4,  4),
            INSTRUCTION("ADC A,E",          std::bind(&CPU::opcode0x8B, this), 1,  4,  4),
            INSTRUCTION("ADC A,H",          std::bind(&CPU::opcode0x8C, this), 1,  4,  4),
            INSTRUCTION("ADC A,L",          std::bind(&CPU::opcode0x8D, this), 1,  4,  4),
            INSTRUCTION("ADC A,(HL)",       std::bind(&CPU::opcode0x8E, this), 1,  8,  8),
            INSTRUCTION("ADC A,A",          std::bind(&CPU::opcode0x8F, this), 1,  4,  4),

            //0x90
            INSTRUCTION("SUB A,B",          std::bind(&CPU::opcode0x90, this), 1,  4,  4),
            INSTRUCTION("SUB A,C",          std::bind(&CPU::opcode0x91, this), 1,  4,  4),
            INSTRUCTION("SUB A,D",          std::bind(&CPU::opcode0x92, this), 1,  4,  4),
            INSTRUCTION("SUB A,E",          std::bind(&CPU::opcode0x93, this), 1,  4,  4),
            INSTRUCTION("SUB A,H",          std::bind(&CPU::opcode0x94, this), 1,  4,  4),
            INSTRUCTION("SUB A,L",          std::bind(&CPU::opcode0x95, this), 1,  4,  4),
            INSTRUCTION("SUB A,(HL)",       std::bind(&CPU::opcode0x96, this), 1,  8,  8),
            INSTRUCTION("SUB A,A",          std::bind(&CPU::opcode0x97, this), 1,  4,  4),
            INSTRUCTION("SBC A,B",          std::bind(&CPU::opcode0x98, this), 1,  4,  4),
            INSTRUCTION("SBC A,C",          std::bind(&CPU::opcode0x99, this), 1,  4,  4),
            INSTRUCTION("SBC A,D",          std::bind(&CPU::opcode0x9A, this), 1,  4,  4),
            INSTRUCTION("SBC A,E",          std::bind(&CPU::opcode0x9B, this), 1,  4,  4),
            INSTRUCTION("SBC A,H",          std::bind(&CPU::opcode0x9C, this), 1,  4,  4),
            INSTRUCTION("SBC A,L",          std::bind(&CPU::opcode0x9D, this), 1,  4,  4),
            INSTRUCTION("SBC A,(HL)",       std::bind(&CPU::opcode0x9E, this), 1,  8,  8),
            INSTRUCTION("SBC A,A",          std::bind(&CPU::opcode0x9F, this), 1,  4,  4),

            //0xA0
            INSTRUCTION("AND A,B",          std::bind(&CPU::opcode0xA0, this), 1,  4,  4),
            INSTRUCTION("AND A,C",          std::bind(&CPU::opcode0xA1, this), 1,  4,  4),
            INSTRUCTION("AND A,D",          std::bind(&CPU::opcode0xA2, this), 1,  4,  4),
            INSTRUCTION("AND A,E",          std::bind(&CPU::opcode0xA3, this), 1,  4,  4),
            INSTRUCTION("AND A,H",          std::bind(&CPU::opcode0xA4, this), 1,  4,  4),
            INSTRUCTION("AND A,L",          std::bind(&CPU::opcode0xA5, this), 1,  4,  4),
            INSTRUCTION("AND A,(HL)",       std::bind(&CPU::opcode0xA6, this), 1,  8,  8),
            INSTRUCTION("AND A,A",          std::bind(&CPU::opcode0xA7, this), 1,  4,  4),
            INSTRUCTION("XOR A,B",          std::bind(&CPU::opcode0xA8, this), 1,  4,  4),
            INSTRUCTION("XOR A,C",          std::bind(&CPU::opcode0xA9, this), 1,  4,  4),
            INSTRUCTION("XOR A,D",          std::bind(&CPU::opcode0xAA, this), 1,  4,  4),
            INSTRUCTION("XOR A,E",          std::bind(&CPU::opcode0xAB, this), 1,  4,  4),
            INSTRUCTION("XOR A,H",          std::bind(&CPU::opcode0xAC, this), 1,  4,  4),
            INSTRUCTION("XOR A,L",          std::bind(&CPU::opcode0xAD, this), 1,  4,  4),
            INSTRUCTION("XOR A,(HL)",       std::bind(&CPU::opcode0xAE, this), 1,  8,  8),
            INSTRUCTION("XOR A,A",          std::bind(&CPU::opcode0xAF, this), 1,  4,  4),

            //0xB0
            INSTRUCTION("OR A,B",           std::bind(&CPU::opcode0xB0, this), 1,  4,  4),
            INSTRUCTION("OR A,C",           std::bind(&CPU::opcode0xB1, this), 1,  4,  4),
            INSTRUCTION("OR A,D",           std::bind(&CPU::opcode0xB2, this), 1,  4,  4),
            INSTRUCTION("OR A,E",           std::bind(&CPU::opcode0xB3, this), 1,  4,  4),
            INSTRUCTION("OR A,H",           std::bind(&CPU::opcode0xB4, this), 1,  4,  4),
            INSTRUCTION("OR A,L",           std::bind(&CPU::opcode0xB5, this), 1,  4,  4),
            INSTRUCTION("OR A,(HL)",        std::bind(&CPU::opcode0xB6, this), 1,  8,  8),
            INSTRUCTION("OR A,A",           std::bind(&CPU::opcode0xB7, this), 1,  4,  4),
            INSTRUCTION("CP A,B",           std::bind(&CPU::opcode0xB8, this), 1,  4,  4),
            INSTRUCTION("CP A,C",           std::bind(&CPU::opcode0xB9, this), 1,  4,  4),
            INSTRUCTION("CP A,D",           std::bind(&CPU::opcode0xBA, this), 1,  4,  4),
            INSTRUCTION("CP A,E",           std::bind(&CPU::opcode0xBB, this), 1,  4,  4),
            INSTRUCTION("CP A,H",           std::bind(&CPU::opcode0xBC, this), 1,  4,  4),
            INSTRUCTION("CP A,L",           std::bind(&CPU::opcode0xBD, this), 1,  4,  4),
            INSTRUCTION("CP A,(HL)",        std::bind(&CPU::opcode0xBE, this), 1,  8,  8),
            INSTRUCTION("CP A,A",           std::bind(&CPU::opcode0xBF, this), 1,  4,  4),

            //0xC0
            INSTRUCTION("RET NZ",           std::bind(&CPU::opcode0xC0, this), 1, 20,  8),
            INSTRUCTION("POP BC",           std::bind(&CPU::opcode0xC1, this), 1, 12, 12),
            INSTRUCTION("JP NZ,u16",        std::bind(&CPU::opcode0xC2, this), 3, 16, 12),
            INSTRUCTION("JP u16",           std::bind(&CPU::opcode0xC3, this), 3, 16, 16),
            INSTRUCTION("CALL NZ,u16",      std::bind(&CPU::opcode0xC4, this), 3, 24, 12),
            INSTRUCTION("PUSH BC",          std::bind(&CPU::opcode0xC5, this), 1, 16, 16),
            INSTRUCTION("ADD A,u8",         std::bind(&CPU::opcode0xC6, this), 2,  8,  8),
            INSTRUCTION("RST 00h",          std::bind(&CPU::opcode0xC7, this), 1, 16, 16),
            INSTRUCTION("RET Z",            std::bind(&CPU::opcode0xC8, this), 1, 20,  8),
            INSTRUCTION("RET",              std::bind(&CPU::opcode0xC9, this), 1, 16, 16),
            INSTRUCTION("JP Z,u16",         std::bind(&CPU::opcode0xCA, this), 3, 16, 12),
            INSTRUCTION("PREFIX CB",        NULL, 1,  4,  4),
            INSTRUCTION("CALL Z,u16",       std::bind(&CPU::opcode0xCC, this), 3, 24, 12),
            INSTRUCTION("CALL u16",         std::bind(&CPU::opcode0xCD, this), 3, 24, 24),
            INSTRUCTION("ADC A,u8",         std::bind(&CPU::opcode0xCE, this), 2,  8,  8),
            INSTRUCTION("RST 08h",          std::bind(&CPU::opcode0xCF, this), 1, 16, 16),

            //0xD0
            INSTRUCTION("RET NC",           std::bind(&CPU::opcode0xD0, this), 1, 20,  8),
            INSTRUCTION("POP DE",           std::bind(&CPU::opcode0xD1, this), 1, 12, 12),
            INSTRUCTION("JP NC,u16",        std::bind(&CPU::opcode0xD2, this), 3, 16, 12),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("CALL NC,u16",      std::bind(&CPU::opcode0xD4, this), 3, 24, 12),
            INSTRUCTION("PUSH DE",          std::bind(&CPU::opcode0xD5, this), 1, 16, 16),
            INSTRUCTION("SUB A,u8",         std::bind(&CPU::opcode0xD6, this), 2,  8,  8),
            INSTRUCTION("RST 10h",          std::bind(&CPU::opcode0xD7, this), 1, 16, 16),
            INSTRUCTION("RET C",            std::bind(&CPU::opcode0xD8, this), 1, 20,  8),
            INSTRUCTION("RETI",             std::bind(&CPU::opcode0xD9, this), 1, 16, 16),
            INSTRUCTION("JP C,u16",         std::bind(&CPU::opcode0xDA, this), 3, 16, 12),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("CALL C,u16",       std::bind(&CPU::opcode0xDC, this), 3, 24, 12),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("SBC A,u8",         std::bind(&CPU::opcode0xDE, this), 2,  8,  8),
            INSTRUCTION("RST 18h",          std::bind(&CPU::opcode0xDF, this), 1, 16, 16),

            //0xE0
            INSTRUCTION("LD (FF00+u8),A",   std::bind(&CPU::opcode0xE0, this), 2, 12, 12),
            INSTRUCTION("POP HL",           std::bind(&CPU::opcode0xE1, this), 1, 12, 12),
            INSTRUCTION("LD (FF00+C),A",    std::bind(&CPU::opcode0xE2, this), 1,  8,  8),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("PUSH HL",          std::bind(&CPU::opcode0xE5, this), 1, 16, 16),
            INSTRUCTION("AND A,u8",         std::bind(&CPU::opcode0xE6, this), 2,  8,  8),
            INSTRUCTION("RST 20h",          std::bind(&CPU::opcode0xE7, this), 1, 16, 16),
            INSTRUCTION("ADD SP,i8",        std::bind(&CPU::opcode0xE8, this), 2, 16, 16),
            INSTRUCTION("JP HL",            std::bind(&CPU::opcode0xE9, this), 1,  4,  4),
            INSTRUCTION("LD (u16),A",       std::bind(&CPU::opcode0xEA, this), 3, 16, 16),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("XOR A,u8",         std::bind(&CPU::opcode0xEE, this), 2,  8,  8),
            INSTRUCTION("RST 28h",          std::bind(&CPU::opcode0xEF, this), 1, 16, 16),

            //0xF0
            INSTRUCTION("LD A,(FF00+u8)",   std::bind(&CPU::opcode0xF0, this), 2, 12, 12),
            INSTRUCTION("POP AF",           std::bind(&CPU::opcode0xF1, this), 1, 12, 12),
            INSTRUCTION("LD A,(FF00+C)",    std::bind(&CPU::opcode0xF2, this), 1,  8,  8),
            INSTRUCTION("DI",               std::bind(&CPU::opcode0xF3, this), 1,  4,  4),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("PUSH AF",          std::bind(&CPU::opcode0xF5, this), 1, 16, 16),
            INSTRUCTION("OR A,u8",          std::bind(&CPU::opcode0xF6, this), 2,  8,  8),
            INSTRUCTION("RST 30h",          std::bind(&CPU::opcode0xF7, this), 1, 16, 16),
            INSTRUCTION("LD HL,SP+i8",      std::bind(&CPU::opcode0xF8, this), 2, 12, 12),
            INSTRUCTION("LD SP,HL",         std::bind(&CPU::opcode0xF9, this), 1,  8,  8),
            INSTRUCTION("LD A,(u16)",       std::bind(&CPU::opcode0xFA, this), 3, 16, 16),
            INSTRUCTION("EI",               std::bind(&CPU::opcode0xFB, this), 1,  4,  4),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("CP A,u8",          std::bind(&CPU::opcode0xFE, this), 2,  8,  8),
            INSTRUCTION("RST 38h",          std::bind(&CPU::opcode0xFF, this), 1, 16, 16)
        };
        
        const Instruction instructionsCB[0x100] = 
        {
            //0x00
            INSTRUCTION("RLC B",            std::bind(&CPU::opcodeCB0x00, this), 2,  8,  8),
            INSTRUCTION("RLC C",            std::bind(&CPU::opcodeCB0x01, this), 2,  8,  8),
            INSTRUCTION("RLC D",            std::bind(&CPU::opcodeCB0x02, this), 2,  8,  8),
            INSTRUCTION("RLC E",            std::bind(&CPU::opcodeCB0x03, this), 2,  8,  8),
            INSTRUCTION("RLC H",            std::bind(&CPU::opcodeCB0x04, this), 2,  8,  8),
            INSTRUCTION("RLC L",            std::bind(&CPU::opcodeCB0x05, this), 2,  8,  8),
            INSTRUCTION("RLC (HL)",         std::bind(&CPU::opcodeCB0x06, this), 2, 16, 16),
            INSTRUCTION("RLC A",            std::bind(&CPU::opcodeCB0x07, this), 2,  8,  8),
            INSTRUCTION("RRC B",            std::bind(&CPU::opcodeCB0x08, this), 2,  8,  8),
            INSTRUCTION("RRC C",            std::bind(&CPU::opcodeCB0x09, this), 2,  8,  8),
            INSTRUCTION("RRC D",            std::bind(&CPU::opcodeCB0x0A, this), 2,  8,  8),
            INSTRUCTION("RRC E",            std::bind(&CPU::opcodeCB0x0B, this), 2,  8,  8),
            INSTRUCTION("RRC H",            std::bind(&CPU::opcodeCB0x0C, this), 2,  8,  8),
            INSTRUCTION("RRC L",            std::bind(&CPU::opcodeCB0x0D, this), 2,  8,  8),
            INSTRUCTION("RRC (HL)",         std::bind(&CPU::opcodeCB0x0E, this), 2, 16, 16),
            INSTRUCTION("RRC A",            std::bind(&CPU::opcodeCB0x0F, this), 2,  8,  8),

            //0x10
            INSTRUCTION("RL B",             std::bind(&CPU::opcodeCB0x10, this), 2,  8,  8),
            INSTRUCTION("RL C",             std::bind(&CPU::opcodeCB0x11, this), 2,  8,  8),
            INSTRUCTION("RL D",             std::bind(&CPU::opcodeCB0x12, this), 2,  8,  8),
            INSTRUCTION("RL E",             std::bind(&CPU::opcodeCB0x13, this), 2,  8,  8),
            INSTRUCTION("RL H",             std::bind(&CPU::opcodeCB0x14, this), 2,  8,  8),
            INSTRUCTION("RL L",             std::bind(&CPU::opcodeCB0x15, this), 2,  8,  8),
            INSTRUCTION("RL (HL)",          std::bind(&CPU::opcodeCB0x16, this), 2, 16, 16),
            INSTRUCTION("RL A",             std::bind(&CPU::opcodeCB0x17, this), 2,  8,  8),
            INSTRUCTION("RR B",             std::bind(&CPU::opcodeCB0x18, this), 2,  8,  8),
            INSTRUCTION("RR C",             std::bind(&CPU::opcodeCB0x19, this), 2,  8,  8),
            INSTRUCTION("RR D",             std::bind(&CPU::opcodeCB0x1A, this), 2,  8,  8),
            INSTRUCTION("RR E",             std::bind(&CPU::opcodeCB0x1B, this), 2,  8,  8),
            INSTRUCTION("RR H",             std::bind(&CPU::opcodeCB0x1C, this), 2,  8,  8),
            INSTRUCTION("RR L",             std::bind(&CPU::opcodeCB0x1D, this), 2,  8,  8),
            INSTRUCTION("RR (HL)",          std::bind(&CPU::opcodeCB0x1E, this), 2, 16, 16),
            INSTRUCTION("RR A",             std::bind(&CPU::opcodeCB0x1F, this), 2,  8,  8),

            //0x20
            INSTRUCTION("SLA B",            std::bind(&CPU::opcodeCB0x20, this), 2,  8,  8),
            INSTRUCTION("SLA C",            std::bind(&CPU::opcodeCB0x21, this), 2,  8,  8),
            INSTRUCTION("SLA D",            std::bind(&CPU::opcodeCB0x22, this), 2,  8,  8),
            INSTRUCTION("SLA E",            std::bind(&CPU::opcodeCB0x23, this), 2,  8,  8),
            INSTRUCTION("SLA H",            std::bind(&CPU::opcodeCB0x24, this), 2,  8,  8),
            INSTRUCTION("SLA L",            std::bind(&CPU::opcodeCB0x25, this), 2,  8,  8),
            INSTRUCTION("SLA (HL)",         std::bind(&CPU::opcodeCB0x26, this), 2, 16, 16),
            INSTRUCTION("SLA A",            std::bind(&CPU::opcodeCB0x27, this), 2,  8,  8),
            INSTRUCTION("SRA B",            std::bind(&CPU::opcodeCB0x28, this), 2,  8,  8),
            INSTRUCTION("SRA C",            std::bind(&CPU::opcodeCB0x29, this), 2,  8,  8),
            INSTRUCTION("SRA D",            std::bind(&CPU::opcodeCB0x2A, this), 2,  8,  8),
            INSTRUCTION("SRA E",            std::bind(&CPU::opcodeCB0x2B, this), 2,  8,  8),
            INSTRUCTION("SRA H",            std::bind(&CPU::opcodeCB0x2C, this), 2,  8,  8),
            INSTRUCTION("SRA L",            std::bind(&CPU::opcodeCB0x2D, this), 2,  8,  8),
            INSTRUCTION("SRA (HL)",         std::bind(&CPU::opcodeCB0x2E, this), 2, 16, 16),
            INSTRUCTION("SRA A",            std::bind(&CPU::opcodeCB0x2F, this), 2,  8,  8),

            //0x30
            INSTRUCTION("SWAP B",           std::bind(&CPU::opcodeCB0x30, this), 2,  8,  8),
            INSTRUCTION("SWAP C",           std::bind(&CPU::opcodeCB0x31, this), 2,  8,  8),
            INSTRUCTION("SWAP D",           std::bind(&CPU::opcodeCB0x32, this), 2,  8,  8),
            INSTRUCTION("SWAP E",           std::bind(&CPU::opcodeCB0x33, this), 2,  8,  8),
            INSTRUCTION("SWAP H",           std::bind(&CPU::opcodeCB0x34, this), 2,  8,  8),
            INSTRUCTION("SWAP L",           std::bind(&CPU::opcodeCB0x35, this), 2,  8,  8),
            INSTRUCTION("SWAP (HL)",        std::bind(&CPU::opcodeCB0x36, this), 2, 16, 16),
            INSTRUCTION("SWAP A",           std::bind(&CPU::opcodeCB0x37, this), 2,  8,  8),
            INSTRUCTION("SRL B",            std::bind(&CPU::opcodeCB0x38, this), 2,  8,  8),
            INSTRUCTION("SRL C",            std::bind(&CPU::opcodeCB0x39, this), 2,  8,  8),
            INSTRUCTION("SRL D",            std::bind(&CPU::opcodeCB0x3A, this), 2,  8,  8),
            INSTRUCTION("SRL E",            std::bind(&CPU::opcodeCB0x3B, this), 2,  8,  8),
            INSTRUCTION("SRL H",            std::bind(&CPU::opcodeCB0x3C, this), 2,  8,  8),
            INSTRUCTION("SRL L",            std::bind(&CPU::opcodeCB0x3D, this), 2,  8,  8),
            INSTRUCTION("SRL (HL)",         std::bind(&CPU::opcodeCB0x3E, this), 2, 16, 16),
            INSTRUCTION("SRL A",            std::bind(&CPU::opcodeCB0x3F, this), 2,  8,  8),

            //0x40
            INSTRUCTION("BIT 0,B",          std::bind(&CPU::opcodeCB0x40, this), 2,  8,  8),
            INSTRUCTION("BIT 0,C",          std::bind(&CPU::opcodeCB0x41, this), 2,  8,  8),
            INSTRUCTION("BIT 0,D",          std::bind(&CPU::opcodeCB0x42, this), 2,  8,  8),
            INSTRUCTION("BIT 0,E",          std::bind(&CPU::opcodeCB0x43, this), 2,  8,  8),
            INSTRUCTION("BIT 0,H",          std::bind(&CPU::opcodeCB0x44, this), 2,  8,  8),
            INSTRUCTION("BIT 0,L",          std::bind(&CPU::opcodeCB0x45, this), 2,  8,  8),
            INSTRUCTION("BIT 0,(HL)",       std::bind(&CPU::opcodeCB0x46, this), 2, 12, 12),
            INSTRUCTION("BIT 0,A",          std::bind(&CPU::opcodeCB0x47, this), 2,  8,  8),
            INSTRUCTION("BIT 1,B",          std::bind(&CPU::opcodeCB0x48, this), 2,  8,  8),
            INSTRUCTION("BIT 1,C",          std::bind(&CPU::opcodeCB0x49, this), 2,  8,  8),
            INSTRUCTION("BIT 1,D",          std::bind(&CPU::opcodeCB0x4A, this), 2,  8,  8),
            INSTRUCTION("BIT 1,E",          std::bind(&CPU::opcodeCB0x4B, this), 2,  8,  8),
            INSTRUCTION("BIT 1,H",          std::bind(&CPU::opcodeCB0x4C, this), 2,  8,  8),
            INSTRUCTION("BIT 1,L",          std::bind(&CPU::opcodeCB0x4D, this), 2,  8,  8),
            INSTRUCTION("BIT 1,(HL)",       std::bind(&CPU::opcodeCB0x4E, this), 2, 12, 12),
            INSTRUCTION("BIT 1,A",          std::bind(&CPU::opcodeCB0x4F, this), 2,  8,  8),

            //0x50
            INSTRUCTION("BIT 2,B",          std::bind(&CPU::opcodeCB0x50, this), 2,  8,  8),
            INSTRUCTION("BIT 2,C",          std::bind(&CPU::opcodeCB0x51, this), 2,  8,  8),
            INSTRUCTION("BIT 2,D",          std::bind(&CPU::opcodeCB0x52, this), 2,  8,  8),
            INSTRUCTION("BIT 2,E",          std::bind(&CPU::opcodeCB0x53, this), 2,  8,  8),
            INSTRUCTION("BIT 2,H",          std::bind(&CPU::opcodeCB0x54, this), 2,  8,  8),
            INSTRUCTION("BIT 2,L",          std::bind(&CPU::opcodeCB0x55, this), 2,  8,  8),
            INSTRUCTION("BIT 2,(HL)",       std::bind(&CPU::opcodeCB0x56, this), 2, 12, 12),
            INSTRUCTION("BIT 2,A",          std::bind(&CPU::opcodeCB0x57, this), 2,  8,  8),
            INSTRUCTION("BIT 3,B",          std::bind(&CPU::opcodeCB0x58, this), 2,  8,  8),
            INSTRUCTION("BIT 3,C",          std::bind(&CPU::opcodeCB0x59, this), 2,  8,  8),
            INSTRUCTION("BIT 3,D",          std::bind(&CPU::opcodeCB0x5A, this), 2,  8,  8),
            INSTRUCTION("BIT 3,E",          std::bind(&CPU::opcodeCB0x5B, this), 2,  8,  8),
            INSTRUCTION("BIT 3,H",          std::bind(&CPU::opcodeCB0x5C, this), 2,  8,  8),
            INSTRUCTION("BIT 3,L",          std::bind(&CPU::opcodeCB0x5D, this), 2,  8,  8),
            INSTRUCTION("BIT 3,(HL)",       std::bind(&CPU::opcodeCB0x5E, this), 2, 12, 12),
            INSTRUCTION("BIT 3,A",          std::bind(&CPU::opcodeCB0x5F, this), 2,  8,  8),

            //0x60
            INSTRUCTION("BIT 4,B",          std::bind(&CPU::opcodeCB0x60, this), 2,  8,  8),
            INSTRUCTION("BIT 4,C",          std::bind(&CPU::opcodeCB0x61, this), 2,  8,  8),
            INSTRUCTION("BIT 4,D",          std::bind(&CPU::opcodeCB0x62, this), 2,  8,  8),
            INSTRUCTION("BIT 4,E",          std::bind(&CPU::opcodeCB0x63, this), 2,  8,  8),
            INSTRUCTION("BIT 4,H",          std::bind(&CPU::opcodeCB0x64, this), 2,  8,  8),
            INSTRUCTION("BIT 4,L",          std::bind(&CPU::opcodeCB0x65, this), 2,  8,  8),
            INSTRUCTION("BIT 4,(HL)",       std::bind(&CPU::opcodeCB0x66, this), 2, 12, 12),
            INSTRUCTION("BIT 4,A",          std::bind(&CPU::opcodeCB0x67, this), 2,  8,  8),
            INSTRUCTION("BIT 5,B",          std::bind(&CPU::opcodeCB0x68, this), 2,  8,  8),
            INSTRUCTION("BIT 5,C",          std::bind(&CPU::opcodeCB0x69, this), 2,  8,  8),
            INSTRUCTION("BIT 5,D",          std::bind(&CPU::opcodeCB0x6A, this), 2,  8,  8),
            INSTRUCTION("BIT 5,E",          std::bind(&CPU::opcodeCB0x6B, this), 2,  8,  8),
            INSTRUCTION("BIT 5,H",          std::bind(&CPU::opcodeCB0x6C, this), 2,  8,  8),
            INSTRUCTION("BIT 5,L",          std::bind(&CPU::opcodeCB0x6D, this), 2,  8,  8),
            INSTRUCTION("BIT 5,(HL)",       std::bind(&CPU::opcodeCB0x6E, this), 2, 12, 12),
            INSTRUCTION("BIT 5,A",          std::bind(&CPU::opcodeCB0x6F, this), 2,  8,  8),

            //0x70
            INSTRUCTION("BIT 6,B",          std::bind(&CPU::opcodeCB0x70, this), 2,  8,  8),
            INSTRUCTION("BIT 6,C",          std::bind(&CPU::opcodeCB0x71, this), 2,  8,  8),
            INSTRUCTION("BIT 6,D",          std::bind(&CPU::opcodeCB0x72, this), 2,  8,  8),
            INSTRUCTION("BIT 6,E",          std::bind(&CPU::opcodeCB0x73, this), 2,  8,  8),
            INSTRUCTION("BIT 6,H",          std::bind(&CPU::opcodeCB0x74, this), 2,  8,  8),
            INSTRUCTION("BIT 6,L",          std::bind(&CPU::opcodeCB0x75, this), 2,  8,  8),
            INSTRUCTION("BIT 6,(HL)",       std::bind(&CPU::opcodeCB0x76, this), 2, 12, 12),
            INSTRUCTION("BIT 6,A",          std::bind(&CPU::opcodeCB0x77, this), 2,  8,  8),
            INSTRUCTION("BIT 7,B",          std::bind(&CPU::opcodeCB0x78, this), 2,  8,  8),
            INSTRUCTION("BIT 7,C",          std::bind(&CPU::opcodeCB0x79, this), 2,  8,  8),
            INSTRUCTION("BIT 7,D",          std::bind(&CPU::opcodeCB0x7A, this), 2,  8,  8),
            INSTRUCTION("BIT 7,E",          std::bind(&CPU::opcodeCB0x7B, this), 2,  8,  8),
            INSTRUCTION("BIT 7,H",          std::bind(&CPU::opcodeCB0x7C, this), 2,  8,  8),
            INSTRUCTION("BIT 7,L",          std::bind(&CPU::opcodeCB0x7D, this), 2,  8,  8),
            INSTRUCTION("BIT 7,(HL)",       std::bind(&CPU::opcodeCB0x7E, this), 2, 12, 12),
            INSTRUCTION("BIT 7,A",          std::bind(&CPU::opcodeCB0x7F, this), 2,  8,  8),

            //0x80
            INSTRUCTION("RES 0,B",          std::bind(&CPU::opcodeCB0x80, this), 2,  8,  8),
            INSTRUCTION("RES 0,C",          std::bind(&CPU::opcodeCB0x81, this), 2,  8,  8),
            INSTRUCTION("RES 0,D",          std::bind(&CPU::opcodeCB0x82, this), 2,  8,  8),
            INSTRUCTION("RES 0,E",          std::bind(&CPU::opcodeCB0x83, this), 2,  8,  8),
            INSTRUCTION("RES 0,H",          std::bind(&CPU::opcodeCB0x84, this), 2,  8,  8),
            INSTRUCTION("RES 0,L",          std::bind(&CPU::opcodeCB0x85, this), 2,  8,  8),
            INSTRUCTION("RES 0,(HL)",       std::bind(&CPU::opcodeCB0x86, this), 2, 16, 16),
            INSTRUCTION("RES 0,A",          std::bind(&CPU::opcodeCB0x87, this), 2,  8,  8),
            INSTRUCTION("RES 1,B",          std::bind(&CPU::opcodeCB0x88, this), 2,  8,  8),
            INSTRUCTION("RES 1,C",          std::bind(&CPU::opcodeCB0x89, this), 2,  8,  8),
            INSTRUCTION("RES 1,D",          std::bind(&CPU::opcodeCB0x8A, this), 2,  8,  8),
            INSTRUCTION("RES 1,E",          std::bind(&CPU::opcodeCB0x8B, this), 2,  8,  8),
            INSTRUCTION("RES 1,H",          std::bind(&CPU::opcodeCB0x8C, this), 2,  8,  8),
            INSTRUCTION("RES 1,L",          std::bind(&CPU::opcodeCB0x8D, this), 2,  8,  8),
            INSTRUCTION("RES 1,(HL)",       std::bind(&CPU::opcodeCB0x8E, this), 2, 16, 16),
            INSTRUCTION("RES 1,A",          std::bind(&CPU::opcodeCB0x8F, this), 2,  8,  8),

            //0x90
            INSTRUCTION("RES 2,B",          std::bind(&CPU::opcodeCB0x90, this), 2,  8,  8),
            INSTRUCTION("RES 2,C",          std::bind(&CPU::opcodeCB0x91, this), 2,  8,  8),
            INSTRUCTION("RES 2,D",          std::bind(&CPU::opcodeCB0x92, this), 2,  8,  8),
            INSTRUCTION("RES 2,E",          std::bind(&CPU::opcodeCB0x93, this), 2,  8,  8),
            INSTRUCTION("RES 2,H",          std::bind(&CPU::opcodeCB0x94, this), 2,  8,  8),
            INSTRUCTION("RES 2,L",          std::bind(&CPU::opcodeCB0x95, this), 2,  8,  8),
            INSTRUCTION("RES 2,(HL)",       std::bind(&CPU::opcodeCB0x96, this), 2, 16, 16),
            INSTRUCTION("RES 2,A",          std::bind(&CPU::opcodeCB0x97, this), 2,  8,  8),
            INSTRUCTION("RES 3,B",          std::bind(&CPU::opcodeCB0x98, this), 2,  8,  8),
            INSTRUCTION("RES 3,C",          std::bind(&CPU::opcodeCB0x99, this), 2,  8,  8),
            INSTRUCTION("RES 3,D",          std::bind(&CPU::opcodeCB0x9A, this), 2,  8,  8),
            INSTRUCTION("RES 3,E",          std::bind(&CPU::opcodeCB0x9B, this), 2,  8,  8),
            INSTRUCTION("RES 3,H",          std::bind(&CPU::opcodeCB0x9C, this), 2,  8,  8),
            INSTRUCTION("RES 3,L",          std::bind(&CPU::opcodeCB0x9D, this), 2,  8,  8),
            INSTRUCTION("RES 3,(HL)",       std::bind(&CPU::opcodeCB0x9E, this), 2, 16, 16),
            INSTRUCTION("RES 3,A",          std::bind(&CPU::opcodeCB0x9F, this), 2,  8,  8),

            //0xA0
            INSTRUCTION("RES 4,B",          std::bind(&CPU::opcodeCB0xA0, this), 2,  8,  8),
            INSTRUCTION("RES 4,C",          std::bind(&CPU::opcodeCB0xA1, this), 2,  8,  8),
            INSTRUCTION("RES 4,D",          std::bind(&CPU::opcodeCB0xA2, this), 2,  8,  8),
            INSTRUCTION("RES 4,E",          std::bind(&CPU::opcodeCB0xA3, this), 2,  8,  8),
            INSTRUCTION("RES 4,H",          std::bind(&CPU::opcodeCB0xA4, this), 2,  8,  8),
            INSTRUCTION("RES 4,L",          std::bind(&CPU::opcodeCB0xA5, this), 2,  8,  8),
            INSTRUCTION("RES 4,(HL)",       std::bind(&CPU::opcodeCB0xA6, this), 2, 16, 16),
            INSTRUCTION("RES 4,A",          std::bind(&CPU::opcodeCB0xA7, this), 2,  8,  8),
            INSTRUCTION("RES 5,B",          std::bind(&CPU::opcodeCB0xA8, this), 2,  8,  8),
            INSTRUCTION("RES 5,C",          std::bind(&CPU::opcodeCB0xA9, this), 2,  8,  8),
            INSTRUCTION("RES 5,D",          std::bind(&CPU::opcodeCB0xAA, this), 2,  8,  8),
            INSTRUCTION("RES 5,E",          std::bind(&CPU::opcodeCB0xAB, this), 2,  8,  8),
            INSTRUCTION("RES 5,H",          std::bind(&CPU::opcodeCB0xAC, this), 2,  8,  8),
            INSTRUCTION("RES 5,L",          std::bind(&CPU::opcodeCB0xAD, this), 2,  8,  8),
            INSTRUCTION("RES 5,(HL)",       std::bind(&CPU::opcodeCB0xAE, this), 2, 16, 16),
            INSTRUCTION("RES 5,A",          std::bind(&CPU::opcodeCB0xAF, this), 2,  8,  8),

            //0xB0
            INSTRUCTION("RES 6,B",          std::bind(&CPU::opcodeCB0xB0, this), 2,  8,  8),
            INSTRUCTION("RES 6,C",          std::bind(&CPU::opcodeCB0xB1, this), 2,  8,  8),
            INSTRUCTION("RES 6,D",          std::bind(&CPU::opcodeCB0xB2, this), 2,  8,  8),
            INSTRUCTION("RES 6,E",          std::bind(&CPU::opcodeCB0xB3, this), 2,  8,  8),
            INSTRUCTION("RES 6,H",          std::bind(&CPU::opcodeCB0xB4, this), 2,  8,  8),
            INSTRUCTION("RES 6,L",          std::bind(&CPU::opcodeCB0xB5, this), 2,  8,  8),
            INSTRUCTION("RES 6,(HL)",       std::bind(&CPU::opcodeCB0xB6, this), 2, 16, 16),
            INSTRUCTION("RES 6,A",          std::bind(&CPU::opcodeCB0xB7, this), 2,  8,  8),
            INSTRUCTION("RES 7,B",          std::bind(&CPU::opcodeCB0xB8, this), 2,  8,  8),
            INSTRUCTION("RES 7,C",          std::bind(&CPU::opcodeCB0xB9, this), 2,  8,  8),
            INSTRUCTION("RES 7,D",          std::bind(&CPU::opcodeCB0xBA, this), 2,  8,  8),
            INSTRUCTION("RES 7,E",          std::bind(&CPU::opcodeCB0xBB, this), 2,  8,  8),
            INSTRUCTION("RES 7,H",          std::bind(&CPU::opcodeCB0xBC, this), 2,  8,  8),
            INSTRUCTION("RES 7,L",          std::bind(&CPU::opcodeCB0xBD, this), 2,  8,  8),
            INSTRUCTION("RES 7,(HL)",       std::bind(&CPU::opcodeCB0xBE, this), 2, 16, 16),
            INSTRUCTION("RES 7,A",          std::bind(&CPU::opcodeCB0xBF, this), 2,  8,  8),

            //0xC0
            INSTRUCTION("SET 0,B",          std::bind(&CPU::opcodeCB0xC0, this), 2,  8,  8),
            INSTRUCTION("SET 0,C",          std::bind(&CPU::opcodeCB0xC1, this), 2,  8,  8),
            INSTRUCTION("SET 0,D",          std::bind(&CPU::opcodeCB0xC2, this), 2,  8,  8),
            INSTRUCTION("SET 0,E",          std::bind(&CPU::opcodeCB0xC3, this), 2,  8,  8),
            INSTRUCTION("SET 0,H",          std::bind(&CPU::opcodeCB0xC4, this), 2,  8,  8),
            INSTRUCTION("SET 0,L",          std::bind(&CPU::opcodeCB0xC5, this), 2,  8,  8),
            INSTRUCTION("SET 0,(HL)",       std::bind(&CPU::opcodeCB0xC6, this), 2, 16, 16),
            INSTRUCTION("SET 0,A",          std::bind(&CPU::opcodeCB0xC7, this), 2,  8,  8),
            INSTRUCTION("SET 1,B",          std::bind(&CPU::opcodeCB0xC8, this), 2,  8,  8),
            INSTRUCTION("SET 1,C",          std::bind(&CPU::opcodeCB0xC9, this), 2,  8,  8),
            INSTRUCTION("SET 1,D",          std::bind(&CPU::opcodeCB0xCA, this), 2,  8,  8),
            INSTRUCTION("SET 1,E",          std::bind(&CPU::opcodeCB0xCB, this), 2,  8,  8),
            INSTRUCTION("SET 1,H",          std::bind(&CPU::opcodeCB0xCC, this), 2,  8,  8),
            INSTRUCTION("SET 1,L",          std::bind(&CPU::opcodeCB0xCD, this), 2,  8,  8),
            INSTRUCTION("SET 1,(HL)",       std::bind(&CPU::opcodeCB0xCE, this), 2, 16, 16),
            INSTRUCTION("SET 1,A",          std::bind(&CPU::opcodeCB0xCF, this), 2,  8,  8),

            //0xD0
            INSTRUCTION("SET 2,B",          std::bind(&CPU::opcodeCB0xD0, this), 2,  8,  8),
            INSTRUCTION("SET 2,C",          std::bind(&CPU::opcodeCB0xD1, this), 2,  8,  8),
            INSTRUCTION("SET 2,D",          std::bind(&CPU::opcodeCB0xD2, this), 2,  8,  8),
            INSTRUCTION("SET 2,E",          std::bind(&CPU::opcodeCB0xD3, this), 2,  8,  8),
            INSTRUCTION("SET 2,H",          std::bind(&CPU::opcodeCB0xD4, this), 2,  8,  8),
            INSTRUCTION("SET 2,L",          std::bind(&CPU::opcodeCB0xD5, this), 2,  8,  8),
            INSTRUCTION("SET 2,(HL)",       std::bind(&CPU::opcodeCB0xD6, this), 2, 16, 16),
            INSTRUCTION("SET 2,A",          std::bind(&CPU::opcodeCB0xD7, this), 2,  8,  8),
            INSTRUCTION("SET 3,B",          std::bind(&CPU::opcodeCB0xD8, this), 2,  8,  8),
            INSTRUCTION("SET 3,C",          std::bind(&CPU::opcodeCB0xD9, this), 2,  8,  8),
            INSTRUCTION("SET 3,D",          std::bind(&CPU::opcodeCB0xDA, this), 2,  8,  8),
            INSTRUCTION("SET 3,E",          std::bind(&CPU::opcodeCB0xDB, this), 2,  8,  8),
            INSTRUCTION("SET 3,H",          std::bind(&CPU::opcodeCB0xDC, this), 2,  8,  8),
            INSTRUCTION("SET 3,L",          std::bind(&CPU::opcodeCB0xDD, this), 2,  8,  8),
            INSTRUCTION("SET 3,(HL)",       std::bind(&CPU::opcodeCB0xDE, this), 2, 16, 16),
            INSTRUCTION("SET 3,A",          std::bind(&CPU::opcodeCB0xDF, this), 2,  8,  8),

            //0xE0
            INSTRUCTION("SET 4,B",          std::bind(&CPU::opcodeCB0xE0, this), 2,  8,  8),
            INSTRUCTION("SET 4,C",          std::bind(&CPU::opcodeCB0xE1, this), 2,  8,  8),
            INSTRUCTION("SET 4,D",          std::bind(&CPU::opcodeCB0xE2, this), 2,  8,  8),
            INSTRUCTION("SET 4,E",          std::bind(&CPU::opcodeCB0xE3, this), 2,  8,  8),
            INSTRUCTION("SET 4,H",          std::bind(&CPU::opcodeCB0xE4, this), 2,  8,  8),
            INSTRUCTION("SET 4,L",          std::bind(&CPU::opcodeCB0xE5, this), 2,  8,  8),
            INSTRUCTION("SET 4,(HL)",       std::bind(&CPU::opcodeCB0xE6, this), 2, 16, 16),
            INSTRUCTION("SET 4,A",          std::bind(&CPU::opcodeCB0xE7, this), 2,  8,  8),
            INSTRUCTION("SET 5,B",          std::bind(&CPU::opcodeCB0xE8, this), 2,  8,  8),
            INSTRUCTION("SET 5,C",          std::bind(&CPU::opcodeCB0xE9, this), 2,  8,  8),
            INSTRUCTION("SET 5,D",          std::bind(&CPU::opcodeCB0xEA, this), 2,  8,  8),
            INSTRUCTION("SET 5,E",          std::bind(&CPU::opcodeCB0xEB, this), 2,  8,  8),
            INSTRUCTION("SET 5,H",          std::bind(&CPU::opcodeCB0xEC, this), 2,  8,  8),
            INSTRUCTION("SET 5,L",          std::bind(&CPU::opcodeCB0xED, this), 2,  8,  8),
            INSTRUCTION("SET 5,(HL)",       std::bind(&CPU::opcodeCB0xEE, this), 2, 16, 16),
            INSTRUCTION("SET 5,A",          std::bind(&CPU::opcodeCB0xEF, this), 2,  8,  8),

            //0xF0
            INSTRUCTION("SET 6,B",          std::bind(&CPU::opcodeCB0xF0, this), 2,  8,  8),
            INSTRUCTION("SET 6,C",          std::bind(&CPU::opcodeCB0xF1, this), 2,  8,  8),
            INSTRUCTION("SET 6,D",          std::bind(&CPU::opcodeCB0xF2, this), 2,  8,  8),
            INSTRUCTION("SET 6,E",          std::bind(&CPU::opcodeCB0xF3, this), 2,  8,  8),
            INSTRUCTION("SET 6,H",          std::bind(&CPU::opcodeCB0xF4, this), 2,  8,  8),
            INSTRUCTION("SET 6,L",          std::bind(&CPU::opcodeCB0xF5, this), 2,  8,  8),
            INSTRUCTION("SET 6,(HL)",       std::bind(&CPU::opcodeCB0xF6, this), 2, 16, 16),
            INSTRUCTION("SET 6,A",          std::bind(&CPU::opcodeCB0xF7, this), 2,  8,  8),
            INSTRUCTION("SET 7,B",          std::bind(&CPU::opcodeCB0xF8, this), 2,  8,  8),
            INSTRUCTION("SET 7,C",          std::bind(&CPU::opcodeCB0xF9, this), 2,  8,  8),
            INSTRUCTION("SET 7,D",          std::bind(&CPU::opcodeCB0xFA, this), 2,  8,  8),
            INSTRUCTION("SET 7,E",          std::bind(&CPU::opcodeCB0xFB, this), 2,  8,  8),
            INSTRUCTION("SET 7,H",          std::bind(&CPU::opcodeCB0xFC, this), 2,  8,  8),
            INSTRUCTION("SET 7,L",          std::bind(&CPU::opcodeCB0xFD, this), 2,  8,  8),
            INSTRUCTION("SET 7,(HL)",       std::bind(&CPU::opcodeCB0xFE, this), 2, 16, 16),
            INSTRUCTION("SET 7,A",          std::bind(&CPU::opcodeCB0xFF, this), 2,  8,  8)
        };
};