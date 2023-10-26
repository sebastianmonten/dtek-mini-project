#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h" 

int getsw( void ) {
    volatile int* portd = (volatile int*) PORTD;
    return ((*portd) >> 8 ) & 0b1111;
    // returns 0000000 0000000 0000000 0000 SW4 SW3 SW2 SW1
}

int getbtns(void) {
    volatile int* portd = (volatile int*) PORTD;
    return ((*portd) >> 5 ) & 0b111;
    // returns 0000000 0000000 0000000 00000 BTN4 BTN3 BTN2
}