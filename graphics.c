#include "graphics.h"
#include "mipslabdata.h"
#define SWAP(x, y) do {x ^= y; y ^= x; x ^= y;} while (0); 
void set_pixel(int x, int y) {
    // out of bounds heck
    if (x > 127 || y > 31 || x < 0 || y < 0)
        return;

    int page = x / 32;


    int local_x = x % 32;

    int index = (y / 8) * 32 + local_x;
    int bitindex = y % 8;

    buf[page][index] &= ~(1 << bitindex);
}

static void set_pixel_unchecked(int x, int y) {
    int page = x / 32;
    int local_x = x % 32;
    int index = (y / 8) * 32 + local_x;
    int bitindex = y % 8;
    buf[page][index] &= ~(1 << bitindex);
}

// dont use this, use put_line_horizontal or put_line_vertical
void put_line(int x1, int y1, int x2, int y2) {  
    return;
}

void put_line_horizontal(int x1, int x2, int y) {
	if (x1 > x2)
		SWAP(x1, x2);
	
	if (y < Y_MIN || y > Y_MAX)
		return;

	while (x1 <= x2 && x1 < SCREEN_WIDTH) {
		if (x1 < 0) {
			x1 = 0;
			continue;
		}
		set_pixel_unchecked(x1, y);
		x1++;
	}
}

void put_line_vertical(int y1, int y2, int x) {
	if (y1 > y2)
		SWAP(y1, y2);
	
	if (x < X_MIN || x > X_MAX)
		return;

	while (y1 <= y2 && y1 < SCREEN_HEIGHT) {
		if (y1 < 0) {
			y1 = 0;
			continue;
		}
		set_pixel_unchecked(x, y1);
		y1++;
	}
}

void draw_to_buf(int x, int y, Sprite s) {
	int sx;
	int sy;
	for (sy = 0; sy < s.height; sy++) {
		for (sx = 0; sx < s.width; sx++) {
			if (s.data[sy][sx]) {
				// check if the current x-value of the pixel is out of bounds
				int pixel_x = x + sx - s.x_origin;
				if (pixel_x < 0)
					continue;
				else if (pixel_x > 127)
					break;
				
				// check if the current y-value of the pixel is out of bounds
				int pixel_y = y + sy - s.y_origin;
				if (pixel_y < 0)
					continue;
				else if (pixel_y > 31)
					break;

				int page = pixel_x / 32;
				int local_x = (pixel_x) % 32;

				int index = ((pixel_y) / 8) * 32 + local_x;
				// int bitindex = 7 - ((y + sy) % 8);
				int bitindex = ((pixel_y) % 8);

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

void put_circle(int x, int y, int rad) {
    // will add bresenham algo later..
}

void put_rectangle(int x1, int y1, int x2, int y2) {
	put_line_horizontal(x1, x2, y1);
	put_line_horizontal(x1, x2, y2);

	put_line_vertical(y1, y2, x1);
	put_line_vertical(y1, y2, x2);
}