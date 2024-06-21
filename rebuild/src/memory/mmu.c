#include "mmu.h"

#include <stdlib.h>

#include <logging/logging.h>

struct MMU *create_mmu() {

    struct MMU *mmu = malloc(sizeof(struct MMU));
    if (!mmu) {
        goto init_fail;
    }

    mmu->video_ram = malloc(sizeof(u8) * VIDEO_RAM_SIZE * VIDEO_RAM_BANK_COUNT);
    if (!mmu->video_ram) {
        goto video_ram_fail;
    }
    mmu->video_ram_bank = 0;

    mmu->work_ram_0 = malloc(sizeof(u8) * WORK_RAM_0_SIZE);
    if (!mmu->work_ram_0) {
        goto work_ram_0_fail;
    }

    mmu->work_ram_swappable = malloc(sizeof(u8) * WORK_RAM_SWAPPABLE_SIZE * WORK_RAM_SWAPPABLE_COUNT);
    if (!mmu->work_ram_swappable) {
        goto work_ram_swappable_fail;
    }
    mmu->work_ram_bank = 0;

    mmu->oam = malloc(sizeof(u8) * OAM_SIZE);
    if (!mmu->oam) {
        goto oam_fail;
    }

    mmu->high_ram = malloc(sizeof(u8) * HIGH_RAM_SIZE);
    if (!mmu->high_ram) {
        goto high_ram_fail;
    }

    goto init_success;

    high_ram_fail:
    free(mmu->oam);

    oam_fail:
    free(mmu->work_ram_swappable);

    work_ram_swappable_fail:
    free(mmu->work_ram_0);

    work_ram_0_fail:
    free(mmu->video_ram);

    video_ram_fail:
    free(mmu);

    init_fail:
    return NULL;

    init_success:
    gameboy_log(LOG_DEBUG, "Initialized MMU!");
    return mmu;
}

void destroy_mmu(struct MMU *mmu) {

    free(mmu->high_ram);
    free(mmu->oam);
    free(mmu->work_ram_swappable);
    free(mmu->work_ram_0);
    free(mmu->video_ram);
    free(mmu);
    gameboy_log(LOG_DEBUG, "Destroyed MMU!");
}

void set_address(struct MMU *mmu, u16 address) {
    mmu->address = address;
}

u8 read_byte(struct MMU *mmu) {

    u16 address = mmu->address;
    u16 translated_address;

    if (address < ROM_SIZE) {
        return 0; // TODO: Cart
    }

    if (address <= VIDEO_RAM_END) {
        translated_address = address - VIDEO_RAM_START;
        translated_address += mmu->video_ram_bank * VIDEO_RAM_SIZE; // Account for multiple vram banks
        return mmu->video_ram[translated_address];
    }

    if (address <= EXTERNAL_RAM_END) {
        return 0; // TODO: Cart
    }

    if (address <= WORK_RAM_0_END) {
        translated_address = address - WORK_RAM_0_START;
        return mmu->work_ram_0[translated_address];
    }

    if (address <= WORK_RAM_SWAPPABLE_END) {
        translated_address = address - WORK_RAM_SWAPPABLE_START;
        translated_address += mmu->work_ram_bank * WORK_RAM_SWAPPABLE_SIZE; // Account for multiple work ram banks
        return mmu->work_ram_swappable[translated_address];
    }

    if (address <= ECHO_RAM_0_END) {
        translated_address = address - ECHO_RAM_0_START;
        return mmu->work_ram_0[translated_address]; // Echo ram maps to work ram
    }

    if (address <= ECHO_RAM_SWAPPABLE_END) {
        translated_address = address - ECHO_RAM_SWAPPABLE_START;
        translated_address += mmu->work_ram_bank * WORK_RAM_SWAPPABLE_SIZE; // Account for multiple work ram banks
        return mmu->work_ram_swappable[translated_address]; // Echo ram maps to work ram
    }

    if (address <= OAM_END) {
        translated_address = address - OAM_START;
        return mmu->oam[translated_address];
    }

    if (address <= NOT_USABLE_END) {
        gameboy_log(LOG_ERROR, "Invalid read at address 0x%04X", address);
        return UINT8_MAX;
    }

    if (address <= IO_REGISTERS_END) {
        return 0; // TODO: IO
    }

    if (address <= HIGH_RAM_END) {
        translated_address = address - HIGH_RAM_START;
        return mmu->high_ram[address];
    }

    if (address == INTERUPT_ENABLE_REGISTER) {
        return 0; // TODO: IE
    }

    gameboy_log(LOG_ERROR, "Invalid read at address of 0x%X", address);
    return UINT8_MAX;
}

void write_byte(struct MMU *mmu, u8 byte) {

    u16 address = mmu->address;
    u16 translated_address;

    if (address < ROM_SIZE) {
        return; // TODO: Cart
    }

    if (address <= VIDEO_RAM_END) {
        translated_address = address - VIDEO_RAM_START;
        translated_address += mmu->video_ram_bank * VIDEO_RAM_SIZE; // Account for multiple vram banks
        mmu->video_ram[translated_address] = byte;
    }

    if (address <= EXTERNAL_RAM_END) {
        return; // TODO: Cart
    }

    if (address <= WORK_RAM_0_END) {
        translated_address = address - WORK_RAM_0_START;
        mmu->work_ram_0[translated_address] = byte;
    }

    if (address <= WORK_RAM_SWAPPABLE_END) {
        translated_address = address - WORK_RAM_SWAPPABLE_START;
        translated_address += mmu->work_ram_bank * WORK_RAM_SWAPPABLE_SIZE; // Account for multiple work ram banks
        mmu->work_ram_swappable[translated_address] = byte;
    }

    if (address <= ECHO_RAM_0_END) {
        translated_address = address - ECHO_RAM_0_START;
        mmu->work_ram_0[translated_address] = byte; // Echo ram maps to work ram
    }

    if (address <= ECHO_RAM_SWAPPABLE_END) {
        translated_address = address - ECHO_RAM_SWAPPABLE_START;
        translated_address += mmu->work_ram_bank * WORK_RAM_SWAPPABLE_SIZE; // Account for multiple work ram banks
        mmu->work_ram_swappable[translated_address] = byte; // Echo ram maps to work ram
    }

    if (address <= OAM_END) {
        translated_address = address - OAM_START;
        mmu->oam[translated_address] = byte;
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
    }

    if (address == INTERUPT_ENABLE_REGISTER) {
        return; // TODO: IE
    }

    gameboy_log(LOG_ERROR, "Invalid write of 0x%02X at address 0x%04X", byte, address);
}
