#include "bus.h"

#include <cartridge/cartridge.h>
#include <memory/mmu.h>

#include <logging/logging.h>

int init_bus(struct Bus *bus) {

    bus->address = 0;
    gameboy_log(LOG_DEBUG, "Initialized Bus!");
    return RETURN_OK;
}

void cleanup_bus(struct Bus *bus) {

    gameboy_log(LOG_DEBUG, "Destroyed Bus!");
}

void set_cart(struct Bus *bus, struct Cartridge *cart) {

    bus->cart = cart;
}

void set_mmu(struct Bus *bus, struct MMU *mmu) {

    bus->mmu = mmu;
}

void set_address(struct Bus *bus, u16 address) {
    bus->address = address;
}

void read_byte(struct Bus *bus) {

    u16 translated_address;
    u16 address = bus->address;

    struct Cartridge *cart = bus->cart;
    struct MMU *mmu = bus->mmu;

    if (address < ROM_SIZE) {
        bus->data = read_byte_from_cart(cart, address);
        return;
    }

    if (address <= VIDEO_RAM_END) {
        translated_address = address - VIDEO_RAM_START;
        translated_address += mmu->video_ram_bank * VIDEO_RAM_SIZE; // Account for multiple vram banks
        bus->data = mmu->video_ram[translated_address];
        return;
    }

    if (address <= EXTERNAL_RAM_END) {
        bus->data = 0; // TODO: Cart
        return;
    }

    if (address <= WORK_RAM_0_END) {
        translated_address = address - WORK_RAM_0_START;
        bus->data = mmu->work_ram_0[translated_address];
        return;
    }

    if (address <= WORK_RAM_SWAPPABLE_END) {
        translated_address = address - WORK_RAM_SWAPPABLE_START;
        translated_address += mmu->work_ram_bank * WORK_RAM_SWAPPABLE_SIZE; // Account for multiple work ram banks
        bus->data = mmu->work_ram_swappable[translated_address];
        return;
    }

    if (address <= ECHO_RAM_0_END) {
        translated_address = address - ECHO_RAM_0_START;
        bus->data = mmu->work_ram_0[translated_address]; // Echo ram maps to work ram
        return;
    }

    if (address <= ECHO_RAM_SWAPPABLE_END) {
        translated_address = address - ECHO_RAM_SWAPPABLE_START;
        translated_address += mmu->work_ram_bank * WORK_RAM_SWAPPABLE_SIZE; // Account for multiple work ram banks
        bus->data = mmu->work_ram_swappable[translated_address]; // Echo ram maps to work ram
        return;
    }

    if (address <= OAM_END) {
        translated_address = address - OAM_START;
        bus->data = mmu->oam[translated_address];
        return;
    }

    if (address <= NOT_USABLE_END) {
        gameboy_log(LOG_ERROR, "Invalid read at address 0x%04X", address);
        bus->data = UINT8_MAX;
        return;
    }

    if (address <= IO_REGISTERS_END) {
        bus->data = 0; // TODO: IO
        return;
    }

    if (address <= HIGH_RAM_END) {
        translated_address = address - HIGH_RAM_START;
        bus->data = mmu->high_ram[address];
        return;
    }

    if (address == IE_REGISTER_ADDRESS) {
        bus->data = 0; // TODO: IE
        return;
    }

    gameboy_log(LOG_ERROR, "Invalid read at address of 0x%X", address);
    bus->data = UINT8_MAX;
}

void write_byte(struct Bus *bus, u8 byte) {

    u16 translated_address;
    u16 address = bus->address;

    struct Cartridge *cart = bus->cart;
    struct MMU *mmu = bus->mmu;

    if (address < ROM_SIZE) {
        (void)(cart);
        return; // TODO: Cart
    }

    if (address <= VIDEO_RAM_END) {
        translated_address = address - VIDEO_RAM_START;
        translated_address += mmu->video_ram_bank * VIDEO_RAM_SIZE; // Account for multiple vram banks
        mmu->video_ram[translated_address] = byte;
        return;
    }

    if (address <= EXTERNAL_RAM_END) {
        return; // TODO: Cart
    }

    if (address <= WORK_RAM_0_END) {
        translated_address = address - WORK_RAM_0_START;
        mmu->work_ram_0[translated_address] = byte;
        return;
    }

    if (address <= WORK_RAM_SWAPPABLE_END) {
        translated_address = address - WORK_RAM_SWAPPABLE_START;
        translated_address += mmu->work_ram_bank * WORK_RAM_SWAPPABLE_SIZE; // Account for multiple work ram banks
        mmu->work_ram_swappable[translated_address] = byte;
        return;
    }

    if (address <= ECHO_RAM_0_END) {
        translated_address = address - ECHO_RAM_0_START;
        mmu->work_ram_0[translated_address] = byte; // Echo ram maps to work ram
        return;
    }

    if (address <= ECHO_RAM_SWAPPABLE_END) {
        translated_address = address - ECHO_RAM_SWAPPABLE_START;
        translated_address += mmu->work_ram_bank * WORK_RAM_SWAPPABLE_SIZE; // Account for multiple work ram banks
        mmu->work_ram_swappable[translated_address] = byte; // Echo ram maps to work ram
        return;
    }

    if (address <= OAM_END) {
        translated_address = address - OAM_START;
        mmu->oam[translated_address] = byte;
        return;
    }

    if (address <= NOT_USABLE_END) {
        gameboy_log(LOG_ERROR, "Invalid write of 0x%02X at address 0x%04X", byte, address);
        return;
    }

    if (address <= IO_REGISTERS_END) {
        return; // TODO: IO
    }

    if (address <= HIGH_RAM_END) {
        translated_address = address - HIGH_RAM_START;
        mmu->high_ram[address] = byte;
        return;
    }

    if (address == IE_REGISTER_ADDRESS) {
        return; // TODO: IE
    }

    gameboy_log(LOG_ERROR, "Invalid write of 0x%02X at address 0x%04X", byte, address);
}