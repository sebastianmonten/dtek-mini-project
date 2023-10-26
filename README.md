# dtek-mini-project

## Objective and Requirements
Develop a game where the player controls a spaceship with a joystick. Enemies come at you randomly, and you use the button on the joystick to shoot at them. Obstacles move from right to left on the screen to simulate that the player is flying rapidly and the player can move the ship in X and Y directions across the screen using the joystick's roll and pitch. The player gains points for time stayed alive and/or enemies killed (and near misses?). This high score is stored and displayed. Two players can play at the same time and collaborate. In this mode the enemies are tougher to shoot down. The player can also select a higher difficulty level, which makes the ship fly faster and increases the amount of enemies and/or the enemies resilience. 

## Must-haves
Pixel-by-pixel movement, smooth animations.
Reading analog input from a joystick.
Display the game on the built-in OLED graphical display.
Objects on the screen larger than 2x2 pixels.
Single player and two player mode.
Different difficult levels, a higher difficulty level increases the amount and speed of obstacles and enemies. 
Highscore, save to username initials.

## Can-haves
Display the game on a larger external screen.
Highscore stored in EEPROM using I2C

## Solution
We will use the e ChipKIT Uno32  and its IO shield together with a joystick module. The project will be developed mainly in C (using MIPS assembly where necessary), using the MCB32 toolchain and the code provided for Lab 3. Frame updates will be controlled by timers and their corresponding interrupts, and hardware input will be handled with the help of interrupt handlers. The graphics will be displayed on either the Basic IO shield's OLED display or on an external screen. When it comes to the joystick, we will read the analog output from the joystick with the MIP32 Analog-to-Digital Converter. Its x and y output will translate to the x and y movement in the game, and the switch output triggers fire. 

## Verification
We will verify the project by testing it extensively during all stages of development. We will make sure that we can write pixels to the screen, read joystick input, and that the game logic works. When we say game logic we mean things like that the player and enemies move smoothly in an expected manner, that enemies take damage from player fire, that the player is damaged upon colliding with an enemy or its projectile, that enemies spawn correctly and that the game indeed does becomes harder with 2 players.

## Deadlines
12 October extended abstract for feedback
week 50 (11 December first slot) project and final abstract complete on EXPO
