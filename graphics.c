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

void draw_to_buf(int x, int y, Sprite s) {
	int sx;
	int sy;
	for (sy = 0; sy < s.height; sy++) {
		for (sx = 0; sx < s.width; sx++) {
			if (s.data[sy][sx]) {
				int page = (x + sx) / 32;
				if (page > 3) break;
				if (sy + y >= 32) break;

				int local_x = (x + sx) % 32;

				int index = ((y + sy) / 8) * 32 + local_x;
				// int bitindex = 7 - ((y + sy) % 8);
				int bitindex = ((y + sy) % 8);

				buf[page][index] &= ~(1 << bitindex);
				
			}
			
		}
	}

}

void display_buf(void) {
	int i;
	for (i = 0; i<4; i++) {
		display_image(i*32, buf[i]);
	}
}

void clear_buf(void) {
	int i;
	int j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 128; j++) {
			buf[i][j] = ~0;
		}
	}
}

void draw_to_buf_test(void) {
	// int bitindex = 7;
	// int page = 0;
	// int index = 32;
	buf[0][1] &= ~(1 << (7- 0));
	buf[0][1] &= ~(1 << (7- 1));
	buf[0][1] &= ~(1 << (7- 2));
	buf[0][1] &= ~(1 << (7- 3));
	buf[0][1] &= ~(1 << (7- 4));
	buf[0][1] &= ~(1 << (7- 5));
	buf[0][1] &= ~(1 << (7- 6));
	buf[0][1] &= ~(1 << (7- 7));
	
	buf[0][32] &= ~(1 << (7- 0));
	buf[0][32] &= ~(1 << (7- 1));
	buf[0][32] &= ~(1 << (7- 2));
	buf[0][32] &= ~(1 << (7- 3));
	buf[0][32] &= ~(1 << (7- 4));
	buf[0][32] &= ~(1 << (7- 5));
	buf[0][32] &= ~(1 << (7- 6));
	buf[0][32] &= ~(1 << (7- 7));
}
