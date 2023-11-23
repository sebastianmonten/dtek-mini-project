#include "object.h"

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

void draw_object(void* p) {
    Object* o = (Object*) p;
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