#include <stdint.h>
#include "object.h"

#ifndef MIPSLABDATA_H
#define MIPSLABDATA_H
uint8_t buf[4][128]; // main buffer
Sprite ball;
Object test_ball;


// GLOBAL VARIABLES FOR JOYSTICK
#define JOY_X_NEUTRAL (762)
#define JOY_Y_NEUTRAL (799)
#define JOY_XY_DEVIATION (10)

#endif