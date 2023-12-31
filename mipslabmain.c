/* mipslabmain.c

   This file is written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall
   modified 2023 by Anders Mouanga and Sebastian Montén

*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#include "graphics.h"

#include "mipslabdata.h"

#include "object.h"
#include "adc.h"






int main(void) {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;
	
	// seb disabled:
	display_init();
	// display_string(0, "KTH/ICT lab");
	// display_string(1, "in Computer");
	// display_string(2, "Engineering");
	// display_string(3, "Welcome!");
	display_update();
	
	// display_image(96, icon);
	// display_image(0, icon2);
	// display_image2(0,0);
	
	labinit(); /* Do any lab-specific initialization */
    joy_x = adc_at_pin(4); // A1
    joy_y = adc_at_pin(8); // A2
    joy_sw = adc_at_pin(10); // A3

	
	while( 1 )
	{	
		// draw_to_buf(x, y, ball);
		// put_line(x+0, y+0, x+16, y+16);
		// put_line(0, 0, 16, 16);
		// display_buf();
		// clear_buf();
		
		// x += 1;

		// update_object(&test_ball);
		delay(50);
	  	labwork(); /* Do lab-specific things again and again */

	}
	return 0;


}