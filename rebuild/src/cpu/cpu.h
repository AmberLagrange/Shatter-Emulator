#ifndef CPU_H
#define CPU_H

#include <core.h>

struct Registers {

    #ifdef IS_BIG_ENDIAN

        union {
            struct {
                u8 a;
                u8 f;
            };
            u16 af;
        };

        union {
            struct {
                u8 b;
                u8 c;
            };
            u16 bc;
        };

        union {
            struct {
                u8 d;
                u8 e;
            };
            u16 de;
        };

        union {
            struct {
                u8 h;
                u8 l;
            };
            u16 hl;
        };
    #else

        union {
            struct {
                u8 f;
                u8 a;
            };
            u16 af;
        };

        union {
            struct {
                u8 c;
                u8 b;
            };
            u16 bc;
        };

        union {
            struct {
                u8 e;
                u8 d;
            };
            u16 de;
        };

        union {
            struct {
                u8 l;
                u8 h;
            };
            u16 hl;
        };
    #endif

    u16 sp;
    u16 pc;

};

struct CPU {
    
    struct Registers registers;
};

struct CPU *init_cpu();
void destroy_cpu(struct CPU *cpu);

void reset_cpu(struct CPU *cpu);

#endif // CPU_H
