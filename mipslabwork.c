/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

// #include "time4io.h" // include
#include "adc.h"
#include "player.h"

// OUR GLOBAL VARIABLES
int timeoutcount = 0;
int prime = 1234567;
// int joy_x = 0;
// int joy_y = 0;
// int joy_sw = 0;

// GLOBAL FOR BALL COORD
int y_global = 0;
int x_global = 0;
// END OF OUR GLOBAL VARIABLES

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr(void)
{ 

  int triple = 0; // 

  //handle SW2
  if (((IFS(0) >> 11)) & 1) {
    triple = 1;
    tick(&mytime);
    tick(&mytime);
    tick(&mytime);
    IFSCLR(0) = 1 << 11; // clear bit 11

    // time2string(textstring, mytime);
    // display_string(3, textstring);
    // display_update();
    // display_image(96, icon); // add doge icon 
  }

  if (((IFS(0) >> 8)) & 1) {
    // happens every 10 ms
    timeoutcount++;
    IFSCLR(0) = 0b100000000; // =  0x100

    // PROBE A1 and A2 for x and y of joystick
    joy_x = adc_at_pin(4); // A1
    joy_y = adc_at_pin(8); // A2
    joy_sw = adc_at_pin(10); // A3
  }
  

  if (timeoutcount == 10) {
    if (!triple) {
      tick(&mytime);
    }

    // THIS BLOCK HAPPWNS EVERY 100ms
    // time2string(textstring, mytime);
    // display_string(0, "joy x = ");
    // display_string(1, itoaconv(joy_x));
    // display_update();
    // display_update();
    // display_image(96, icon); // add doge icon 
    ////////////////////////////////////////////
    timeoutcount = 0;
  }
}

/* Lab-specific initialization goes here */
void labinit(void)
{
  // initialize Port E so that bits 7 through 0
  // of Port E are set as outputs (i.e., the 8 least significant bits)
  volatile int *trise = (volatile int *)0xbf886100;
  *trise = *trise & ~0xFF; // only set the 8 lsb to 0

  volatile int *trisd = (volatile int *)TRISD;
  *trisd |= (0b1111111 << 5); // only set bits 5 to 11 to 1

  // timer 2 initialization
  T2CON = 0x0;
  TMR2 = 0x0;            // reset the timer value
  PR2 = 31250;           // timeout interval is 31250
  T2CONSET = 0b01110000; // set timer 2 pre scale to 1:256 (bit 6-4 is 111 -> 7)
  
  IPCSET(2) = 0xD; // 1100 set priority of counter 2 interrupt
  
  IFSCLR(0) = 0x00000100; // Clear the timer interrupt status flag of timer 2
  
  IECSET(0) = 0x00000100; // Enable timer interrupts of timer 2, done last

  T2CONSET = 0x8000; // start timer 2
  // end of timer 2 initialization

  // int2 initialization
  IECSET(0) = (1 << 11); // set bit 11
  IPCSET(2) = (0b110 << 26) + (0b11 << 24); // set priority and subpriority of INT2
  IFSCLR(0) = (1 << 11); // clear bit 11 in IFS

  // end of int2 initialization

  enable_interrupt(); // disabled by seb
  return;
}

/* This function is called repetitively from the main program */
void labwork(void)
{
  // prime = nextprime(prime);
  // display_string(0, itoaconv(prime));
  // display_update();
  // display_image(0, icon2); // add doge icon, old was 96



    // int joy_x_threshold_upper = 740;
    // int joy_x_threshold_lower = 720;

    // int joy_y_threshold_upper = 770;
    // int joy_y_threshold_lower = 750;
    
    // if (joy_x < joy_x_threshold_lower && x_global - 3 > 0) {
    //   x_global -= 3;
    // } else if (joy_x > joy_x_threshold_upper && x_global+ ball.width + 3 < SCREEN_WIDTH) {
    //   x_global += 3;
    // }
    // if (joy_y < joy_y_threshold_lower && y_global - 3 > 0) {
    //   y_global -= 3;
    // } else if (joy_y > joy_y_threshold_upper && y_global + ball.height + 3 < SCREEN_HEIGHT) {
    //   y_global += 3;
    // }
    // draw_to_buf(x_global, y_global, ball);
		// display_buf();
    // clear_buf();



    // update_object(&player);
    // display_buf();
    // clear_buf();

    display_string(0, 'x');
    display_buf();
    
}
