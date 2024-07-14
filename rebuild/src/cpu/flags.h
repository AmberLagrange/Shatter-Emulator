#ifndef FLAGS_H
#define FLAGS_H

// F register
// ZNHC....

// Z (Zero) flag is set when the result of an operation is zero (0)
// N (Negative) flag is set when an instruction is a subtraction. Only used in DAA
// H (Half) flag is set when the result of an operation causes a carry from the lower nibble. Only used in DAA
// C (Carry) flag is set when the result of an operation causes a carry from the upper nibble

enum Flags {
    FLAG_ZERO       = 7,
    FLAG_NEGATIVE   = 6,
    FLAG_HALF       = 5,
    FLAG_CARRY      = 4,
};

#define GET_FLAG(flag)      ((gb->cpu.registers.f &   (1 << flag)) >> flag)
#define SET_FLAG(flag)      ((gb->cpu.registers.f |=  (1 << flag)))
#define CLEAR_FLAG(flag)    ((gb->cpu.registers.f &= ~(1 << flag)))
#define TOGGLE_FLAG(flag)   ((gb->cpu.registers.f ^=  (1 << flag)))

#endif // FLAGS_H
