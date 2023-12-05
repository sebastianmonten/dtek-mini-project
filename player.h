#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "adc.h"

Object player;

void player_ai(Object* pl);
bool player_check_collision_obj(Object* pl, Object* obj);
bool get_player_alive();
void set_player_alive(bool val);

#endif