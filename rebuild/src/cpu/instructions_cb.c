#include <cpu/instructions.h>

#include <audio/apu.h>
#include <cpu/flags.h>
#include <cpu/instructions_helper.h>
#include <logging/logging.h>
#include <memory/bus.h>
#include <video/ppu.h>
#include <gameboy.h>

__attribute__((always_inline)) bool execute_cb_opcode(struct Gameboy *gb) {

    enum CB_Opcode cb_opcode = (enum CB_Opcode)gb->cpu.registers.ir;

    u8 carry;

    switch (cb_opcode) {
        
//--------------------------------0x00--------------------------------//



//--------------------------------0x10--------------------------------//

        // 0x18
        case CB_OPCODE_RR_B:

            RR(gb->cpu.registers.b);

        // 0x19
        case CB_OPCODE_RR_C:

            RR(gb->cpu.registers.c);

        // 0x1A
        case CB_OPCODE_RR_D:

            RR(gb->cpu.registers.d);

        // 0x1B
        case CB_OPCODE_RR_E:

            RR(gb->cpu.registers.e);

        // 0x1C
        case CB_OPCODE_RR_H:

            RR(gb->cpu.registers.h);

        // 0x1D
        case CB_OPCODE_RR_L:

            RR(gb->cpu.registers.l);

        // 0x1F
        case CB_OPCODE_RR_A:

            RR(gb->cpu.registers.a);

//--------------------------------0x20--------------------------------//



//--------------------------------0x30--------------------------------//

        // 0x38
        case CB_OPCODE_SRL_B:

            SRL(gb->cpu.registers.b);

//--------------------------------0x40--------------------------------//



//--------------------------------0x50--------------------------------//



//--------------------------------0x60--------------------------------//



//--------------------------------0x70--------------------------------//



//--------------------------------0x80--------------------------------//



//--------------------------------0x90--------------------------------//



//--------------------------------0xA0--------------------------------//



//--------------------------------0xB0--------------------------------//



//--------------------------------0xC0--------------------------------//



//--------------------------------0xD0--------------------------------//



//--------------------------------0xE0--------------------------------//



//--------------------------------0xF0--------------------------------//




        default:
            gameboy_log(LOG_FATAL, "Unhandled CB opcode: 0x%02X", (u8)cb_opcode);
    }

    return false;
}
