#pragma once

#define GET_BIT(x, n) ((x >> n) & 0x01)
#define SET_BIT(x, n) (x |= (0x01 << n))
#define CLEAR_BIT(x, n) (x &= ~(0x01 << n))
#define FLIP_BIT(x, n) (x ^= (0x01 << n))