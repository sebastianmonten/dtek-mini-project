#ifndef ADC_H
#define ADC_H


#include <stdbool.h>

int joy_x;
int joy_y;
int joy_sw;

void delay(int cyc);
int adc_setup(void);
int adc_at_pin(int pin_num);

// GLOBAL VARIABLES FOR JOYSTICK
#define JOY_X_NEUTRAL (762)
#define JOY_Y_NEUTRAL (799)
#define JOY_XY_DEVIATION (10)
bool sw_pressed;
////

#endif