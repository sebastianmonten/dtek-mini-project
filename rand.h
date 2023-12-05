#ifndef RAND_H
#define RAND_H

int rand_seed;

// returns a random int, from INT_MIN to INT_MAX
int rand();

// returns a random int from [a, b]
int rand_range(int a, int b);

// returns a random float in [0.0, 1.0)
float rand_float();

// returns a random float from [a, b)
float rand_float_range(float a, float b);

// uses the xorshift algorithm on an int to generate a new "random" int
void xorshift(int* val);


#endif