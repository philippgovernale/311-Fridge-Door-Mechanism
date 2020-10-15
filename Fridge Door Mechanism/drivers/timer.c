/*
 * timer.c
 *
 * Created: 10/13/2020 9:26:57 PM
 *  Author: philipp
 */
#include "avr/io.h"

#define BLINK_LIGHT_TIME 500
#define DOOR_UNATTENDED_TIME 30000;
/*counts number of ms. Put this in header with extern label*/
volatile uint8_t timer_count_led;
volatile uint16_t timer_count_unattended;

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

void timer_2_initialise(){
	TCCR2A = 0x02;
	TCCR0B = 0x03;
	OCR0A = 250;
	OCR0B = 250;
}

void set_door_unattended_interrupt(){
	TIMSK |= (1 << OCIE2B);
	timer_count_unattended = 0;
	TCNT2 = 0;
}

void set_flashing_led_interrupt(){
	/*enable interupt vector */
	TIMSK |= (1 << OCIE2A);
	timer_count_led = 0;
	TCNT2 = 0; //reset count
}

void clear_door_unattended_interrupt(){
	TIMSK &= ~(1 << OCIE2B);
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

/*measure time door left open*/
ISR(TIMER_2_COMPB_VECT){
	timer_count_led++;

	if (timer_count == DOOR_UNATTENDED_TIME){
		door_unattended = 1;
	}
}


/*flashing LED timer*/
ISR(TIMER_2_COMPA_VECT){
	timer_count_unattended++;

	if (timer_count == BLINK_LIGHT_TIME){
		PORTB ^= (1 << PB4); /*toggle red led*/
	}
}
