#include "object.h"
#include "rand.h"
int object_count = 0;

#define NULL ((void*)0)

void bounce_ball_ai(Object* ball) {
    update_object_general(ball);

    if((ball->x <= 0) || (ball->x >= 128-(ball->sprite->width))) {
        ball->x_speed *= -1;
    }

    // if (ball->x <= 0)
    //     ball->x_speed = 4;

    // else if (ball->x >= 128-(ball->sprite->width))
    //     ball->x_speed = -4;
}

void draw_object(Object* o) {
    if (o->sprite)
        draw_to_buf(o->x, o->y, *(o->sprite));
}

void update_object(Object* o) {
    if (o->update_func)
        o->update_func(o);
    else
        update_object_general(o);
}

void update_object_general(Object* o) {
    o->x += o->x_speed;
    o->y += o->y_speed;
    draw_object(o);
}


// returns NULL if there are already too many objects
Object* add_object(int x, int y, Sprite* sprite, void (*update_func)(Object*), void* bonus_data, int bonus_data_size) {
    int i;
    for (i = 0; i < MAX_OBJECTS; i++)
        if (objects[i].active == false)
        {
            object_count++;
            objects[i].x = x;
            objects[i].y = y;
            objects[i].sprite = sprite;
            objects[i].bonus_data = bonus_data;
            objects[i].bonus_data_size = bonus_data_size;
            objects[i].update_func = update_func;
            objects[i].x_speed = 0;
            objects[i].y_speed = 0;
            objects[i].active = true;
            return &objects[i];
        }
    return (Object*) NULL;
}

// returns NULL if there are already too many objects
Object* add_blank_object() {
    int i;
    for (i = 0; i < MAX_OBJECTS; i++)
        if (objects[i].active == false) 
        {
            object_count++;
            objects[i].x = 0;
            objects[i].y = 0;
            objects[i].sprite = NULL;
            objects[i].bonus_data = NULL;
            objects[i].bonus_data_size = 0;
            objects[i].update_func = NULL;
            objects[i].x_speed = 0;
            objects[i].y_speed = 0;
            objects[i].active = true;
            return &objects[i];
        }

    return (Object*) NULL;
}

void delete_object(Object* o) {
    o->active = false;
    object_count--;
}

void line_obstacle_ai(Object* line) {
    update_object_general(line);

    if (line->x > X_MAX + 40 || line->x < X_MIN - 40) {
        delete_object(line);
        return;
    }

    put_line_vertical(line->y - 5, line->y + 5, line->x);
}

void add_line_obstacle() {
    Object* line = add_object(127, 16, NULL, line_obstacle_ai, NULL, 0); 
    if (line)
        line->x_speed = -3;

}

void line_obstacle_horizontal_ai(Object* line) {
    update_object_general(line);

    if (line->y > Y_MAX + 40 || line->y < Y_MIN - 40) {
        delete_object(line);
        return;
    }

    put_line_horizontal(line->x - 3, line->x + 3, line->y);
}

void add_line_obstacle_horizontal() {
    Object* line = add_object(40, 60, NULL, line_obstacle_horizontal_ai, NULL, 0);
    if (line)
        line->y_speed = -3;
}

void portal_ai(Object* portal) {
    update_object_general(portal);

    // draw the portal: a rectangle of width 5 at (x, y - 10) and at (x, y + 10)
    // the rectangles go slightly out of bounds

    if (portal->x > X_MAX + 40 || portal->x < X_MIN - 40) {
        delete_object(portal);
        return;
    }

    int offset = random_range(-2, 2);

    put_rectangle(portal->x - 2, portal->y - 11 + offset, portal->x + 2, -2);
    put_rectangle(portal->x - 2, portal->y + 11 + offset, portal->x + 2, SCREEN_HEIGHT + 2);
}

void add_portal() {
    Object* portal = add_object(140, 16, NULL, portal_ai, NULL, 0);
    if (portal)
        portal->x_speed = -3;
}