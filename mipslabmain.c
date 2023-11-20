/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#include "graphics.h"

void display_buf(void) {
	int i;
	for (i = 0; i<4; i++) {
		display_image(i*32, buf[i]);
	}
}

void clear_buf(void) {
	int i;
	int j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 128; j++) {
			buf[i][j] = ~0;
		}
	}
}

void draw_to_buf(int x, int y, Sprite s) {
	int sx;
	int sy;
	for (sy = 0; sy < s.height; sy++) {
		for (sx = 0; sx < s.width; sx++) {
			if (s.data[sy][sx]) {
				int page = (x + sx) / 32;
				if (page > 3) break;
				if (sy + y >= 32) break;

				int local_x = (x + sx) % 32;

				int index = ((y + sy) / 8) * 32 + local_x;
				// int bitindex = 7 - ((y + sy) % 8);
				int bitindex = ((y + sy) % 8);

				buf[page][index] &= ~(1 << bitindex);
				
			}
			
		}
	}

}

void draw_to_buf_test(void) {
	// int bitindex = 7;
	// int page = 0;
	// int index = 32;
	buf[0][1] &= ~(1 << (7- 0));
	buf[0][1] &= ~(1 << (7- 1));
	buf[0][1] &= ~(1 << (7- 2));
	buf[0][1] &= ~(1 << (7- 3));
	buf[0][1] &= ~(1 << (7- 4));
	buf[0][1] &= ~(1 << (7- 5));
	buf[0][1] &= ~(1 << (7- 6));
	buf[0][1] &= ~(1 << (7- 7));
	
	buf[0][32] &= ~(1 << (7- 0));
	buf[0][32] &= ~(1 << (7- 1));
	buf[0][32] &= ~(1 << (7- 2));
	buf[0][32] &= ~(1 << (7- 3));
	buf[0][32] &= ~(1 << (7- 4));
	buf[0][32] &= ~(1 << (7- 5));
	buf[0][32] &= ~(1 << (7- 6));
	buf[0][32] &= ~(1 << (7- 7));
}


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
	
	// labinit(); /* Do any lab-specific initialization */
	int y = 0;
	int x = 0;

	int y2 = 0;
	int x2 = 110;

	int v = 1;
	int v2 = -1;

	
	// delay(1000);
	// clear_buf();
	// draw_to_buf(0, 0, ball);
	// display_buf();
	// delay(1000);
	// draw_to_buf(32, 0, ball);
	// display_buf();
	// delay(1000);
	// draw_to_buf(64, 0, ball);
	// display_buf();
	// delay(1000);
	// draw_to_buf(96, 0, ball);
	// display_buf();
	while( 1 )
	{	
		draw_to_buf(x, y, ball);
		// put_line(0, 0, 4, 4);
		draw_to_buf(x2, y2, ball2);
		display_buf();
		clear_buf();
		delay(50);
		x += v;
		x2 += v2;

		if (x > 128 - ball.width) {
			v = -v;
		}

		if (x2 < 0) {
			v2 = -v2;
		}
	//   labwork(); /* Do lab-specific things again and again */

	}
	return 0;


}







