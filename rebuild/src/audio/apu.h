#ifndef APU_H
#define APU_H

#include <core.h>

struct APU {

};

/**
 * @brief Initializes the APU
 * 
 * @param apu 
 * @return int 
 */
int init_apu(struct APU *apu);

/**
 * @brief Cleans up the APU
 * 
 * @param apu 
 */
void cleanup_apu(struct APU *apu);

#endif // APU_H
