#pragma once

#include "core.h"

#include "instruction.h"
#include "flags.h"

class MMU;

class CPU
{
    public:
        CPU();

        void setMMU(MMU* mmu);

        void tick();

        inline void setFlag(const Flags::Register& flag)    { m_Registers.F |= flag; }
        inline void clearFlag(const Flags::Register& flag)  { m_Registers.F &= ~flag; }
        inline void flipFlag(const Flags::Register& flag)   { m_Registers.F ^= flag; }
        inline bool isFlagSet(const Flags::Register& flag)  { return m_Registers.F & flag; }

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

        void reset();
        void handleInterrupts();
        
        MMU* m_MMU;

        Registers m_Registers;
        bool m_IME;
    public:

        //--------------------------------------Opcode Helpers--------------------------------------//

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

        void opcodePUSH(const u8& reg);
        void opcodePOP(u8& reg);

        //--------------------------------------Opcodes--------------------------------------//

        //0x00

        void op00(); // NOP
        void op01(); // LD BC,u16
        void op02(); // LD (BC),A
        void op03(); // INC BC
        void op04(); // INC B
        void op05(); // DEC B
        void op06(); // LD B,u8
        void op07(); // RLCA
        void op08(); // LD (u16),SP
        void op09(); // ADD HL,BC
        void op0A(); // LD A,(BC)
        void op0B(); // DEC BC
        void op0C(); // INC C
        void op0D(); // DEC C
        void op0E(); // LD C,u8
        void op0F(); // RRCA

        //0x10

        void op10(); // STOP
        void op11(); // LD DE,u16
        void op12(); // LD (DE),A
        void op13(); // INC DE
        void op14(); // INC D
        void op15(); // DEC D
        void op16(); // LD D,u8
        void op17(); // RLA
        void op18(); // JR i8
        void op19(); // ADD HL,DE
        void op1A(); // LD A,(DE)
        void op1B(); // DEC DE
        void op1C(); // INC E
        void op1D(); // DEC E
        void op1E(); // LD E,u8
        void op1F(); // RRA

        //0x20

        void op20(); // JR NZ,i8
        void op21(); // LD HL,u16
        void op22(); // LD (HL+),A
        void op23(); // INC HL
        void op24(); // INC H
        void op25(); // DEC H
        void op26(); // LD H,u8
        void op27(); // DAA
        void op28(); // JR Z,i8
        void op29(); // ADD HL,HL
        void op2A(); // LD A,(HL+)
        void op2B(); // DEC HL
        void op2C(); // INC L
        void op2D(); // DEC L
        void op2E(); // LD L,u8
        void op2F(); // CPL

        //0x30

        void op30(); // JR NC,i8
        void op31(); // LD SP,u16
        void op32(); // LD (HL-),A
        void op33(); // INC SP
        void op34(); // INC (HL)
        void op35(); // DEC (HL)
        void op36(); // LD (HL),u8
        void op37(); // SCF
        void op38(); // JR C,i8
        void op39(); // ADD HL,SP
        void op3A(); // LD A,(HL-)
        void op3B(); // DEC SP
        void op3C(); // INC A
        void op3D(); // DEC A
        void op3E(); // LD A,u8
        void op3F(); // CCF

        //0x40

        void op40(); // LD B,B
        void op41(); // LD B,C
        void op42(); // LD B,D
        void op43(); // LD B,E
        void op44(); // LD B,H
        void op45(); // LD B,L
        void op46(); // LD B,(HL)
        void op47(); // LD B,A
        void op48(); // LD C,B
        void op49(); // LD C,C
        void op4A(); // LD C,D
        void op4B(); // LD C,E
        void op4C(); // LD C,H
        void op4D(); // LD C,L
        void op4E(); // LD C,(HL)
        void op4F(); // LD C,A

        //0x50

        void op50(); // LD D,B
        void op51(); // LD D,C
        void op52(); // LD D,D
        void op53(); // LD D,E
        void op54(); // LD D,H
        void op55(); // LD D,L
        void op56(); // LD D,(HL)
        void op57(); // LD D,A
        void op58(); // LD E,B
        void op59(); // LD E,C
        void op5A(); // LD E,D
        void op5B(); // LD E,E
        void op5C(); // LD E,H
        void op5D(); // LD E,L
        void op5E(); // LD E,(HL)
        void op5F(); // LD E,A

        //0x60

        void op60(); // LD H,B
        void op61(); // LD H,C
        void op62(); // LD H,D
        void op63(); // LD H,E
        void op64(); // LD H,H
        void op65(); // LD H,L
        void op66(); // LD H,(HL)
        void op67(); // LD H,A
        void op68(); // LD L,B
        void op69(); // LD L,C
        void op6A(); // LD L,D
        void op6B(); // LD L,E
        void op6C(); // LD L,H
        void op6D(); // LD L,L
        void op6E(); // LD L,(HL)
        void op6F(); // LD L,A

        //0x70

        void op70(); // LD (HL),B
        void op71(); // LD (HL),C
        void op72(); // LD (HL),D
        void op73(); // LD (HL),E
        void op74(); // LD (HL),H
        void op75(); // LD (HL),L
        void op76(); // HALT
        void op77(); // LD (HL),A
        void op78(); // LD A,B
        void op79(); // LD A,C
        void op7A(); // LD A,D
        void op7B(); // LD A,E
        void op7C(); // LD A,H
        void op7D(); // LD A,L
        void op7E(); // LD A,(HL)
        void op7F(); // LD A,A

        //0x80

        void op80(); // ADD A,B
        void op81(); // ADD A,C
        void op82(); // ADD A,D
        void op83(); // ADD A,E
        void op84(); // ADD A,H
        void op85(); // ADD A,L
        void op86(); // ADD A,(HL)
        void op87(); // ADD A,A
        void op88(); // ADC A,B
        void op89(); // ADC A,C
        void op8A(); // ADC A,D
        void op8B(); // ADC A,E
        void op8C(); // ADC A,H
        void op8D(); // ADC A,L
        void op8E(); // ADC A,(HL)
        void op8F(); // ADC A,A

        //0x90

        void op90(); // SUB A,B
        void op91(); // SUB A,C
        void op92(); // SUB A,D
        void op93(); // SUB A,E
        void op94(); // SUB A,H
        void op95(); // SUB A,L
        void op96(); // SUB A,(HL)
        void op97(); // SUB A,A
        void op98(); // SBC A,B
        void op99(); // SBC A,C
        void op9A(); // SBC A,D
        void op9B(); // SBC A,E
        void op9C(); // SBC A,H
        void op9D(); // SBC A,L
        void op9E(); // SBC A,(HL)
        void op9F(); // SBC A,A

        //0xA0

        void opA0(); // AND A,B
        void opA1(); // AND A,C
        void opA2(); // AND A,D
        void opA3(); // AND A,E
        void opA4(); // AND A,H
        void opA5(); // AND A,L
        void opA6(); // AND A,(HL)
        void opA7(); // AND A,A
        void opA8(); // XOR A,B
        void opA9(); // XOR A,C
        void opAA(); // XOR A,D
        void opAB(); // XOR A,E
        void opAC(); // XOR A,H
        void opAD(); // XOR A,L
        void opAE(); // XOR A,(HL)
        void opAF(); // XOR A,A

        //0xB0

        void opB0(); // OR A,B
        void opB1(); // OR A,C
        void opB2(); // OR A,D
        void opB3(); // OR A,E
        void opB4(); // OR A,H
        void opB5(); // OR A,L
        void opB6(); // OR A,(HL)
        void opB7(); // OR A,A
        void opB8(); // CP A,B
        void opB9(); // CP A,C
        void opBA(); // CP A,D
        void opBB(); // CP A,E
        void opBC(); // CP A,H
        void opBD(); // CP A,L
        void opBE(); // CP A,(HL)
        void opBF(); // CP A,A

        //0xC0

        void opC0(); // RET NZ
        void opC1(); // POP BC
        void opC2(); // JP NZ,u16
        void opC3(); // JP u16
        void opC4(); // CALL NZ,u16
        void opC5(); // PUSH BC
        void opC6(); // ADD A,u8
        void opC7(); // RST 00h
        void opC8(); // RET Z
        void opC9(); // RET
        void opCA(); // JP Z,u16
        void opCB(); // PREFIX CB
        void opCC(); // CALL Z,u16
        void opCD(); // CALL u16
        void opCE(); // ADC A,u8
        void opCF(); // RST 08h

        //0xD0

        void opD0(); // RET NC
        void opD1(); // POP DE
        void opD2(); // JP NC,u16
        void opD3(); // UNUSED
        void opD4(); // CALL NC,u16
        void opD5(); // PUSH DE
        void opD6(); // SUB A,u8
        void opD7(); // RST 10h
        void opD8(); // RET C
        void opD9(); // RETI
        void opDA(); // JP C,u16
        void opDB(); // UNUSED
        void opDC(); // CALL C,u16
        void opDD(); // UNUSED
        void opDE(); // SBC A,u8
        void opDF(); // RST 18h

        //0xE0

        void opE0(); // LD (FF00+u8),A
        void opE1(); // POP HL
        void opE2(); // LD (FF00+C),A
        void opE3(); // UNUSED
        void opE4(); // UNUSED
        void opE5(); // PUSH HL
        void opE6(); // AND A,u8
        void opE7(); // RST 20h
        void opE8(); // ADD SP,i8
        void opE9(); // JP HL
        void opEA(); // LD (u16),A
        void opEB(); // UNUSED
        void opEC(); // UNUSED
        void opED(); // UNUSED
        void opEE(); // XOR A,u8
        void opEF(); // RST 28h

        //0xF0

        void opF0(); // LD A,(FF00+u8)
        void opF1(); // POP AF
        void opF2(); // LD A,(FF00+C)
        void opF3(); // DI
        void opF4(); // UNUSED
        void opF5(); // PUSH AF
        void opF6(); // OR A,u8
        void opF7(); // RST 30h
        void opF8(); // LD HL,SP+i8
        void opF9(); // LD SP,HL
        void opFA(); // LD A,(u16)
        void opFB(); // EI
        void opFC(); // UNUSED
        void opFD(); // UNUSED
        void opFE(); // CP A,u8
        void opFF(); // RST 38h

        //--------------------------------------Opcode Tables--------------------------------------//

        const Instruction instructions[0x100] = 
        {
            //0x00
            INSTRUCTION("NOP",              std::bind(&CPU::op00, this), 1,  4,  4),
            INSTRUCTION("LD BC,u16",        NULL, 3, 12, 12),
            INSTRUCTION("LD (BC),A",        NULL, 1,  8,  8),
            INSTRUCTION("INC BC",           NULL, 1,  8,  8),
            INSTRUCTION("INC B",            NULL, 1,  4,  4),
            INSTRUCTION("DEC B",            std::bind(&CPU::op05, this), 1,  4,  4),
            INSTRUCTION("LD B,u8",          std::bind(&CPU::op06, this), 2,  8,  8),
            INSTRUCTION("RLCA",             NULL, 1,  4,  4),
            INSTRUCTION("LD (u16),SP",      NULL, 3, 20, 20),
            INSTRUCTION("ADD HL,BC",        NULL, 1,  8,  8),
            INSTRUCTION("LD A,(BC)",        NULL, 1,  8,  8),
            INSTRUCTION("DEC BC",           NULL, 1,  8,  8),
            INSTRUCTION("INC C",            NULL, 1,  4,  4),
            INSTRUCTION("DEC C",            std::bind(&CPU::op0D, this), 1,  4,  4),
            INSTRUCTION("LD C,u8",          std::bind(&CPU::op0E, this), 2,  8,  8),
            INSTRUCTION("RRCA",             NULL, 1,  4,  4),

            //0x10
            INSTRUCTION("STOP",             NULL, 2,  4,  4),
            INSTRUCTION("LD DE,u16",        NULL, 3, 12, 12),
            INSTRUCTION("LD (DE),A",        NULL, 1,  8,  8),
            INSTRUCTION("INC DE",           NULL, 1,  8,  8),
            INSTRUCTION("INC D",            NULL, 1,  4,  4),
            INSTRUCTION("DEC D",            NULL, 1,  4,  4),
            INSTRUCTION("LD D,u8",          NULL, 2,  8,  8),
            INSTRUCTION("RLA",              NULL, 1,  4,  4),
            INSTRUCTION("JR i8",            NULL, 2, 12, 12),
            INSTRUCTION("ADD HL,DE",        NULL, 1,  8,  8),
            INSTRUCTION("LD A,(DE)",        NULL, 1,  8,  8),
            INSTRUCTION("DEC DE",           NULL, 1,  8,  8),
            INSTRUCTION("INC E",            NULL, 1,  4,  4),
            INSTRUCTION("DEC E",            NULL, 1,  4,  4),
            INSTRUCTION("LD E,u8",          NULL, 2,  8,  8),
            INSTRUCTION("RRA",              NULL, 1,  4,  4),

            //0x20
            INSTRUCTION("JR NZ,i8",         std::bind(&CPU::op20, this), 2, 12,  8),
            INSTRUCTION("LD HL,u16",        std::bind(&CPU::op21, this), 3, 12, 12),
            INSTRUCTION("LD (HL+),A",       NULL, 1,  8,  8),
            INSTRUCTION("INC HL",           NULL, 1,  8,  8),
            INSTRUCTION("INC H",            NULL, 1,  4,  4),
            INSTRUCTION("DEC H",            NULL, 1,  4,  4),
            INSTRUCTION("LD H,u8",          NULL, 2,  8,  8),
            INSTRUCTION("DAA",              NULL, 1,  4,  4),
            INSTRUCTION("JR Z,i8",          NULL, 2, 12,  8),
            INSTRUCTION("ADD HL,HL",        NULL, 1,  8,  8),
            INSTRUCTION("LD A,(HL+)",       NULL, 1,  8,  8),
            INSTRUCTION("DEC HL",           NULL, 1,  8,  8),
            INSTRUCTION("INC L",            NULL, 1,  4,  4),
            INSTRUCTION("DEC L",            NULL, 1,  4,  4),
            INSTRUCTION("LD L,u8",          NULL, 2,  8,  8),
            INSTRUCTION("CPL",              NULL, 1,  4,  4),

            //0x30
            INSTRUCTION("JR NC,i8",         NULL, 2, 12,  8),
            INSTRUCTION("LD SP,u16",        NULL, 3, 12, 12),
            INSTRUCTION("LD (HL-),A",       std::bind(&CPU::op32, this), 1,  8,  8),
            INSTRUCTION("INC SP",           NULL, 1,  8,  8),
            INSTRUCTION("INC (HL)",         NULL, 1, 12, 12),
            INSTRUCTION("DEC (HL)",         NULL, 1, 12, 12),
            INSTRUCTION("LD (HL),u8",       NULL, 2, 12, 12),
            INSTRUCTION("SCF",              NULL, 1,  4,  4),
            INSTRUCTION("JR C,i8",          NULL, 2, 12,  8),
            INSTRUCTION("ADD HL,SP",        NULL, 1,  8,  8),
            INSTRUCTION("LD A,(HL-)",       NULL, 1,  8,  8),
            INSTRUCTION("DEC SP",           NULL, 1,  8,  8),
            INSTRUCTION("INC A",            NULL, 1,  4,  4),
            INSTRUCTION("DEC A",            NULL, 1,  4,  4),
            INSTRUCTION("LD A,u8",          std::bind(&CPU::op3E, this), 2,  8,  8),
            INSTRUCTION("CCF",              NULL, 1,  4,  4),

            //0x40
            INSTRUCTION("LD B,B",           NULL, 1,  4,  4),
            INSTRUCTION("LD B,C",           NULL, 1,  4,  4),
            INSTRUCTION("LD B,D",           NULL, 1,  4,  4),
            INSTRUCTION("LD B,E",           NULL, 1,  4,  4),
            INSTRUCTION("LD B,H",           NULL, 1,  4,  4),
            INSTRUCTION("LD B,L",           NULL, 1,  4,  4),
            INSTRUCTION("LD B,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("LD B,A",           NULL, 1,  4,  4),
            INSTRUCTION("LD C,B",           NULL, 1,  4,  4),
            INSTRUCTION("LD C,C",           NULL, 1,  4,  4),
            INSTRUCTION("LD C,D",           NULL, 1,  4,  4),
            INSTRUCTION("LD C,E",           NULL, 1,  4,  4),
            INSTRUCTION("LD C,H",           NULL, 1,  4,  4),
            INSTRUCTION("LD C,L",           NULL, 1,  4,  4),
            INSTRUCTION("LD C,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("LD C,A",           NULL, 1,  4,  4),

            //0x50
            INSTRUCTION("LD D,B",           NULL, 1,  4,  4),
            INSTRUCTION("LD D,C",           NULL, 1,  4,  4),
            INSTRUCTION("LD D,D",           NULL, 1,  4,  4),
            INSTRUCTION("LD D,E",           NULL, 1,  4,  4),
            INSTRUCTION("LD D,H",           NULL, 1,  4,  4),
            INSTRUCTION("LD D,L",           NULL, 1,  4,  4),
            INSTRUCTION("LD D,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("LD D,A",           NULL, 1,  4,  4),
            INSTRUCTION("LD E,B",           NULL, 1,  4,  4),
            INSTRUCTION("LD E,C",           NULL, 1,  4,  4),
            INSTRUCTION("LD E,D",           NULL, 1,  4,  4),
            INSTRUCTION("LD E,E",           NULL, 1,  4,  4),
            INSTRUCTION("LD E,H",           NULL, 1,  4,  4),
            INSTRUCTION("LD E,L",           NULL, 1,  4,  4),
            INSTRUCTION("LD E,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("LD E,A",           NULL, 1,  4,  4),

            //0x60
            INSTRUCTION("LD H,B",           NULL, 1,  4,  4),
            INSTRUCTION("LD H,C",           NULL, 1,  4,  4),
            INSTRUCTION("LD H,D",           NULL, 1,  4,  4),
            INSTRUCTION("LD H,E",           NULL, 1,  4,  4),
            INSTRUCTION("LD H,H",           NULL, 1,  4,  4),
            INSTRUCTION("LD H,L",           NULL, 1,  4,  4),
            INSTRUCTION("LD H,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("LD H,A",           NULL, 1,  4,  4),
            INSTRUCTION("LD L,B",           NULL, 1,  4,  4),
            INSTRUCTION("LD L,C",           NULL, 1,  4,  4),
            INSTRUCTION("LD L,D",           NULL, 1,  4,  4),
            INSTRUCTION("LD L,E",           NULL, 1,  4,  4),
            INSTRUCTION("LD L,H",           NULL, 1,  4,  4),
            INSTRUCTION("LD L,L",           NULL, 1,  4,  4),
            INSTRUCTION("LD L,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("LD L,A",           NULL, 1,  4,  4),

            //0x70
            INSTRUCTION("LD (HL),B",        NULL, 1,  8,  8),
            INSTRUCTION("LD (HL),C",        NULL, 1,  8,  8),
            INSTRUCTION("LD (HL),D",        NULL, 1,  8,  8),
            INSTRUCTION("LD (HL),E",        NULL, 1,  8,  8),
            INSTRUCTION("LD (HL),H",        NULL, 1,  8,  8),
            INSTRUCTION("LD (HL),L",        NULL, 1,  8,  8),
            INSTRUCTION("HALT",             NULL, 1,  4,  4),
            INSTRUCTION("LD (HL),A",        NULL, 1,  8,  8),
            INSTRUCTION("LD A,B",           NULL, 1,  4,  4),
            INSTRUCTION("LD A,C",           NULL, 1,  4,  4),
            INSTRUCTION("LD A,D",           NULL, 1,  4,  4),
            INSTRUCTION("LD A,E",           NULL, 1,  4,  4),
            INSTRUCTION("LD A,H",           NULL, 1,  4,  4),
            INSTRUCTION("LD A,L",           NULL, 1,  4,  4),
            INSTRUCTION("LD A,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("LD A,A",           NULL, 1,  4,  4),

            //0x80  
            INSTRUCTION("ADD A,B",          NULL, 1,  4,  4),
            INSTRUCTION("ADD A,C",          NULL, 1,  4,  4),
            INSTRUCTION("ADD A,D",          NULL, 1,  4,  4),
            INSTRUCTION("ADD A,E",          NULL, 1,  4,  4),
            INSTRUCTION("ADD A,H",          NULL, 1,  4,  4),
            INSTRUCTION("ADD A,L",          NULL, 1,  4,  4),
            INSTRUCTION("ADD A,(HL)",       NULL, 1,  8,  8),
            INSTRUCTION("ADD A,A",          NULL, 1,  4,  4),
            INSTRUCTION("ADC A,B",          NULL, 1,  4,  4),
            INSTRUCTION("ADC A,C",          NULL, 1,  4,  4),
            INSTRUCTION("ADC A,D",          NULL, 1,  4,  4),
            INSTRUCTION("ADC A,E",          NULL, 1,  4,  4),
            INSTRUCTION("ADC A,H",          NULL, 1,  4,  4),
            INSTRUCTION("ADC A,L",          NULL, 1,  4,  4),
            INSTRUCTION("ADC A,(HL)",       NULL, 1,  8,  8),
            INSTRUCTION("ADC A,A",          NULL, 1,  4,  4),

            //0x90
            INSTRUCTION("SUB A,B",          NULL, 1,  4,  4),
            INSTRUCTION("SUB A,C",          NULL, 1,  4,  4),
            INSTRUCTION("SUB A,D",          NULL, 1,  4,  4),
            INSTRUCTION("SUB A,E",          NULL, 1,  4,  4),
            INSTRUCTION("SUB A,H",          NULL, 1,  4,  4),
            INSTRUCTION("SUB A,L",          NULL, 1,  4,  4),
            INSTRUCTION("SUB A,(HL)",       NULL, 1,  8,  8),
            INSTRUCTION("SUB A,A",          NULL, 1,  4,  4),
            INSTRUCTION("SBC A,B",          NULL, 1,  4,  4),
            INSTRUCTION("SBC A,C",          NULL, 1,  4,  4),
            INSTRUCTION("SBC A,D",          NULL, 1,  4,  4),
            INSTRUCTION("SBC A,E",          NULL, 1,  4,  4),
            INSTRUCTION("SBC A,H",          NULL, 1,  4,  4),
            INSTRUCTION("SBC A,L",          NULL, 1,  4,  4),
            INSTRUCTION("SBC A,(HL)",       NULL, 1,  8,  8),
            INSTRUCTION("SBC A,A",          NULL, 1,  4,  4),

            //0xA0
            INSTRUCTION("AND A,B",          NULL, 1,  4,  4),
            INSTRUCTION("AND A,C",          NULL, 1,  4,  4),
            INSTRUCTION("AND A,D",          NULL, 1,  4,  4),
            INSTRUCTION("AND A,E",          NULL, 1,  4,  4),
            INSTRUCTION("AND A,H",          NULL, 1,  4,  4),
            INSTRUCTION("AND A,L",          NULL, 1,  4,  4),
            INSTRUCTION("AND A,(HL)",       NULL, 1,  8,  8),
            INSTRUCTION("AND A,A",          NULL, 1,  4,  4),
            INSTRUCTION("XOR A,B",          NULL, 1,  4,  4),
            INSTRUCTION("XOR A,C",          NULL, 1,  4,  4),
            INSTRUCTION("XOR A,D",          NULL, 1,  4,  4),
            INSTRUCTION("XOR A,E",          NULL, 1,  4,  4),
            INSTRUCTION("XOR A,H",          NULL, 1,  4,  4),
            INSTRUCTION("XOR A,L",          NULL, 1,  4,  4),
            INSTRUCTION("XOR A,(HL)",       NULL, 1,  8,  8),
            INSTRUCTION("XOR A,A",          std::bind(&CPU::opAF, this), 1,  4,  4),

            //0xB0
            INSTRUCTION("OR A,B",           NULL, 1,  4,  4),
            INSTRUCTION("OR A,C",           NULL, 1,  4,  4),
            INSTRUCTION("OR A,D",           NULL, 1,  4,  4),
            INSTRUCTION("OR A,E",           NULL, 1,  4,  4),
            INSTRUCTION("OR A,H",           NULL, 1,  4,  4),
            INSTRUCTION("OR A,L",           NULL, 1,  4,  4),
            INSTRUCTION("OR A,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("OR A,A",           NULL, 1,  4,  4),
            INSTRUCTION("CP A,B",           NULL, 1,  4,  4),
            INSTRUCTION("CP A,C",           NULL, 1,  4,  4),
            INSTRUCTION("CP A,D",           NULL, 1,  4,  4),
            INSTRUCTION("CP A,E",           NULL, 1,  4,  4),
            INSTRUCTION("CP A,H",           NULL, 1,  4,  4),
            INSTRUCTION("CP A,L",           NULL, 1,  4,  4),
            INSTRUCTION("CP A,(HL)",        NULL, 1,  8,  8),
            INSTRUCTION("CP A,A",           NULL, 1,  4,  4),

            //0xC0
            INSTRUCTION("RET NZ",           NULL, 1, 20,  8),
            INSTRUCTION("POP BC",           NULL, 1, 12, 12),
            INSTRUCTION("JP NZ,u16",        NULL, 3, 16, 12),
            INSTRUCTION("JP u16",           std::bind(&CPU::opC3, this), 3, 16, 16),
            INSTRUCTION("CALL NZ,u16",      NULL, 3, 24, 12),
            INSTRUCTION("PUSH BC",          NULL, 1, 16, 16),
            INSTRUCTION("ADD A,u8",         NULL, 2,  8,  8),
            INSTRUCTION("RST 00h",          NULL, 1, 16, 16),
            INSTRUCTION("RET Z",            NULL, 1, 20,  8),
            INSTRUCTION("RET",              NULL, 1, 16, 16),
            INSTRUCTION("JP Z,u16",         NULL, 3, 16, 12),
            INSTRUCTION("PREFIX CB",        NULL, 1,  4,  4),
            INSTRUCTION("CALL Z,u16",       NULL, 3, 24, 12),
            INSTRUCTION("CALL u16",         NULL, 3, 24, 24),
            INSTRUCTION("ADC A,u8",         NULL, 2,  8,  8),
            INSTRUCTION("RST 08h",          NULL, 1, 16, 16),

            //0xD0
            INSTRUCTION("RET NC",           NULL, 1, 20,  8),
            INSTRUCTION("POP DE",           NULL, 1, 12, 12),
            INSTRUCTION("JP NC,u16",        NULL, 3, 16, 12),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("CALL NC,u16",      NULL, 3, 24, 12),
            INSTRUCTION("PUSH DE",          NULL, 1, 16, 16),
            INSTRUCTION("SUB A,u8",         NULL, 2,  8,  8),
            INSTRUCTION("RST 10h",          NULL, 1, 16, 16),
            INSTRUCTION("RET C",            NULL, 1, 20,  8),
            INSTRUCTION("RETI",             NULL, 1, 16, 16),
            INSTRUCTION("JP C,u16",         NULL, 3, 16, 12),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("CALL C,u16",       NULL, 3, 24, 12),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("SBC A,u8",         NULL, 2,  8,  8),
            INSTRUCTION("RST 18h",          NULL, 1, 16, 16),

            //0xE0
            INSTRUCTION("LD (FF00+u8),A",   std::bind(&CPU::opE0, this), 2, 12, 12),
            INSTRUCTION("POP HL",           NULL, 1, 12, 12),
            INSTRUCTION("LD (FF00+C),A",    NULL, 1,  8,  8),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("PUSH HL",          NULL, 1, 16, 16),
            INSTRUCTION("AND A,u8",         NULL, 2,  8,  8),
            INSTRUCTION("RST 20h",          NULL, 1, 16, 16),
            INSTRUCTION("ADD SP,i8",        NULL, 2, 16, 16),
            INSTRUCTION("JP HL",            NULL, 1,  4,  4),
            INSTRUCTION("LD (u16),A",       NULL, 3, 16, 16),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("XOR A,u8",         NULL, 2,  8,  8),
            INSTRUCTION("RST 28h",          NULL, 1, 16, 16),

            //0xF0
            INSTRUCTION("LD A,(FF00+u8)",   std::bind(&CPU::opF0, this), 2, 12, 12),
            INSTRUCTION("POP AF",           NULL, 1, 12, 12),
            INSTRUCTION("LD A,(FF00+C)",    NULL, 1,  8,  8),
            INSTRUCTION("DI",               std::bind(&CPU::opF3, this), 1,  4,  4),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("PUSH AF",          NULL, 1, 16, 16),
            INSTRUCTION("OR A,u8",          NULL, 2,  8,  8),
            INSTRUCTION("RST 30h",          NULL, 1, 16, 16),
            INSTRUCTION("LD HL,SP+i8",      NULL, 2, 12, 12),
            INSTRUCTION("LD SP,HL",         NULL, 1,  8,  8),
            INSTRUCTION("LD A,(u16)",       NULL, 3, 16, 16),
            INSTRUCTION("EI",               std::bind(&CPU::opFB, this), 1,  4,  4),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("UNUSED",           NULL, 1,  0,  0),
            INSTRUCTION("CP A,u8",          std::bind(&CPU::opFE, this), 2,  8,  8),
            INSTRUCTION("RST 38h",          NULL, 1, 16, 16)
        };
        
        const Instruction instructionsCB[0x100] = 
        {
            //0x0
            INSTRUCTION("RLC B",            NULL, 2,  8,  8),
            INSTRUCTION("RLC C",            NULL, 2,  8,  8),
            INSTRUCTION("RLC D",            NULL, 2,  8,  8),
            INSTRUCTION("RLC E",            NULL, 2,  8,  8),
            INSTRUCTION("RLC H",            NULL, 2,  8,  8),
            INSTRUCTION("RLC L",            NULL, 2,  8,  8),
            INSTRUCTION("RLC (HL)",         NULL, 2, 16, 16),
            INSTRUCTION("RLC A",            NULL, 2,  8,  8),
            INSTRUCTION("RRC B",            NULL, 2,  8,  8),
            INSTRUCTION("RRC C",            NULL, 2,  8,  8),
            INSTRUCTION("RRC D",            NULL, 2,  8,  8),
            INSTRUCTION("RRC E",            NULL, 2,  8,  8),
            INSTRUCTION("RRC H",            NULL, 2,  8,  8),
            INSTRUCTION("RRC L",            NULL, 2,  8,  8),
            INSTRUCTION("RRC (HL)",         NULL, 2, 16, 16),
            INSTRUCTION("RRC A",            NULL, 2,  8,  8),

            //0x10
            INSTRUCTION("RL B",             NULL, 2,  8,  8),
            INSTRUCTION("RL C",             NULL, 2,  8,  8),
            INSTRUCTION("RL D",             NULL, 2,  8,  8),
            INSTRUCTION("RL E",             NULL, 2,  8,  8),
            INSTRUCTION("RL H",             NULL, 2,  8,  8),
            INSTRUCTION("RL L",             NULL, 2,  8,  8),
            INSTRUCTION("RL (HL)",          NULL, 2, 16, 16),
            INSTRUCTION("RL A",             NULL, 2,  8,  8),
            INSTRUCTION("RR B",             NULL, 2,  8,  8),
            INSTRUCTION("RR C",             NULL, 2,  8,  8),
            INSTRUCTION("RR D",             NULL, 2,  8,  8),
            INSTRUCTION("RR E",             NULL, 2,  8,  8),
            INSTRUCTION("RR H",             NULL, 2,  8,  8),
            INSTRUCTION("RR L",             NULL, 2,  8,  8),
            INSTRUCTION("RR (HL)",          NULL, 2, 16, 16),
            INSTRUCTION("RR A",             NULL, 2,  8,  8),

            //0x20
            INSTRUCTION("SLA B",            NULL, 2,  8,  8),
            INSTRUCTION("SLA C",            NULL, 2,  8,  8),
            INSTRUCTION("SLA D",            NULL, 2,  8,  8),
            INSTRUCTION("SLA E",            NULL, 2,  8,  8),
            INSTRUCTION("SLA H",            NULL, 2,  8,  8),
            INSTRUCTION("SLA L",            NULL, 2,  8,  8),
            INSTRUCTION("SLA (HL)",         NULL, 2, 16, 16),
            INSTRUCTION("SLA A",            NULL, 2,  8,  8),
            INSTRUCTION("SRA B",            NULL, 2,  8,  8),
            INSTRUCTION("SRA C",            NULL, 2,  8,  8),
            INSTRUCTION("SRA D",            NULL, 2,  8,  8),
            INSTRUCTION("SRA E",            NULL, 2,  8,  8),
            INSTRUCTION("SRA H",            NULL, 2,  8,  8),
            INSTRUCTION("SRA L",            NULL, 2,  8,  8),
            INSTRUCTION("SRA (HL)",         NULL, 2, 16, 16),
            INSTRUCTION("SRA A",            NULL, 2,  8,  8),

            //0x30
            INSTRUCTION("SWAP B",           NULL, 2,  8,  8),
            INSTRUCTION("SWAP C",           NULL, 2,  8,  8),
            INSTRUCTION("SWAP D",           NULL, 2,  8,  8),
            INSTRUCTION("SWAP E",           NULL, 2,  8,  8),
            INSTRUCTION("SWAP H",           NULL, 2,  8,  8),
            INSTRUCTION("SWAP L",           NULL, 2,  8,  8),
            INSTRUCTION("SWAP (HL)",        NULL, 2, 16, 16),
            INSTRUCTION("SWAP A",           NULL, 2,  8,  8),
            INSTRUCTION("SRL B",            NULL, 2,  8,  8),
            INSTRUCTION("SRL C",            NULL, 2,  8,  8),
            INSTRUCTION("SRL D",            NULL, 2,  8,  8),
            INSTRUCTION("SRL E",            NULL, 2,  8,  8),
            INSTRUCTION("SRL H",            NULL, 2,  8,  8),
            INSTRUCTION("SRL L",            NULL, 2,  8,  8),
            INSTRUCTION("SRL (HL)",         NULL, 2, 16, 16),
            INSTRUCTION("SRL A",            NULL, 2,  8,  8),

            //0x40
            INSTRUCTION("BIT 0,B",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 0,C",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 0,D",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 0,E",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 0,H",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 0,L",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 0,(HL)",       NULL, 2, 12, 12),
            INSTRUCTION("BIT 0,A",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 1,B",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 1,C",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 1,D",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 1,E",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 1,H",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 1,L",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 1,(HL)",       NULL, 2, 12, 12),
            INSTRUCTION("BIT 1,A",          NULL, 2,  8,  8),

            //0x50
            INSTRUCTION("BIT 2,B",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 2,C",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 2,D",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 2,E",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 2,H",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 2,L",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 2,(HL)",       NULL, 2, 12, 12),
            INSTRUCTION("BIT 2,A",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 3,B",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 3,C",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 3,D",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 3,E",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 3,H",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 3,L",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 3,(HL)",       NULL, 2, 12, 12),
            INSTRUCTION("BIT 3,A",          NULL, 2,  8,  8),

            //0x60
            INSTRUCTION("BIT 4,B",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 4,C",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 4,D",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 4,E",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 4,H",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 4,L",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 4,(HL)",       NULL, 2, 12, 12),
            INSTRUCTION("BIT 4,A",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 5,B",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 5,C",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 5,D",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 5,E",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 5,H",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 5,L",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 5,(HL)",       NULL, 2, 12, 12),
            INSTRUCTION("BIT 5,A",          NULL, 2,  8,  8),

            //0x70
            INSTRUCTION("BIT 6,B",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 6,C",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 6,D",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 6,E",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 6,H",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 6,L",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 6,(HL)",       NULL, 2, 12, 12),
            INSTRUCTION("BIT 6,A",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 7,B",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 7,C",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 7,D",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 7,E",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 7,H",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 7,L",          NULL, 2,  8,  8),
            INSTRUCTION("BIT 7,(HL)",       NULL, 2, 12, 12),
            INSTRUCTION("BIT 7,A",          NULL, 2,  8,  8),

            //0x80
            INSTRUCTION("RES 0,B",          NULL, 2,  8,  8),
            INSTRUCTION("RES 0,C",          NULL, 2,  8,  8),
            INSTRUCTION("RES 0,D",          NULL, 2,  8,  8),
            INSTRUCTION("RES 0,E",          NULL, 2,  8,  8),
            INSTRUCTION("RES 0,H",          NULL, 2,  8,  8),
            INSTRUCTION("RES 0,L",          NULL, 2,  8,  8),
            INSTRUCTION("RES 0,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("RES 0,A",          NULL, 2,  8,  8),
            INSTRUCTION("RES 1,B",          NULL, 2,  8,  8),
            INSTRUCTION("RES 1,C",          NULL, 2,  8,  8),
            INSTRUCTION("RES 1,D",          NULL, 2,  8,  8),
            INSTRUCTION("RES 1,E",          NULL, 2,  8,  8),
            INSTRUCTION("RES 1,H",          NULL, 2,  8,  8),
            INSTRUCTION("RES 1,L",          NULL, 2,  8,  8),
            INSTRUCTION("RES 1,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("RES 1,A",          NULL, 2,  8,  8),

            //0x90
            INSTRUCTION("RES 2,B",          NULL, 2,  8,  8),
            INSTRUCTION("RES 2,C",          NULL, 2,  8,  8),
            INSTRUCTION("RES 2,D",          NULL, 2,  8,  8),
            INSTRUCTION("RES 2,E",          NULL, 2,  8,  8),
            INSTRUCTION("RES 2,H",          NULL, 2,  8,  8),
            INSTRUCTION("RES 2,L",          NULL, 2,  8,  8),
            INSTRUCTION("RES 2,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("RES 2,A",          NULL, 2,  8,  8),
            INSTRUCTION("RES 3,B",          NULL, 2,  8,  8),
            INSTRUCTION("RES 3,C",          NULL, 2,  8,  8),
            INSTRUCTION("RES 3,D",          NULL, 2,  8,  8),
            INSTRUCTION("RES 3,E",          NULL, 2,  8,  8),
            INSTRUCTION("RES 3,H",          NULL, 2,  8,  8),
            INSTRUCTION("RES 3,L",          NULL, 2,  8,  8),
            INSTRUCTION("RES 3,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("RES 3,A",          NULL, 2,  8,  8),

            //0xA0
            INSTRUCTION("RES 4,B",          NULL, 2,  8,  8),
            INSTRUCTION("RES 4,C",          NULL, 2,  8,  8),
            INSTRUCTION("RES 4,D",          NULL, 2,  8,  8),
            INSTRUCTION("RES 4,E",          NULL, 2,  8,  8),
            INSTRUCTION("RES 4,H",          NULL, 2,  8,  8),
            INSTRUCTION("RES 4,L",          NULL, 2,  8,  8),
            INSTRUCTION("RES 4,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("RES 4,A",          NULL, 2,  8,  8),
            INSTRUCTION("RES 5,B",          NULL, 2,  8,  8),
            INSTRUCTION("RES 5,C",          NULL, 2,  8,  8),
            INSTRUCTION("RES 5,D",          NULL, 2,  8,  8),
            INSTRUCTION("RES 5,E",          NULL, 2,  8,  8),
            INSTRUCTION("RES 5,H",          NULL, 2,  8,  8),
            INSTRUCTION("RES 5,L",          NULL, 2,  8,  8),
            INSTRUCTION("RES 5,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("RES 5,A",          NULL, 2,  8,  8),

            //0xB0
            INSTRUCTION("RES 6,B",          NULL, 2,  8,  8),
            INSTRUCTION("RES 6,C",          NULL, 2,  8,  8),
            INSTRUCTION("RES 6,D",          NULL, 2,  8,  8),
            INSTRUCTION("RES 6,E",          NULL, 2,  8,  8),
            INSTRUCTION("RES 6,H",          NULL, 2,  8,  8),
            INSTRUCTION("RES 6,L",          NULL, 2,  8,  8),
            INSTRUCTION("RES 6,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("RES 6,A",          NULL, 2,  8,  8),
            INSTRUCTION("RES 7,B",          NULL, 2,  8,  8),
            INSTRUCTION("RES 7,C",          NULL, 2,  8,  8),
            INSTRUCTION("RES 7,D",          NULL, 2,  8,  8),
            INSTRUCTION("RES 7,E",          NULL, 2,  8,  8),
            INSTRUCTION("RES 7,H",          NULL, 2,  8,  8),
            INSTRUCTION("RES 7,L",          NULL, 2,  8,  8),
            INSTRUCTION("RES 7,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("RES 7,A",          NULL, 2,  8,  8),

            //0xC0
            INSTRUCTION("SET 0,B",          NULL, 2,  8,  8),
            INSTRUCTION("SET 0,C",          NULL, 2,  8,  8),
            INSTRUCTION("SET 0,D",          NULL, 2,  8,  8),
            INSTRUCTION("SET 0,E",          NULL, 2,  8,  8),
            INSTRUCTION("SET 0,H",          NULL, 2,  8,  8),
            INSTRUCTION("SET 0,L",          NULL, 2,  8,  8),
            INSTRUCTION("SET 0,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("SET 0,A",          NULL, 2,  8,  8),
            INSTRUCTION("SET 1,B",          NULL, 2,  8,  8),
            INSTRUCTION("SET 1,C",          NULL, 2,  8,  8),
            INSTRUCTION("SET 1,D",          NULL, 2,  8,  8),
            INSTRUCTION("SET 1,E",          NULL, 2,  8,  8),
            INSTRUCTION("SET 1,H",          NULL, 2,  8,  8),
            INSTRUCTION("SET 1,L",          NULL, 2,  8,  8),
            INSTRUCTION("SET 1,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("SET 1,A",          NULL, 2,  8,  8),

            //0xD0
            INSTRUCTION("SET 2,B",          NULL, 2,  8,  8),
            INSTRUCTION("SET 2,C",          NULL, 2,  8,  8),
            INSTRUCTION("SET 2,D",          NULL, 2,  8,  8),
            INSTRUCTION("SET 2,E",          NULL, 2,  8,  8),
            INSTRUCTION("SET 2,H",          NULL, 2,  8,  8),
            INSTRUCTION("SET 2,L",          NULL, 2,  8,  8),
            INSTRUCTION("SET 2,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("SET 2,A",          NULL, 2,  8,  8),
            INSTRUCTION("SET 3,B",          NULL, 2,  8,  8),
            INSTRUCTION("SET 3,C",          NULL, 2,  8,  8),
            INSTRUCTION("SET 3,D",          NULL, 2,  8,  8),
            INSTRUCTION("SET 3,E",          NULL, 2,  8,  8),
            INSTRUCTION("SET 3,H",          NULL, 2,  8,  8),
            INSTRUCTION("SET 3,L",          NULL, 2,  8,  8),
            INSTRUCTION("SET 3,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("SET 3,A",          NULL, 2,  8,  8),

            //0xE0
            INSTRUCTION("SET 4,B",          NULL, 2,  8,  8),
            INSTRUCTION("SET 4,C",          NULL, 2,  8,  8),
            INSTRUCTION("SET 4,D",          NULL, 2,  8,  8),
            INSTRUCTION("SET 4,E",          NULL, 2,  8,  8),
            INSTRUCTION("SET 4,H",          NULL, 2,  8,  8),
            INSTRUCTION("SET 4,L",          NULL, 2,  8,  8),
            INSTRUCTION("SET 4,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("SET 4,A",          NULL, 2,  8,  8),
            INSTRUCTION("SET 5,B",          NULL, 2,  8,  8),
            INSTRUCTION("SET 5,C",          NULL, 2,  8,  8),
            INSTRUCTION("SET 5,D",          NULL, 2,  8,  8),
            INSTRUCTION("SET 5,E",          NULL, 2,  8,  8),
            INSTRUCTION("SET 5,H",          NULL, 2,  8,  8),
            INSTRUCTION("SET 5,L",          NULL, 2,  8,  8),
            INSTRUCTION("SET 5,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("SET 5,A",          NULL, 2,  8,  8),

            //0xF0
            INSTRUCTION("SET 6,B",          NULL, 2,  8,  8),
            INSTRUCTION("SET 6,C",          NULL, 2,  8,  8),
            INSTRUCTION("SET 6,D",          NULL, 2,  8,  8),
            INSTRUCTION("SET 6,E",          NULL, 2,  8,  8),
            INSTRUCTION("SET 6,H",          NULL, 2,  8,  8),
            INSTRUCTION("SET 6,L",          NULL, 2,  8,  8),
            INSTRUCTION("SET 6,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("SET 6,A",          NULL, 2,  8,  8),
            INSTRUCTION("SET 7,B",          NULL, 2,  8,  8),
            INSTRUCTION("SET 7,C",          NULL, 2,  8,  8),
            INSTRUCTION("SET 7,D",          NULL, 2,  8,  8),
            INSTRUCTION("SET 7,E",          NULL, 2,  8,  8),
            INSTRUCTION("SET 7,H",          NULL, 2,  8,  8),
            INSTRUCTION("SET 7,L",          NULL, 2,  8,  8),
            INSTRUCTION("SET 7,(HL)",       NULL, 2, 16, 16),
            INSTRUCTION("SET 7,A",          NULL, 2,  8,  8)
        };
};