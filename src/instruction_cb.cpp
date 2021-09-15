#include "instruction.h"
#include "cpu.h"
#include "mmu.h"

//--------------------------------------CB Opcode Helpers--------------------------------------//

//--------------------------------------CB Opcodes--------------------------------------//

//0x00

void CPU::opcodeCB0x00() // RLC B
{
	
}

void CPU::opcodeCB0x01() // RLC C
{
	
}

void CPU::opcodeCB0x02() // RLC D
{
	
}

void CPU::opcodeCB0x03() // RLC E
{
	
}

void CPU::opcodeCB0x04() // RLC H
{
	
}

void CPU::opcodeCB0x05() // RLC L
{
	
}

void CPU::opcodeCB0x06() // RLC (HL)
{
	
}

void CPU::opcodeCB0x07() // RLC A
{
	
}

void CPU::opcodeCB0x08() // RRC B
{
	
}

void CPU::opcodeCB0x09() // RRC C
{
	
}

void CPU::opcodeCB0x0A() // RRC D
{
	
}

void CPU::opcodeCB0x0B() // RRC E
{
	
}

void CPU::opcodeCB0x0C() // RRC H
{
	
}

void CPU::opcodeCB0x0D() // RRC L
{
	
}

void CPU::opcodeCB0x0E() // RRC (HL)
{
	
}

void CPU::opcodeCB0x0F() // RRC A
{
	
}

//0x10

void CPU::opcodeCB0x10() // RL B
{
	
}

void CPU::opcodeCB0x11() // RL C
{
	
}

void CPU::opcodeCB0x12() // RL D
{
	
}

void CPU::opcodeCB0x13() // RL E
{
	
}

void CPU::opcodeCB0x14() // RL H
{
	
}

void CPU::opcodeCB0x15() // RL L
{
	
}

void CPU::opcodeCB0x16() // RL (HL)
{
	
}

void CPU::opcodeCB0x17() // RL A
{
	
}

void CPU::opcodeCB0x18() // RR B
{
	
}

void CPU::opcodeCB0x19() // RR C
{
	
}

void CPU::opcodeCB0x1A() // RR D
{
	
}

void CPU::opcodeCB0x1B() // RR E
{
	
}

void CPU::opcodeCB0x1C() // RR H
{
	
}

void CPU::opcodeCB0x1D() // RR L
{
	
}

void CPU::opcodeCB0x1E() // RR (HL)
{
	
}

void CPU::opcodeCB0x1F() // RR A
{
	
}

//0x20

void CPU::opcodeCB0x20() // SLA B
{
	
}

void CPU::opcodeCB0x21() // SLA C
{
	
}

void CPU::opcodeCB0x22() // SLA D
{
	
}

void CPU::opcodeCB0x23() // SLA E
{
	
}

void CPU::opcodeCB0x24() // SLA H
{
	
}

void CPU::opcodeCB0x25() // SLA L
{
	
}

void CPU::opcodeCB0x26() // SLA (HL)
{
	
}

void CPU::opcodeCB0x27() // SLA A
{
	
}

void CPU::opcodeCB0x28() // SRA B
{
	
}

void CPU::opcodeCB0x29() // SRA C
{
	
}

void CPU::opcodeCB0x2A() // SRA D
{
	
}

void CPU::opcodeCB0x2B() // SRA E
{
	
}

void CPU::opcodeCB0x2C() // SRA H
{
	
}

void CPU::opcodeCB0x2D() // SRA L
{
	
}

void CPU::opcodeCB0x2E() // SRA (HL)
{
	
}

void CPU::opcodeCB0x2F() // SRA A
{
	
}

//0x30

void CPU::opcodeCB0x30() // SWAP B
{
	
}

void CPU::opcodeCB0x31() // SWAP C
{
	
}

void CPU::opcodeCB0x32() // SWAP D
{
	
}

void CPU::opcodeCB0x33() // SWAP E
{
	
}

void CPU::opcodeCB0x34() // SWAP H
{
	
}

void CPU::opcodeCB0x35() // SWAP L
{
	
}

void CPU::opcodeCB0x36() // SWAP (HL)
{
	
}

void CPU::opcodeCB0x37() // SWAP A
{
	
}

void CPU::opcodeCB0x38() // SRL B
{
	
}

void CPU::opcodeCB0x39() // SRL C
{
	
}

void CPU::opcodeCB0x3A() // SRL D
{
	
}

void CPU::opcodeCB0x3B() // SRL E
{
	
}

void CPU::opcodeCB0x3C() // SRL H
{
	
}

void CPU::opcodeCB0x3D() // SRL L
{
	
}

void CPU::opcodeCB0x3E() // SRL (HL)
{
	
}

void CPU::opcodeCB0x3F() // SRL A
{
	
}

//0x40

void CPU::opcodeCB0x40() // BIT 0,B
{
	
}

void CPU::opcodeCB0x41() // BIT 0,C
{
	
}

void CPU::opcodeCB0x42() // BIT 0,D
{
	
}

void CPU::opcodeCB0x43() // BIT 0,E
{
	
}

void CPU::opcodeCB0x44() // BIT 0,H
{
	
}

void CPU::opcodeCB0x45() // BIT 0,L
{
	
}

void CPU::opcodeCB0x46() // BIT 0,(HL)
{
	
}

void CPU::opcodeCB0x47() // BIT 0,A
{
	
}

void CPU::opcodeCB0x48() // BIT 1,B
{
	
}

void CPU::opcodeCB0x49() // BIT 1,C
{
	
}

void CPU::opcodeCB0x4A() // BIT 1,D
{
	
}

void CPU::opcodeCB0x4B() // BIT 1,E
{
	
}

void CPU::opcodeCB0x4C() // BIT 1,H
{
	
}

void CPU::opcodeCB0x4D() // BIT 1,L
{
	
}

void CPU::opcodeCB0x4E() // BIT 1,(HL)
{
	
}

void CPU::opcodeCB0x4F() // BIT 1,A
{
	
}

//0x50

void CPU::opcodeCB0x50() // BIT 2,B
{
	
}

void CPU::opcodeCB0x51() // BIT 2,C
{
	
}

void CPU::opcodeCB0x52() // BIT 2,D
{
	
}

void CPU::opcodeCB0x53() // BIT 2,E
{
	
}

void CPU::opcodeCB0x54() // BIT 2,H
{
	
}

void CPU::opcodeCB0x55() // BIT 2,L
{
	
}

void CPU::opcodeCB0x56() // BIT 2,(HL)
{
	
}

void CPU::opcodeCB0x57() // BIT 2,A
{
	
}

void CPU::opcodeCB0x58() // BIT 3,B
{
	
}

void CPU::opcodeCB0x59() // BIT 3,C
{
	
}

void CPU::opcodeCB0x5A() // BIT 3,D
{
	
}

void CPU::opcodeCB0x5B() // BIT 3,E
{
	
}

void CPU::opcodeCB0x5C() // BIT 3,H
{
	
}

void CPU::opcodeCB0x5D() // BIT 3,L
{
	
}

void CPU::opcodeCB0x5E() // BIT 3,(HL)
{
	
}

void CPU::opcodeCB0x5F() // BIT 3,A
{
	
}

//0x60

void CPU::opcodeCB0x60() // BIT 4,B
{
	
}

void CPU::opcodeCB0x61() // BIT 4,C
{
	
}

void CPU::opcodeCB0x62() // BIT 4,D
{
	
}

void CPU::opcodeCB0x63() // BIT 4,E
{
	
}

void CPU::opcodeCB0x64() // BIT 4,H
{
	
}

void CPU::opcodeCB0x65() // BIT 4,L
{
	
}

void CPU::opcodeCB0x66() // BIT 4,(HL)
{
	
}

void CPU::opcodeCB0x67() // BIT 4,A
{
	
}

void CPU::opcodeCB0x68() // BIT 5,B
{
	
}

void CPU::opcodeCB0x69() // BIT 5,C
{
	
}

void CPU::opcodeCB0x6A() // BIT 5,D
{
	
}

void CPU::opcodeCB0x6B() // BIT 5,E
{
	
}

void CPU::opcodeCB0x6C() // BIT 5,H
{
	
}

void CPU::opcodeCB0x6D() // BIT 5,L
{
	
}

void CPU::opcodeCB0x6E() // BIT 5,(HL)
{
	
}

void CPU::opcodeCB0x6F() // BIT 5,A
{
	
}

//0x70

void CPU::opcodeCB0x70() // BIT 6,B
{
	
}

void CPU::opcodeCB0x71() // BIT 6,C
{
	
}

void CPU::opcodeCB0x72() // BIT 6,D
{
	
}

void CPU::opcodeCB0x73() // BIT 6,E
{
	
}

void CPU::opcodeCB0x74() // BIT 6,H
{
	
}

void CPU::opcodeCB0x75() // BIT 6,L
{
	
}

void CPU::opcodeCB0x76() // BIT 6,(HL)
{
	
}

void CPU::opcodeCB0x77() // BIT 6,A
{
	
}

void CPU::opcodeCB0x78() // BIT 7,B
{
	
}

void CPU::opcodeCB0x79() // BIT 7,C
{
	
}

void CPU::opcodeCB0x7A() // BIT 7,D
{
	
}

void CPU::opcodeCB0x7B() // BIT 7,E
{
	
}

void CPU::opcodeCB0x7C() // BIT 7,H
{
	
}

void CPU::opcodeCB0x7D() // BIT 7,L
{
	
}

void CPU::opcodeCB0x7E() // BIT 7,(HL)
{
	
}

void CPU::opcodeCB0x7F() // BIT 7,A
{
	
}

//0x80

void CPU::opcodeCB0x80() // RES 0,B
{
	
}

void CPU::opcodeCB0x81() // RES 0,C
{
	
}

void CPU::opcodeCB0x82() // RES 0,D
{
	
}

void CPU::opcodeCB0x83() // RES 0,E
{
	
}

void CPU::opcodeCB0x84() // RES 0,H
{
	
}

void CPU::opcodeCB0x85() // RES 0,L
{
	
}

void CPU::opcodeCB0x86() // RES 0,(HL)
{
	
}

void CPU::opcodeCB0x87() // RES 0,A
{
	
}

void CPU::opcodeCB0x88() // RES 1,B
{
	
}

void CPU::opcodeCB0x89() // RES 1,C
{
	
}

void CPU::opcodeCB0x8A() // RES 1,D
{
	
}

void CPU::opcodeCB0x8B() // RES 1,E
{
	
}

void CPU::opcodeCB0x8C() // RES 1,H
{
	
}

void CPU::opcodeCB0x8D() // RES 1,L
{
	
}

void CPU::opcodeCB0x8E() // RES 1,(HL)
{
	
}

void CPU::opcodeCB0x8F() // RES 1,A
{
	
}

//0x90

void CPU::opcodeCB0x90() // RES 2,B
{
	
}

void CPU::opcodeCB0x91() // RES 2,C
{
	
}

void CPU::opcodeCB0x92() // RES 2,D
{
	
}

void CPU::opcodeCB0x93() // RES 2,E
{
	
}

void CPU::opcodeCB0x94() // RES 2,H
{
	
}

void CPU::opcodeCB0x95() // RES 2,L
{
	
}

void CPU::opcodeCB0x96() // RES 2,(HL)
{
	
}

void CPU::opcodeCB0x97() // RES 2,A
{
	
}

void CPU::opcodeCB0x98() // RES 3,B
{
	
}

void CPU::opcodeCB0x99() // RES 3,C
{
	
}

void CPU::opcodeCB0x9A() // RES 3,D
{
	
}

void CPU::opcodeCB0x9B() // RES 3,E
{
	
}

void CPU::opcodeCB0x9C() // RES 3,H
{
	
}

void CPU::opcodeCB0x9D() // RES 3,L
{
	
}

void CPU::opcodeCB0x9E() // RES 3,(HL)
{
	
}

void CPU::opcodeCB0x9F() // RES 3,A
{
	
}

//0xA0

void CPU::opcodeCB0xA0() // RES 4,B
{
	
}

void CPU::opcodeCB0xA1() // RES 4,C
{
	
}

void CPU::opcodeCB0xA2() // RES 4,D
{
	
}

void CPU::opcodeCB0xA3() // RES 4,E
{
	
}

void CPU::opcodeCB0xA4() // RES 4,H
{
	
}

void CPU::opcodeCB0xA5() // RES 4,L
{
	
}

void CPU::opcodeCB0xA6() // RES 4,(HL)
{
	
}

void CPU::opcodeCB0xA7() // RES 4,A
{
	
}

void CPU::opcodeCB0xA8() // RES 5,B
{
	
}

void CPU::opcodeCB0xA9() // RES 5,C
{
	
}

void CPU::opcodeCB0xAA() // RES 5,D
{
	
}

void CPU::opcodeCB0xAB() // RES 5,E
{
	
}

void CPU::opcodeCB0xAC() // RES 5,H
{
	
}

void CPU::opcodeCB0xAD() // RES 5,L
{
	
}

void CPU::opcodeCB0xAE() // RES 5,(HL)
{
	
}

void CPU::opcodeCB0xAF() // RES 5,A
{
	
}

//0xB0

void CPU::opcodeCB0xB0() // RES 6,B
{
	
}

void CPU::opcodeCB0xB1() // RES 6,C
{
	
}

void CPU::opcodeCB0xB2() // RES 6,D
{
	
}

void CPU::opcodeCB0xB3() // RES 6,E
{
	
}

void CPU::opcodeCB0xB4() // RES 6,H
{
	
}

void CPU::opcodeCB0xB5() // RES 6,L
{
	
}

void CPU::opcodeCB0xB6() // RES 6,(HL)
{
	
}

void CPU::opcodeCB0xB7() // RES 6,A
{
	
}

void CPU::opcodeCB0xB8() // RES 7,B
{
	
}

void CPU::opcodeCB0xB9() // RES 7,C
{
	
}

void CPU::opcodeCB0xBA() // RES 7,D
{
	
}

void CPU::opcodeCB0xBB() // RES 7,E
{
	
}

void CPU::opcodeCB0xBC() // RES 7,H
{
	
}

void CPU::opcodeCB0xBD() // RES 7,L
{
	
}

void CPU::opcodeCB0xBE() // RES 7,(HL)
{
	
}

void CPU::opcodeCB0xBF() // RES 7,A
{
	
}

//0xC0

void CPU::opcodeCB0xC0() // SET 0,B
{
	
}

void CPU::opcodeCB0xC1() // SET 0,C
{
	
}

void CPU::opcodeCB0xC2() // SET 0,D
{
	
}

void CPU::opcodeCB0xC3() // SET 0,E
{
	
}

void CPU::opcodeCB0xC4() // SET 0,H
{
	
}

void CPU::opcodeCB0xC5() // SET 0,L
{
	
}

void CPU::opcodeCB0xC6() // SET 0,(HL)
{
	
}

void CPU::opcodeCB0xC7() // SET 0,A
{
	
}

void CPU::opcodeCB0xC8() // SET 1,B
{
	
}

void CPU::opcodeCB0xC9() // SET 1,C
{
	
}

void CPU::opcodeCB0xCA() // SET 1,D
{
	
}

void CPU::opcodeCB0xCB() // SET 1,E
{
	
}

void CPU::opcodeCB0xCC() // SET 1,H
{
	
}

void CPU::opcodeCB0xCD() // SET 1,L
{
	
}

void CPU::opcodeCB0xCE() // SET 1,(HL)
{
	
}

void CPU::opcodeCB0xCF() // SET 1,A
{
	
}

//0xD0

void CPU::opcodeCB0xD0() // SET 2,B
{
	
}

void CPU::opcodeCB0xD1() // SET 2,C
{
	
}

void CPU::opcodeCB0xD2() // SET 2,D
{
	
}

void CPU::opcodeCB0xD3() // SET 2,E
{
	
}

void CPU::opcodeCB0xD4() // SET 2,H
{
	
}

void CPU::opcodeCB0xD5() // SET 2,L
{
	
}

void CPU::opcodeCB0xD6() // SET 2,(HL)
{
	
}

void CPU::opcodeCB0xD7() // SET 2,A
{
	
}

void CPU::opcodeCB0xD8() // SET 3,B
{
	
}

void CPU::opcodeCB0xD9() // SET 3,C
{
	
}

void CPU::opcodeCB0xDA() // SET 3,D
{
	
}

void CPU::opcodeCB0xDB() // SET 3,E
{
	
}

void CPU::opcodeCB0xDC() // SET 3,H
{
	
}

void CPU::opcodeCB0xDD() // SET 3,L
{
	
}

void CPU::opcodeCB0xDE() // SET 3,(HL)
{
	
}

void CPU::opcodeCB0xDF() // SET 3,A
{
	
}

//0xE0

void CPU::opcodeCB0xE0() // SET 4,B
{
	
}

void CPU::opcodeCB0xE1() // SET 4,C
{
	
}

void CPU::opcodeCB0xE2() // SET 4,D
{
	
}

void CPU::opcodeCB0xE3() // SET 4,E
{
	
}

void CPU::opcodeCB0xE4() // SET 4,H
{
	
}

void CPU::opcodeCB0xE5() // SET 4,L
{
	
}

void CPU::opcodeCB0xE6() // SET 4,(HL)
{
	
}

void CPU::opcodeCB0xE7() // SET 4,A
{
	
}

void CPU::opcodeCB0xE8() // SET 5,B
{
	
}

void CPU::opcodeCB0xE9() // SET 5,C
{
	
}

void CPU::opcodeCB0xEA() // SET 5,D
{
	
}

void CPU::opcodeCB0xEB() // SET 5,E
{
	
}

void CPU::opcodeCB0xEC() // SET 5,H
{
	
}

void CPU::opcodeCB0xED() // SET 5,L
{
	
}

void CPU::opcodeCB0xEE() // SET 5,(HL)
{
	
}

void CPU::opcodeCB0xEF() // SET 5,A
{
	
}

//0xF0

void CPU::opcodeCB0xF0() // SET 6,B
{
	
}

void CPU::opcodeCB0xF1() // SET 6,C
{
	
}

void CPU::opcodeCB0xF2() // SET 6,D
{
	
}

void CPU::opcodeCB0xF3() // SET 6,E
{
	
}

void CPU::opcodeCB0xF4() // SET 6,H
{
	
}

void CPU::opcodeCB0xF5() // SET 6,L
{
	
}

void CPU::opcodeCB0xF6() // SET 6,(HL)
{
	
}

void CPU::opcodeCB0xF7() // SET 6,A
{
	
}

void CPU::opcodeCB0xF8() // SET 7,B
{
	
}

void CPU::opcodeCB0xF9() // SET 7,C
{
	
}

void CPU::opcodeCB0xFA() // SET 7,D
{
	
}

void CPU::opcodeCB0xFB() // SET 7,E
{
	
}

void CPU::opcodeCB0xFC() // SET 7,H
{
	
}

void CPU::opcodeCB0xFD() // SET 7,L
{
	
}

void CPU::opcodeCB0xFE() // SET 7,(HL)
{
	
}

void CPU::opcodeCB0xFF() // SET 7,A
{
	
}
