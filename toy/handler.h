#ifndef buzzer_included
#define buzzer_included
#endif

void buzzer_init();
void buzzer_set_period(short cycles);

#ifndef switches_included
#define switches_included

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15

void switch_init();
int switch_interrupt_handler();

#define LED BIT6

void led_init();
void led_flip();

#endif
