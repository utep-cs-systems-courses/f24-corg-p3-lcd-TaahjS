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

int main(){

}