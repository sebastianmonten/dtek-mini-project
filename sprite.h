#include <stdint.h>
#ifndef SPRITE_H
#define SPRITE_H

typedef struct {
    uint8_t** data;
    int width;
    int height;
    int x_origin;
    int y_origin;
} Sprite;

#endif