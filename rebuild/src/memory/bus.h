#ifndef BUS_H
#define BUS_H

#include <core.h>

struct Cartridge;
struct MMU;

struct Bus {

    u16 address;

    struct Cartridge *cart;
    struct MMU *mmu;
};

/**
 * @brief Initializes the bus
 * 
 * @param bus 
 * @return int 
 */
int init_bus(struct Bus *bus);

/**
 * @brief Cleans up the bus
 * 
 * @param bus 
 */
void cleanup_bus(struct Bus *bus);

/**
 * @brief Set the cartridge the bus can use
 * 
 * @param bus 
 * @param cart 
 */
void set_cart(struct Bus *bus, struct Cartridge *cart);

/**
 * @brief Set the MMU the bus can use
 * 
 * @param bus 
 * @param mmu 
 */
void set_mmu(struct Bus *bus, struct MMU *mmu);

/**
 * @brief Sets the address of the bus
 * 
 * @param bus 
 * @param address 
 */
void set_address(struct Bus *bus, u16 address);

/**
 * @brief Read a byte from the set address on the bus
 * 
 * @param bus 
 * @return u8 
 */
u8 read_byte(struct Bus *bus);

/**
 * @brief Write a byte to the set address on the bus
 * 
 * @param bus 
 * @param byte 
 */
void write_byte(struct Bus *bus, u8 byte);

#endif // BUS_H
