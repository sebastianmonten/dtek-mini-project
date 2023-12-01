#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <stdint.h>
#include "sprite.h"

// sample values for now
#define SCREEN_WIDTH (128)
#define SCREEN_HEIGHT (32)

#define X_MIN (0)
#define X_MAX (SCREEN_WIDTH - 1)
#define Y_MIN (0)
#define Y_MAX (SCREEN_HEIGHT - 1)

// typedef struct {
//     uint8_t** data;
//     int width;
//     int height;
// } Sprite;

typedef struct {
    uint8_t data[SCREEN_HEIGHT][SCREEN_WIDTH];
} Screen;

// A pointer to the screen.
// Currently NULL because we currently have no idea where video data is stored.
// Can be changed later to a non-pointer by dereferencing it in this
// definition.
#define SCREEN ((Screen*) NULL);

/* Direct pixel manipulation */

void set_pixel(int x, int y);
void clear_pixel(int x, int y);
void toggle_pixel(int x, int y);

/* Drawing shapes */

void put_circle(int x, int y, int rad);
void put_line(int x1, int y1, int x2, int y2);
void put_line_horizontal(int x1, int x2, int y);
void put_line_vertical(int y1, int y2, int x);
void put_rectangle(int x1, int y1, int x2, int y2);
void draw_to_buf(int x, int y, const Sprite* sprite);

void update_screen();

#endif