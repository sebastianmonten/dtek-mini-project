// for use when generating random enemies.
// the seed will be some function of time elapsed between pic32 startup
// and user press of "start game" button.
#ifndef RANDOM_H
#define RANDOM_H

int random_seed;
int current_number;

// uses xorshift to modify current_number to a new "random" value
void next_number();
int random_int();
float random_float();


#endif
