/*
 * Fridge Door Mechanism.c
 *
 * Created: 10/13/2020 7:51:51 PM
 * Author : ashap
 */ 

#include <avr/io.h>
#include <stdio.h> // do we need this? I added it 

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
	
	
	
	
	
    while (1) 
    {
    }
}
