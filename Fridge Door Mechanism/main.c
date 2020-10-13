/*
 * Fridge Door Mechanism.c
 *
 * Created: 10/13/2020 7:51:51 PM
 * Author : ashap
 */ 

#include <avr/io.h>
#include <stdio.h> // do we need this? I added it. We need it if we want to print out anything

enum current_dir = {OPENING, CLOSING};
	
#define TURN_ON(D) PORTB |= (1 << PB0); D==0 ? PORTB |= (1 << PB1) : PORTB &= ~(1 << PB1) 
#define TURN_OFF PORTB &= ~(1 << PB0)

int TouchSensor(void){
	// Set up input pin
	DDRB &= ~(1<<2) // Set pin 2 to be an input pin on B.
	PORTB |= (1<<2) // Enable the pull-up Resistor for PB2
	
	//** HARDWARE MACROS** //
	#define TOUCHED PINB & (1<<2) // will be 1 if pin is on
	
	// ** The following code will return 1 if the sensor is being touched, and 0 if it is not **//
	
	if(TOUCHED){
		return 1;
	}
	else{
		return 0;
	}
			
}

void coil_PWM(uint8_t frequency, float duty_cycle, enum direction, uint8_t ncycles){
	
	//full cycle period in ms
	uint8_t period_ms = 1000 / frequency;
	
	uint8_t on_time = (uint8_t) (duty_cycle * (float)period_ms);
	uint8_t off_time = (uint8_t) ((1 - duty_cycle) * (float)period_ms);
	
	for(int i=0; i < ncycles, i++){
		TURN_ON(direction);
		timer_wait(on_time);
		TURN_OFF;
		timer_wait(off_time)
	}

}

void setup_current_driver_pins(){
	DDRB |= (1 << PB0); // Set PB0 as output enabling both drivers
	DDRB |= (1 << PB1); // Set PB1 as output PWM (direction) pin for both drivers. high is opening, low is closing 
	
	/*How to drive this current driver still confuses me. I think, we will need 2 signals, one to disable all PWMs and the other to choose direction*/
	
}
void PWMTimer(void){
	
	DDRB |= (1<<3); // set PB3 as output
	
	// Configure PWM mode and clock pre-scaler
	TCCR1A = 0x23; // PWM?
	TCCR1B = 0x1B; // Pre-scalar
	
	// Set Top Value -- > This will need to vary based on the desired duty cycle.
	// T pwm period = resolution * (top + 1)
	// Tpwm on = resolution * (compare +1)
	
	OCRA1AH = 0; 
	OCRA1AL = 5;
	
	// Set Compare value
	OCRA1BH  = 0;
	OCRA1BL = 3; 
	
}

int main(void)
{
	setup_current_driver_pins();
	
	
	
	
    while (1) 
    {
    }
}
