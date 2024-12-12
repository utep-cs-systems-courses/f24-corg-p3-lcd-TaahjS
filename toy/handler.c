#include <msp430.h>
#include "handler.h"
#include "libTimer.h"

void buzzer_init(){
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}

void buzzer_set_period(short cycles){ //how frequently the buzzer buzzes
  CCR0 = cycles;
  CCR1 = cycles >> 1;
}

void switch_init(){
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;

  switch_interrupt_handler();
}

int switch_interrupt_handler(){
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);

  char b1 = (p2val & SW1) ? 0:SW1;
  char b2 = (p2val & SW2) ? 0:SW2;
  char b3 = (p2val & SW3) ? 0:SW3;
  char b4 = (p2val & SW4) ? 0:SW4;

  if(b1){ //pressed
    return -1; //left
  }else if(b2){
    return 1; //right
  }else if(b3){
    return -8; //up
  }else if(b4){
    return 8; //down
  }else{
    return 0;
  }
}

void led_init(){ //sets up P1OUT and turns off LED
  P1DIR |= LED;
  P1OUT &= ~LED;
}

void led_flip(){ //turns off or turns on
  P1OUT ^= LED;
}
