#include <pic32mx.h>
#include <stdbool.h>

// void delay(int cyc) {
// 	volatile int i;
// 	for(i = cyc; i > 0; i--);
// }

// GLOBAL VARIABLES FOR JOYSTICK
bool sw_pressed = 0;
////


int adc_at_pin(int pin_num) {
    // display_string(3, "now running adc");
	// display_update();



	unsigned int speed;
	// unsigned int sweep = 0x1;

    /* PORTB.2 is analog pin with potentiometer*/
	// in original project on github: pin_num = 2
	AD1PCFG = ~(1 << pin_num);
	TRISBSET = (1 << pin_num);
	/* Use pin 2 for positive */
	AD1CHS = (pin_num << 16);
	
	/* Data format in uint32, 0 - 1024
	Manual sampling, auto conversion when sampling is done
	FORM = 0x4; SSRC = 0x7; CLRASAM = 0x0; ASAM = 0x0; */
	AD1CON1 = (0x4 << 8) | (0x7 << 5);
	
	AD1CON2 = 0x0;
	AD1CON3 |= (0x1 << 15);
	
	/* Set up output pins */
	ODCE = 0x0;
	TRISECLR = 0xFF;
	
	/* Turn on ADC */
	AD1CON1 |= (0x1 << 15);
	
	// for(;;) {
		/* Start sampling, wait until conversion is done */
		AD1CON1 |= (0x1 << 1);
		while(!(AD1CON1 & (0x1 << 1)));
		while(!(AD1CON1 & 0x1));
		
		/* Get the analog value */
		speed = ADC1BUF0;
		
		// PORTE = sweep;
		// if((sweep <<= 1) >= 0x100)
		// 	sweep = 1;
		// delay(speed * 1000);
	// }
	
	return (int) speed;
}
