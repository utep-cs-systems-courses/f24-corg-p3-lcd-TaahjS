/*
  Todo:
  State 1
  Turn off LED, pause timer
  Displays text on screen based on what triggered it (idle, start, replay)
  Have player be able to move for practice. Show controls.
  If player hits text, either RESUME or begin a new game
  When exit state, emit noise
  If start new game reset everything
  Display time upon game completion
  
  
  State 2
  Maze = array that is displayed on screen (like CSIntro proj)
  Set up movements to move left, right, up, down array that is then projected on screen
  Only change part of screen that includes movement (original position, new position)
  Sound for every movement. If hit wall, special noise. If succeed, special noise.
  Constantly blink LED to indicate actively playing (Every 1 second, can implement in assy)
  If no movement for 30s = go to state 1 as idle
  
 */

#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "handler.h"

//State 1 Functions
void pause_screen(int type);
void display_time();

//State 2 Functions
void display_maze(); //call once upon state transition

void move(int dir); //only can move to 1's and 2's
void move_fail(); //noise
void move_success(); //noise

int time[] = {0, 0}, pos = 0, interrupt = 0, state = 1;
int maze[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, -1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 1, 0, 1, 0,
  0, 1, 1, 1, 0, 1, 2, 0,
  0, 1, 0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
}; //-1 = player, 0 = wall, 1 = passable, 2 = exit.

int main(){
  configureClocks();
  lcd_init();
  led_init();
  switch_init();
  buzzer_init();

  buzzer_set_period(0);
  clearScreen(COLOR_RED);

  or_sr(0x18);
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    move(switch_interrupt_handler());
  }
}

void
__interrupt_vec(WDT_VECTOR) WDT(){
  if(state == 1){ //if time is paused
    goto skip;
  }
  interrupt++;
  if(interrupt >= 250){
    time[1]++;
    if(time[1] == 60){
      time[1] = 0;
      time[0]++;
      if(time[0] == 60){
	time[0] = 0;
      }
    }
    interrupt = 0;
  }
 skip:
  display_time();
}

void move(int dir){
  /*
    1 = left(-1), 2 = right(+1), 3 = up(-8), 4 = down(+8)
   */

}

void display_time(){

}
