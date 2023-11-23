#include <stdint.h>
#ifndef SPRITE_H
#define SPRITE_H

typedef struct {
    const uint8_t** data;
    const int width;
    const int height;
    const int x_origin;
    const int y_origin;
} Sprite;

#endif