#include <gameboy.h>


#include <cpu/instructions.h>
#include <logging/logging.h>

int init_gameboy(struct Gameboy *gb) {
    
    if (init_bus(&gb->bus)) {

        gameboy_log(LOG_FATAL, "Failed to initialize Bus!");
        goto bus_init_fail;
    }

    if (init_apu(&gb->apu)) {

        gameboy_log(LOG_FATAL, "Failed to initialize APU!");
        goto apu_init_fail;
    }

    if (init_cpu(&gb->cpu)) {

        gameboy_log(LOG_FATAL, "Failed to initialize CPU!");
        goto cpu_init_fail;
    }

    if (init_mmu(&gb->mmu)) {

        gameboy_log(LOG_FATAL, "Failed to initialize MMU!");
        goto mmu_init_fail;
    }

    if (init_ppu(&gb->ppu)) {

        gameboy_log(LOG_FATAL, "Failed to initialize PPPU!");
        goto ppu_init_fail;
    }

    if (init_screen(&gb->screen, 5)) {

        gameboy_log(LOG_FATAL, "Failed to initialized Screen!");
        goto screen_init_fail;
    }

    set_mmu(&gb->bus, &gb->mmu);

    gb->cart.rom_banks = NULL;
    gb->cart.ram_banks = NULL;
    gb->bus.cart = &gb->cart;
    gb->running = false;

    goto init_success;

    screen_init_fail:
    cleanup_ppu(&gb->ppu);

    ppu_init_fail:
    cleanup_mmu(&gb->mmu);

    mmu_init_fail:
    cleanup_cpu(&gb->cpu);

    cpu_init_fail:
    cleanup_apu(&gb->apu);

    apu_init_fail:
    cleanup_bus(&gb->bus);

    bus_init_fail:
    return INIT_FAIL;

    init_success:
    return RETURN_OK;
}

void cleanup_gameboy(struct Gameboy *gb) {

    cleanup_cartridge(&gb->cart);
    cleanup_screen(&gb->screen);
    cleanup_ppu(&gb->ppu);
    cleanup_mmu(&gb->mmu);
    cleanup_cpu(&gb->cpu);
    cleanup_apu(&gb->apu);
    cleanup_bus(&gb->bus);
}

void start_gameboy(struct Gameboy *gb) {
    
    gameboy_log(LOG_DEBUG, "Starting Gameboy:");
    
    reset_cpu(&gb->cpu);

    // Prefetch the first instruction
    set_address(&gb->bus, gb->cpu.registers.pc);
    read_byte(&gb->bus);
    gb->cpu.registers.ir = gb->bus.data;
    gb->cpu.registers.pc++;

    gb->running = true;
}

void step(struct Gameboy *gb) {

    if (!execute_opcode(gb)) {
        gb->running = false; // Temp while setting up opcodes
    }

    // TODO: Proper screen handling. For now update screen on every opcode
    update_screen(&gb->screen);
    poll_screen_events(gb);

    // TODO: Halting Bug
    // TODO: Interrupts
}
