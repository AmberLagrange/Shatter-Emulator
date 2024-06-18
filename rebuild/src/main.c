#include <gameboy.h>

int main() {

    struct Gameboy gb;
    init_gameboy(&gb);

    start_gameboy(&gb);

    destroy_gameboy(&gb);

    return 0;
}