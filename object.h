#ifndef OBJECT_H
#define OBJECT_H
#include "graphics.h"
#include <stdbool.h>
enum DrawModes {
    DRAW_MODE_NONE,
    DRAW_MODE_NORMAL,
    DRAW_MODE_LINE,
    DRAW_MODE_RECTANGLE,
    DRAW_MODE_CIRCLE
};

struct Object_s {
    Sprite* sprite;
    int x;
    int y;
    int x_speed;
    int y_speed;
    void* bonus_data;
    int bonus_data_size;

    // update_func is a void function that takes an Object* as its only parameter.
    // i assume most Objects will share a common Update function.
    void (*update_func)(struct Object_s*);
    bool active;

};

typedef struct Object_s Object;

#define MAX_OBJECTS 100
Object objects[MAX_OBJECTS];
int object_count;

void update_object(Object* o);
void draw_object(Object* o);
void bounce_ball_ai(Object* ball);
void update_object_general(Object* o);
Object* add_object(int x, int y, Sprite* sprite, void (*update_func)(Object*), void* bonus_data, int bonus_data_size);
Object* add_blank_object();
void delete_object(Object* o);
void add_line_obstacle();
void line_obstacle_ai(Object* line);

void add_line_obstacle_horizontal();
void line_obstacle_horizontal_ai(Object* line);
void add_portal();
void portal_ai();

#endif