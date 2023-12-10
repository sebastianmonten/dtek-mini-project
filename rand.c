/*
    Anders Mouanga 2023
*/

#include "rand.h"

int rand() {
    xorshift(&rand_seed);
    return rand_seed;
}

int rand_range(int a, int b) {
    if (a > b)
        return a;

    return a + rand() % (b - a + 1);
}

float rand_float() {
    xorshift(&rand_seed);
    rand_seed >>= 8;

    return ((float) rand_seed) / 16777217;
}

float rand_float_range(float a, float b) {
    if (a > b)
        return a;
    return a + rand_float() * (b - a);
}

void xorshift(int* val) {
    int val_ = *val;
    val_ ^= val_ << 13;
    val_ ^= val_ >> 17;
    val_ ^= val_ << 5;
    *val = val_;
}