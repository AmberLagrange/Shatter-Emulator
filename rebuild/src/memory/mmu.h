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

/**
 * @brief Initializes the MMU
 * 
 * @param mmu 
 * @return int 
 */
int init_mmu(struct MMU *mmu);

/**
 * @brief Cleans up the MMU
 * 
 * @param mmu 
 */
void cleanup_mmu(struct MMU *mmu);

#endif // MMU_H
