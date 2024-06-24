#ifndef PPU_H
#define PPU_H

#include <core.h>

struct PPU {

};

/**
 * @brief Initializes the PPU
 * 
 * @param ppu 
 * @return int 
 */
int init_ppu(struct PPU *ppu);

/**
 * @brief Cleans up the PPU
 * 
 * @param ppu 
 */
void cleanup_ppu(struct PPU *ppu);

/**
 * @brief Ticks the PPU by 1 M cycle
 * 
 * @param gb 
 */
void tick_ppu(struct PPU *ppu);

#endif // PPU_H
