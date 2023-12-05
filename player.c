#include "player.h"
#include "mipslabdata.h"
#define NULL ((void*)0)

Object player = {
    .sprite = &ball, // from mipslabdata.h
    .x = 2,
    .y = 2,
    .x_speed = 0,
    .y_speed = 0,
    .bonus_data = &player_alive,
    .bonus_data_size = 1,
    .update_func = player_ai,
    .active = true
};

bool player_alive = true;

void player_move_script(Object* pl) {
    // calculate player's x direction: left-none-right
    // and player's y direction: up-none-right


    int x_direction, y_direction;

    if (joy_x < JOY_X_NEUTRAL-JOY_XY_DEVIATION)        
        x_direction = -1;
    else if (joy_x > JOY_X_NEUTRAL+JOY_XY_DEVIATION)   
        x_direction = 1;
    else
        x_direction = 0;
    
    if (joy_y < JOY_Y_NEUTRAL-JOY_XY_DEVIATION)
        y_direction = -1;
    else if (joy_y > JOY_Y_NEUTRAL+JOY_XY_DEVIATION)
        y_direction = 1;
    else
        y_direction = 0;

    // pythagoras: if the player is moving in a y direction, their x speed must be lower
    pl->x_speed = y_direction ? 3 * x_direction : 2 * x_direction;

    // same for y
    pl->y_speed = x_direction ? 3 * y_direction : 2 * y_direction;
}


// absolutely move this to object.h
bool player_check_collision_obj(Object* pl, Object* obj) {

    if (!obj)
        return false;

    // cant collide with an inactive object
    if (pl->active == false || obj->active == false)
        return false;

    // here we make the assumption that pl's sprite origin is (0, 0)
    int left = pl->x;
    int right = left + pl->sprite->width;

    int top = pl->y;
    int bot = top + pl->sprite->height;

    // (very basic) collision check for if obj has a sprite
    if (obj->sprite)
    {
        int obj_left = obj->x - obj->sprite->x_origin;
        int obj_right = obj_left + obj->sprite->width;
        int obj_top = obj->y - obj->sprite->y_origin;
        int obj_bot = obj_top + obj->sprite->height;

        // first check x collision: 
        // lowest possible x value for obj_left is so that its rightmost point touches the player
        // and highest possible x value is so that its leftmost point touches the player
        if 
        (obj_right >= left && obj_left <= right

        // same for y collision:
        // lowest possible y value for obj_top is so that its bottom point touches the player
        // and highest possible y value is so that its top point touches the player
        && obj_bot >= top && obj_top <= bot)
            return true;
        else
            return false;   
    }
     
    // obj does not have a sprite; check if it's a line obstacle
    else if (obj->update_func == line_obstacle_ai)
    {
        if (left <= obj->x && obj->x <= right)
        // these are hardcoded values; change later
            if (top <= obj->y - 5 && obj->y + 5 >= bot)
                return true;
    }

        

    return false;
}

void player_ai(Object* pl) {

    // handle movement input
    player_move_script(pl);

    // check collision with enemies (TBD)
    int i;
    for (i = 0; i < MAX_OBJECTS; i++)
        if (player_check_collision_obj(pl, &objects[i]))
            *(bool*)(pl->bonus_data) = false; // set player alive to false


    // finally update the player's position and draw them
    update_object_general(pl);



    
}