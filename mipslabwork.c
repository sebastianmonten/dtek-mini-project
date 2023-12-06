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

#include <stdio.h> /* test */

// #include "time4io.h" // include
#include "adc.h"
#include "player.h"
#include "rand.h"
#include "common.h"

// OUR GLOBAL VARIABLES
int timeoutcount = 0;
int prime = 1234567;
int time_since_last_sw_press = 0;
int time_debounce_sw = 1;
int total_time_elapsed = 0; // initialize total time counter here
int game_time = 0;

bool you_pressed = 0;

// GOBAL VARIABLES FOR GAME STATE
enum GameState_e {START, HIGHSCORE, GAME, DEATH, ENTER_HIGHSCORE};
typedef enum GameState_e GameState;
GameState gamestate = START;
int time_switch_between_player_graphics = 10;

// GLOBAL VARIABLES FOR START MENU
enum StartSel_e {PLAY, SHOW_HIGHSCORES};
typedef enum StartSel_e StartSel;
StartSel start_sel = PLAY;  // initial game state

// GLOBAL VARIABLES FOR DEATH MENU
enum DeathSel_e {DEATH_SEL_START, DEATH_SEL_ENTER_HIGHSCORE};
typedef enum DeathSel_e DeathSel;
DeathSel death_sel = DEATH_SEL_START;  // initial game state

// GLOBAL VARIABLES FOR ENTER HIGHSCORE MENU
int cursor_x = 0;
int cursor_y = 0;
int cursor_blink_time = 0;
int cursor_blink_max = 2;
int cursor_on = 0;
int time_since_last_cursor_move = 0;
int time_cursor_debounce = 6;
char new_name[3] = "   ";
int new_name_index = 0;

// GLOBAL VARIABLES FOR HIGHSCORE VIEW
#define MAX_ENTRIES 3
struct Entry {
    char name[4];  // 3 characters + null terminator
    int score;
};
struct Entry scores[3] = {
    {"---", 0},
    {"---", 0},
    {"---", 0}
};

// GLOBAL FOR BALL COORD
int y_global = 0;
int x_global = 0;
// END OF OUR GLOBAL VARIABLES

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";


void reset_cursors(void) {
  cursor_x = 0;
  cursor_y = 0;
  death_sel = DEATH_SEL_START;
  start_sel = PLAY;
  new_name_index = 0;

  int i;
  for (i = 0; i < 3; i++) {
    new_name[i] = ' ';
  }
}

void start(void) {

  object_count = 0; // reset
  rand_seed = total_time_elapsed;


  if (joy_y > JOY_Y_NEUTRAL+JOY_XY_DEVIATION && start_sel == PLAY) {
    start_sel = SHOW_HIGHSCORES;
  } else if (joy_y < JOY_Y_NEUTRAL-JOY_XY_DEVIATION && start_sel == SHOW_HIGHSCORES) {
    start_sel = PLAY;
  }

  switch (start_sel)
  {
  case PLAY:
    display_string(1, "> PLAY");
    display_string(2, "  HIGHSCORES");
    if (sw_pressed) {
      gamestate = GAME;
      sw_pressed = 0;
      reset_cursors();
      game_time = 0; // reset game time counter
    }
    break;

  case SHOW_HIGHSCORES:
    display_string(1, "  PLAY");
    display_string(2, "> HIGHSCORES");
    if (sw_pressed) {
      gamestate = HIGHSCORE;
      sw_pressed = 0;
      reset_cursors();
    }
    break;
  
  default:
    break;
  }
}



void game(void) {
  // update all objects
  int i;
  for (i = 0; i < MAX_OBJECTS; i++)
    if (objects[i].active)
      update_object(&objects[i]);
  // display_string(0, "   GAME ON!");

  update_object(&player);

  // check if player is alive
  if (! get_player_alive()) {
    gamestate = DEATH;
    reset_cursors();
    set_player_alive(1);
  }
  
  // display object count
  // char str[16];

  display_string(3, itoaconv(game_time));

}

char* concatenateEntryToString(int index) {
    static char result[17];  // Make sure the buffer is large enough
    int offset = 0;

    // Copy the name to result
    int i;
    for (i = 0; i < 3 && scores[index].name[i] != '\0'; ++i) {
        result[offset++] = scores[index].name[i];
    }

    // Add the colon and space
    result[offset++] = ':';
    result[offset++] = ' ';

    // Convert the score to a string and copy it to result
    int score = scores[index].score;
    int score_digits = 0;

    // Count the number of digits in the score
    do {
        ++score_digits;
        score /= 10;
    } while (score != 0);

    // Convert the score to a string and copy it to result
    score = scores[index].score;
    for (i = 0; i < score_digits; ++i) {
        result[offset + score_digits - i - 1] = '0' + score % 10;
        score /= 10;
    }

    // Null-terminate the string
    result[offset + score_digits] = '\0';

    return result;
}


// old function for enter entry
// void enter_entry(struct Entry scores[], const struct Entry *newEntry) {
//     // Find the index of the first entry with a lower or equal score
//     int insertIndex = 0;
//     while (insertIndex < MAX_ENTRIES && newEntry->score <= scores[insertIndex].score) {
//         insertIndex++;
//     }

//     // Shift scores down to make space for the new entry
//     int i;
//     for (i = MAX_ENTRIES - 1; i > insertIndex; --i) {
//         scores[i] = scores[i - 1];
//     }

//     // Insert the new entry at the correct place
//     scores[insertIndex] = *newEntry;
// }

// new function for enter entry
int compare_names(const char *name1, const char *name2) {
  int i;
  for (i = 0; i < 3; ++i) {
      if (name1[i] < name2[i]) {
          return -1;
      } else if (name1[i] > name2[i]) {
          return 1;
      } else if (name1[i] == '\0' || name2[i] == '\0') {
          return 0;
      }
  }
  return 0;  // Names are equal
}

int find_entry_index(const struct Entry scores[], const char *name) {
  int i;
  for (i = 0; i < MAX_ENTRIES; ++i) {
      if (compare_names(scores[i].name, name) == 0) {
          return i;  // Entry found
      }
  }
  return -1;  // Entry not found
}

void enter_entry(struct Entry scores[], const struct Entry *newEntry) {
    // Check if the name already exists in scores
    int existingIndex = find_entry_index(scores, newEntry->name);

    if (existingIndex != -1) {
        // Name already exists, compare scores
        if (newEntry->score > scores[existingIndex].score) {
            // Update the existing entry with the new score
            scores[existingIndex].score = newEntry->score;
        } else {
            return;  // No need to insert or re-sort if the new score is not higher
        }
    } else {
        // Name does not exist, find the index of the first entry with a lower or equal score
        int insertIndex = 0;
        while (insertIndex < MAX_ENTRIES && newEntry->score <= scores[insertIndex].score) {
            insertIndex++;
        }

        // Shift scores down to make space for the new entry
        int i;
        for (i = MAX_ENTRIES - 1; i > insertIndex; --i) {
            scores[i] = scores[i - 1];
        }

        // Insert the new entry at the correct place
        scores[insertIndex] = *newEntry;
    }

    // Re-sort the scores if needed
    int i;
    for (i = 1; i < MAX_ENTRIES; ++i) {
        int j = i;
        struct Entry temp = scores[i];
        while (j > 0 && temp.score > scores[j - 1].score) {
            scores[j] = scores[j - 1];
            --j;
        }
        scores[j] = temp;
    }
}

void highscores(void) {
  if (sw_pressed) {
    gamestate = START;
    sw_pressed = 0;
    reset_cursors();
  }
  display_string(0, "          > BACK");
  display_string(1, concatenateEntryToString(0));
  display_string(2, concatenateEntryToString(1));
  display_string(3, concatenateEntryToString(2));
}


void death(void) {
  if (joy_y > JOY_Y_NEUTRAL+JOY_XY_DEVIATION && death_sel == DEATH_SEL_START) {
    death_sel = DEATH_SEL_ENTER_HIGHSCORE;
  } else if (joy_y < JOY_Y_NEUTRAL-JOY_XY_DEVIATION && death_sel == DEATH_SEL_ENTER_HIGHSCORE) {
    death_sel = DEATH_SEL_START;
  }

  // remove all objects
  int i;
  for (i = 0; i < MAX_OBJECTS; i++)
    objects[i].active = false;

  display_string(0, "  GAME OVER!");
  switch (death_sel)
  {
  case DEATH_SEL_START:
    display_string(2, "> START MENU");
    display_string(3, "  SAVE SCORE");
    if (sw_pressed) {
      gamestate = START;
      sw_pressed = 0;
    }
    break;

  case DEATH_SEL_ENTER_HIGHSCORE:
    display_string(2, "  START MENU");
    display_string(3, "> SAVE SCORE");
    if (sw_pressed) {
      gamestate = ENTER_HIGHSCORE;
      sw_pressed = 0;
    }
    break;
  
  default:
    break;
  }

}



void enter_highscore(void) {
  int cursor_x_max = 12;
  int cursor_y_max = 1;
  char Strings[2][15] = {" ABCDEFGHIJKLM", " NOPQRSTUVWXYZ"};

  if (time_since_last_cursor_move > time_cursor_debounce) {
    if (joy_y > JOY_Y_NEUTRAL+JOY_XY_DEVIATION && cursor_y < cursor_y_max) {
      cursor_y++;
      time_since_last_cursor_move = 0;
    } else if (joy_y < JOY_Y_NEUTRAL-JOY_XY_DEVIATION && cursor_y > 0) {
      cursor_y--;
      time_since_last_cursor_move = 0;
    }
    if (joy_x > JOY_X_NEUTRAL+JOY_XY_DEVIATION && cursor_x < cursor_x_max) {
      cursor_x++;
      time_since_last_cursor_move = 0;
    } else if (joy_x < JOY_X_NEUTRAL-JOY_XY_DEVIATION && cursor_x > 0) {
      cursor_x--;
      time_since_last_cursor_move = 0;
    }
  }

  time_since_last_cursor_move ++;


  char selected_char = Strings[cursor_y][cursor_x + 1];
  if (cursor_on) {
    Strings[cursor_y][cursor_x + 1] = '_';
  }
  if (sw_pressed && new_name_index < 3) {
    new_name[new_name_index] = selected_char;
    new_name_index++;
    sw_pressed = 0;

    if (new_name_index == 3) {
      struct Entry new_entry;
      int i;
      for (i = 0; i < 3; i++) {
        new_entry.name[i] = new_name[i];
      }
      new_entry.score = game_time;
      enter_entry(scores, &new_entry);
      gamestate = START;
      reset_cursors();
    }
  }

  display_string(0, new_name);
  display_string(2, Strings[0]);
  display_string(3, Strings[1]);
  
}


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
    if (joy_sw) {time_since_last_sw_press++;} // increment time since last press if not currently pressed
    if (joy_sw == 0 & time_since_last_sw_press > time_debounce_sw) {
      sw_pressed = 1;
      time_since_last_sw_press = 0;
    }


    cursor_blink_time++;

    total_time_elapsed++;

    if (gamestate == GAME) {
      game_time++;
      // if (game_time % time_switch_between_player_graphics == 0) {
      //   if ()
      // }
    }


  }

  if (cursor_blink_time >= cursor_blink_max) {
    cursor_blink_time = 0;
    cursor_on = ! cursor_on;
  }
   

  if (timeoutcount == 10) {
    if (!triple) {
      tick(&mytime);
    }



    // THIS BLOCK HAPPWNS EVERY 1000ms
    // time2string(textstring, mytime);
    // display_string(0, "joy x = ");
    // display_string(1, itoaconv(joy_x));
    // display_update();
    // display_update();
    // display_image(96, icon); // add doge icon 

    ////////////////////////////////////////////
    timeoutcount = 0;
    if (gamestate == GAME) {add_portal();}
  } else if (timeoutcount == 5 && gamestate == GAME && game_time > 70) {
    add_line_obstacle_horizontal();
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

    // display_string(0, "START/HIGHSCORES");
    // display_string(1, "START/HIGHSCORES");
    
    // // display_string(3, "START/HIGHSCORES");
    // update_object(&player);


    // MAIN SWITCH CASE FOR GAME STATE
    switch (gamestate)
    {
    case START:
      // display_string(0, "In start menu");
      start();
      break;
    
    case HIGHSCORE:
      highscores();
      break;

    case GAME:
      game();
      break;

    case DEATH:
      death();
      break;

    case ENTER_HIGHSCORE:
      enter_highscore();
      break;
    
    default:
      break;
    }
    // 

    display_buf();
    clear_buf();


}


