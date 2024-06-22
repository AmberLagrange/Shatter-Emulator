#include "mmu.h"

#include <stdlib.h>

#include <logging/logging.h>

int init_mmu(struct MMU *mmu) {

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
    return INIT_FAIL;

    init_success:
    gameboy_log(LOG_DEBUG, "Initialized MMU!");
    return RETURN_OK;
}

void cleanup_mmu(struct MMU *mmu) {

    free(mmu->high_ram);
    free(mmu->oam);
    free(mmu->work_ram_swappable);
    free(mmu->work_ram_0);
    free(mmu->video_ram);
    gameboy_log(LOG_DEBUG, "Destroyed MMU!");
}
