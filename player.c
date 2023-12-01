#include "player.h"
#include "mipslabdata.h"
#define NULL ((void*)0)

Object player = {
    .sprite = &ball, // from mipslabdata.h
    .x = 2,
    .y = 2,
    .x_speed = 0,
    .y_speed = 0,
    .bonus_data = NULL,
    .bonus_data_size = 0,
    .update_func = player_ai
};

void player_move_script(Object* pl) {
    // calculate player's x direction: left-none-right
    // and player's y direction: up-none-right


    int x_direction, y_direction;

    if (joy_x < 720)        
        x_direction = -1;
    else if (joy_x > 740)   
        x_direction = 1;
    else
        x_direction = 0;
    
    if (joy_y < 750)
        y_direction = -1;
    else if (joy_y > 770)
        y_direction = 1;
    else
        y_direction = 0;

    // pythagoras: if the player is moving in a y direction, their x speed must be lower
    pl->x_speed = y_direction ? 3 * x_direction : 2 * x_direction;

    // same for y
    pl->y_speed = x_direction ? 3 * y_direction : 2 * y_direction;
}

void player_ai(Object* pl) {

    // handle movement input
    player_move_script(pl);

    // check collision with enemies (TBD)


    // finally update the player's position and draw them
    update_object_general(pl);



    
}