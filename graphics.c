#include "graphics.h"
#define SWAP(x, y) do {x ^= y; y ^= x; x ^= y;} while (0); 

void set_pixel(int x, int y) {
    // out of bounds heck
    if (x > 127 || y > 31)
        return;

    int page = x / 32;


    int local_x = x % 32;

    int index = (y / 8) * 32 + local_x;
    int bitindex = y % 8;

    buf[page][index] &= ~(1 << bitindex);
}

void set_pixel_unchecked(int x, int y) {
    int page = x / 32;
    int local_x = x % 32;
    int index = (y / 8) * 32 + local_x;
    int bitindex = y % 8;
    buf[page][index] &= ~(1 << bitindex);
}

// bresenham's line algo
void put_line(int x1, int y1, int x2, int y2) {  
    // always go from top left to bottom right
    if (x1 > x2)
        SWAP(x1, x2);
    if (y1 > y2)
        SWAP(y1, y2);

    int 
    dx = x2 - x1, // delta x
    dy = y2 - y1, // delta y
    x = x1, // starting x
    y = y1, // starting y
    p = 2 * (dy - dx); // discriminator 
    while (x < x2) {  
        set_pixel(x, y);
        if(p >= 0) {  
            y++;  
            p += 2*(dy-dx);  
        } else {  
            p += 2*dy;}  
            x++;  
        }  
}