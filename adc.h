#ifndef ADC_H
#define ADC_H


int joy_x;
int joy_y;
int joy_sw;

void delay(int cyc);
int adc_setup(void);
int adc_at_pin(int pin_num);

#endif