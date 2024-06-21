#ifndef MMU_H
#define MMU_H

#include <core.h>

struct MMU {

    u16 address;

    // TODO: Cart
    u8 *video_ram;
    u8 video_ram_bank;
    // external ram is on cart
    u8 *work_ram_0;
    u8 *work_ram_swappable;
    u8 work_ram_bank;
    // echo ram maps to work ram
    u8 *oam;
    // not usable
    // io maps to other functions
    u8 *high_ram;
    // ie maps to other functions
};

struct MMU *init_mmu();
void destroy_mmu(struct MMU *mmu);

void set_address(struct MMU *mmu, u16 address);
u8   read_byte(struct MMU *mmu);
void write_byte(struct MMU *mmu, u8 byte);

#endif // MMU_H
