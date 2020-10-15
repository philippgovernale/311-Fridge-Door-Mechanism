/*
 * timer.c
 *
 * Created: 10/13/2020 9:26:57 PM
 *  Author: philipp
 */
#include "avr/io.h"

#define BLINK_LIGHT_TIME 500
/*counts number of ms. Put this in header with extern label*/
volatile uint8_t timer_count;

/* Initialises timer for use of PWM (1 ms timer)

   - Sets CTC (Clear on compare match mode) mode
   - Sets clock prescaler to 64
   - Sets (8 bit) count value to 250 on Output Compare Match A register
*/
void timer_0_PWM_initialise(){
	TCCR0A = 0x02;
	TCCR0B = 0x03;
	OCR0A = 250;
}

void timer_2_led_blink_initialise(){
	TCCR2A = 0x02;
	TCCR0B = 0x03;
	OCR0A = 250;
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
			TIFR0 |= (1 << OCF0A)
		}
	}
}


/*flashing LED timer*/
ISR(TIMER_2_COMPA_VECT){
	timer_count++;

	if (timer_count == BLINK_LIGHT_TIME){
		PORTB ^= (1 << PB4); /*toggle red led*/
	}
}
