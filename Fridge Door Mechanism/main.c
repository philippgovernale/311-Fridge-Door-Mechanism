/*
 * Fridge Door Mechanism.c
 *
 * Created: 10/13/2020 7:51:51 PM
 * Author : ashap
 */ 

#include <avr/io.h>
#include <stdio.h> // do we need this? I added it. We need it if we want to print out anything

enum current_dir = {OPEN, CLOSED};
	
#define TURN_ON(D) PORTB |= (1 << PB0); !D ? PORTB |= (1 << PB1) : PORTB &= ~(1 << PB1) // Change me
#define TURN_OFF PORTB &= ~(1 << PB0)
#define CURRENT_MEAS_DELAY_MS 4
#define FREQ 20

/*Threshold voltage for 4 ms voltage build up. This voltage is between expected current values for open/closed state*/
#define OPEN_CLOSED_THRES 386 /*if count is above, then door is open, else closed*/


void LED(enum state){
	//** If the state variable is 1 that means door is open, turn on LED **//
	
	DDRB |= (1<<3); // set as output
	
	if (state == 1){
		// turn on the LEd
		PORTB |= (1<<3); // set PB3 high
	}
	else{
		PORTB &= ~(1<<3); // set PB3 low
		
	}
	
	
}

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

void voltage_PWM(uint8_t frequency, float duty_cycle, enum direction, uint8_t ncycles){
	
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
	DDRB |= (1 << 0); // Set PB0 as output enabling both drivers
	DDRB |= (1 << 1); // Set PB1 as output PWM (direction) pin for both drivers. high is opening, low is closing 
	
	/*How to drive this current driver still confuses me. I think, we will need 2 signals, one to disable all PWMs and the other to choose direction*/
	
}

void ADC_initialise(void){
	
	/* Use Pin C0 as ADC input*/
	DDRC &= ~(1<<0) // Set PC0 to be an input pin
	PORTC |= (1<<0) // Enable the pull-up Resistor for PC0	

	/* Use 1.1V ref*/
	ADMUX |= (1 << REFS0);
	ADMUX |= (1 << REFS1);
	
	/*Ensure lower 4 bits are disabled to select ADC0 pin*/
	ADMUX &= 0xF0;
}

enum get_door_state(){
	uint16_t ADC_count = measure_current();
	
	if (ADC_count > OPEN_CLOSED_THRES){
		return OPEN;
	}
	else {
		return CLOSED;
	}
};

uint16_t measure_current (void){
	// Use this function when voltage goes to 0.
	
	ADCSRA |= (1 << ADEN); /*enables ADC*/
	
	timer_wait(CURRENT_MEAS_DELAY_MS);
	
	ADCSRA |= (1 << ADSC) /* start conversion*/
	
	while(ADCSRA & (1 << ADIF) ==0){;}; /*wait for the ADC to convert value*/
	
	uint16_t ADC_value = ADCL | (ADCH<<8); /*ADC value is split in two registers so we combine them together*/
	
	return ADC_value;
}

void door_opening_sequence(){	
	voltage_PWM(FREQ, 0.1, OPEN, 1);
	voltage_PWM(FREQ, 0.3, OPEN, 1);
	voltage_PWM(FREQ, 0.5, OPEN, 1);
	voltage_PWM(FREQ, 0.7, OPEN, 1);
	voltage_PWM(FREQ, 0.9, OPEN, 1);
	
	/* 
	Opening sequence: 250 ms
    Could change this if it doesn't work, just a first guess
	*/
}


int main(void)
{
	setup_current_driver_pins();
	
	
	
	
    while (1) 
    {
    }
}