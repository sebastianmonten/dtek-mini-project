#ifndef OBJECT_H
#define OBJECT_H
#include "graphics.h"
enum DrawModes {
    DRAW_MODE_NONE,
    DRAW_MODE_NORMAL,
    DRAW_MODE_LINE,
    DRAW_MODE_RECTANGLE,
    DRAW_MODE_CIRCLE
};

typedef struct {
    Sprite* sprite;
    int x;
    int y;
    int x_speed;
    int y_speed;
    void* bonus_data;
    int bonus_data_size;

    // update_func is a void function that takes an Object* as its only parameter.
    // i assume most Objects will share a common Update function.
    void (*update_func)(void*); 
} Object;

// extern Object objects[100];

void update_object(Object* o);
void draw_object(void* o);
void bounce_ball_ai(Object* ball);
void update_object_general(Object* o);

#endif