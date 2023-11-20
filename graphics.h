#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "mipslab.h"

// sample values for now
#define SCREEN_WIDTH (128)
#define SCREEN_HEIGHT (64)


typedef struct {
    uint8_t** data;
    int width;
    int height;
} Sprite;

typedef struct {
    uint8_t data[SCREEN_HEIGHT][SCREEN_WIDTH];
} Screen;

// A pointer to the screen's graphics data, for direct screen manipulation.
// Currently NULL because we currently have no idea where video data is stored.
// Can be changed later to a non-pointer by dereferencing it in this definition.
#define SCREEN ((Screen*) NULL)

// A pointer to the frame buffer.
// Currently NULL but will be set at program start.
// Can be changed later to a non-pointer by dereferencing it in this definition.
Screen* BUFFER;

/* Direct pixel manipulation */
// should perhaps be a macro? or inline?
void set_pixel(int x, int y);
void clear_pixel(int x, int y);
void toggle_pixel(int x, int y);

/* Drawing shapes */

void put_circle(int x, int y, int rad);
void put_line(int x1, int y1, int x2, int y2);
void put_rectangle(int x1, int y1, int x2, int y2);
void put_sprite(int x, int y, const Sprite* sprite);

/* Drawing to the frame buffer */
void draw_to_buf(int x, int y, Sprite s);
void display_buf(void);



void draw_to_buf_test(void);

#endif


