#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "handler.h"

//State 1 Functions
void start_screen();

//State 2 Functions
void display_maze(); //call once upon state transition
void display_time(); //updates screen every time update
void draw_square(int coord);

void move(); //player can only move to 1's and 2's
void move_fail(int on); //play or end noise
void update_screen(); //move player pixels

//State 3 Functions
void success(); //game end
void game_over();

char time_string[] = "00:00";
int time[] = {0, 0, -1}, pos = 9, newpos = 9, interrupt = 0, state = 1, timer = 0;
int maze[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, -1, 1, 1, 1, 1, 1, 0,
  0, 1, 0, 1, 0, 0, 1, 0,
  0, 1, 0, 1, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 1, 1, 1,
  0, 1, 1, 1, 0, 1, 0, 1,
  0, 1, 0, 1, 1, 1, 2, 1,
  0, 0, 0, 0, 0, 0, 0, 0,
}; //-1 = player, 0 = wall, 1 = passable, 2 = exit

int main(){
  configureClocks();
  lcd_init();
  led_init();
  switch_init();
  buzzer_init();

  buzzer_set_period(0);
  clearScreen(COLOR_BLACK);
  enableWDTInterrupts();

  start_screen();

  or_sr(0x18);
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    move();
  }
}

void
__interrupt_vec(WDT_VECTOR) WDT(){
  if(state == 2){
    interrupt++;
    if(interrupt >= 250){
      interrupt = 0;
      time[1]++;
      move_fail(0);
      
      if(time[1] == 60){
	time[1] = 0;
	time[0]++;
      }
      if(time[0] == 60){
	time[0] = 0;
      }
      display_time();
    }
  }else if(state == 3){
    interrupt++;
    if(interrupt >= 188){
      if(timer == 0){
	clearScreen(COLOR_BLACK);
      }
      timer = blink_screen(timer);
      interrupt = 0;
    }
  }
}

void start_screen(){
  drawString8x12(20, 1,"MAZE GAME", COLOR_BLUE, COLOR_BLACK);
  drawString8x12(1, 32, "SW1: LEFT", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(1, 44, "SW2: RIGHT", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(1, 56, "SW3: UP", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(1, 68, "SW4: DOWN", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(1, 92, "PRESS ANYTHING", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(1, 104, "TO BEGIN", COLOR_WHITE, COLOR_BLACK);
}

void display_maze(){
  for(int i = 0; i < 64; i++){
    draw_square(i);
  }
}

void display_time(){
  time_string[0] = (time[0] / 10) + '0';
  time_string[1] = (time[0] % 10) + '0';
  time_string[3] = (time[1] / 10) + '0';
  time_string[4] = (time[1] % 10) + '0';
  drawString5x7(50, 120, time_string, COLOR_WHITE, COLOR_BLACK);
}

void draw_square(int coord){ //coord is index in maze
  int row = (coord / 8) * 15;
  int col = (coord % 8) * 15;
  u_int color;

  if(maze[coord] == -1){ //player
    color = COLOR_RED; 
  }else if(maze[coord] == 0){ //wall
    color = COLOR_BLACK;
  }else if(maze[coord] == 1){ //path
    color = COLOR_WHITE;
  }else if(maze[coord] == 2){ //exit
    color = COLOR_BLUE;
  }else{
    color = COLOR_YELLOW;
  }

  for(int i = 0; i < 15; i++){
    for(int j = 0; j < 15; j++){
      drawPixel(col + j, row + i, color);
    }
  }
 
}

void move(){
  if(state == 3){ //if game is over, we can't move anymore
    return;
  }

  if(state == 1){
    state++;
    clearScreen(COLOR_BLACK);
    display_maze();
    display_time();
  }
  
  int step = switch_interrupt_handler();
  if(!step){ //if nothing was pressed
    return;
  }
  
  newpos = pos + step;
  if(newpos < 0 || newpos > 63 || maze[newpos] == 0){ //if invalid move
    led_flip();
    move_fail(1);
    return;
  }else if(maze[newpos] == 2){ //at exit
    led_flip();
    success();
    return;
  }else{ //regular move
    update_screen();
  }
  
}

void move_fail(int on){
  if(on){
    time[2] = time[1];
    //buzzer_set_period(1000);
  }else if(!on && time[2] != time[1]){
    time[2] = -1;
    buzzer_set_period(0);
  }
}

void update_screen(){
  maze[newpos] = -1;
  maze[pos] = 1;
  draw_square(newpos);
  draw_square(pos);
  pos = newpos;
}

void success(){
  state++;
  maze[pos] = 1;
  draw_square(pos);
}

void game_over(){
  drawString8x12(30, 50, "GAME OVER", COLOR_BLUE, COLOR_BLACK);
}
