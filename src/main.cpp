#include "gameboy.h"

int main(int argc, char** argv)
{
    if(argc != 2) return -1;

    Gameboy gb;

    gb.load(argv[1]);
    gb.start();

    return 0;
}