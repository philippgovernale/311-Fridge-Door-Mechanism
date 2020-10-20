#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

#define BLINK_LIGHT_TIME 500 /*blink light frequency of 1 Hz*/
#define DOOR_UNATTENDED_TIME 30000
#define DUTY_CYCLE_10_ON_T 5 /*given freq of 20 Hz (0.1 * 1/20)*/
#define DUTY_CYCLE_10_OFF_T 45


/*counts number of ms. Put this in header with extern label*/
volatile uint8_t timer_count_led;
volatile uint8_t timer_count_open_PWM;
volatile uint16_t timer_count_unattended;

volatile uint8_t PWM_state_high; /*used to indicate whether PWM is low/high*/

volatile uint8_t door_unattended = 0;

/* Initialises timer for use of PWM (1 ms timer)

   - Sets CTC (Clear on compare match mode) mode
   - Sets clock prescaler to 64
   - Sets (8 bit) count value to 250 on Output Compare Match A register
*/
void timer_0_PWM_init(){
	TCCR0A = 0x02;
	TCCR0B = 0x03;
	OCR0A = 250;
}

void timer_2_init(){
	TCCR2A = 0x02;
	TCCR2B = 0x04;
	OCR2A = 250;
}

/*
void timer_2_sleep_init(){
	TCCR2A = 
	TCCR2B =
	
}
*/

void set_door_unattended_and_led_interrupt(){
	TIMSK2 |= (1 << OCIE2B);
	timer_count_unattended = 0;
	TCNT2 = 0;
}


void set_door_open_interrupt(){
	TIMSK0 |= (1 << OCIE0A);
	timer_count_open_PWM = 0;
}

void clear_door_open_interrupt(){
	TIMSK0 &= ~(1 << OCIE0A);
	//also close the switch if open
	switch(0, DOOR_OPEN);
}


void clear_door_unattended_interrupt(){
	TIMSK2 &= ~(1 << OCIE2B);
}

/*OBSOLETE: for ~1s sleep*/
void setup_sleep(){
	CLKPR |= (1 << CLKPS3); // setup 256 clock divider
	
	// Normal mode
	TCCR2A &= ~(1 << WGM21); 
	TCCR2A &= ~(1 << WGM20);
	TCCR2B &= ~(1 << WGM22);
	
	/*64 prescaler*/
	TCCR2B |= (1 << CS22);
	TCCR2B |= (1 << CS21);
	TCCR2B &= ~(1 << CS20);
}


void WDT_init(){
	/*Need to make sure WDTON fuse is programmed*/
	WDTCSR |= (1 << WDCE| 1 << WDE);
	
	WDTCSR |= (1 << WDP2) | (1 <<WDP1); //1 s timing
	WDTCSR &= ~(1 << WDE); //interrupt mode
}

void enable_WDT_interrupt(){
	/*Interrupt Enable*/
	WDTCSR |= (1 << WDIE);
}

void disable_WDT_interrupt(){
	WDTCSR &= ~(1 << WDIE);
}

void timer_wait(uint32_t milliseconds){
	uint32_t timer_overflows = 0;

	//Reset Count
	TCNT0 = 0;

	while(timer_overflows < milliseconds){

		/*check whether Output Compare A match flag is set in Timer interrupt Flag register*/
		if(TIFR0 & (1 << OCF0A)){
			timer_overflows++;

			//Clear OCF0A in TIFR0 register by writing 1 to it
			TIFR0 |= (1 << OCF0A);
		}
	}
}

/*Watchdog timer (for sending to sleep)*/
ISR(WDT_vect){
	;
}

/* interrupt to provide 10% duty cycle when door is open*/
ISR(TIMER0_COMPA_vect){
	timer_count_open_PWM++;
	
	if (PWM_state_high && timer_count_open_PWM == DUTY_CYCLE_10_ON_T){
		switch(0, DOOR_OPEN);
		timer_count_open_PWM = 0;
		PWM_state_high = 0;
	}
	
	else if (!PWM_state_high && timer_count_open_PWM == DUTY_CYCLE_10_OFF_T){
		switch(1, DOOR_OPEN);
		timer_count_open_PWM = 0;
		PWM_state_high = 1;
	}
}

/*measure time door left open*/
/*flashing LED timer*/
ISR(TIMER2_COMPA_vect){
	timer_count_unattended++;
	timer_count_led++;

	if (timer_count_unattended == BLINK_LIGHT_TIME){
		PORTB ^= (1 << PB4); /*toggle red led*/
		timer_count_unattended = 0;
	}
	
	if (timer_count_led == DOOR_UNATTENDED_TIME){
		door_unattended = 1;
	}
}
