#include "core.h"
#include <memory/mmu.h>

#include <logging/logging.h>
#include <stdlib.h>
#include <string.h>

int init_mmu(struct MMU *mmu) {

    mmu->video_ram = malloc(sizeof(u8) * VIDEO_RAM_SIZE * VIDEO_RAM_BANK_COUNT);
    if (!mmu->video_ram) {
        goto video_ram_fail;
    }
    memset(mmu->video_ram, 0, sizeof(u8) * VIDEO_RAM_SIZE * VIDEO_RAM_BANK_COUNT);
    mmu->video_ram_bank = 0;

    mmu->work_ram_0 = malloc(sizeof(u8) * WORK_RAM_0_SIZE);
    if (!mmu->work_ram_0) {
        goto work_ram_0_fail;
    }
    memset(mmu->work_ram_0, 0, sizeof(u8) * WORK_RAM_0_SIZE);

    mmu->work_ram_swappable = malloc(sizeof(u8) * WORK_RAM_SWAPPABLE_SIZE * WORK_RAM_SWAPPABLE_COUNT);
    if (!mmu->work_ram_swappable) {
        goto work_ram_swappable_fail;
    }
    memset(mmu->work_ram_swappable, 0, sizeof(u8) * WORK_RAM_SWAPPABLE_SIZE * WORK_RAM_SWAPPABLE_COUNT);
    mmu->work_ram_bank = 0;

    mmu->oam = malloc(sizeof(u8) * OAM_SIZE);
    if (!mmu->oam) {
        goto oam_fail;
    }
    memset(mmu->oam, 0, sizeof(u8) * OAM_SIZE);

    mmu->high_ram = malloc(sizeof(u8) * HIGH_RAM_SIZE);
    if (!mmu->high_ram) {
        goto high_ram_fail;
    }
    memset(mmu->high_ram, 0, sizeof(u8) * HIGH_RAM_SIZE);

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
