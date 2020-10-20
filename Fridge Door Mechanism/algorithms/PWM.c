#include <stdlib.h>
#include "pwm.h"
#include "../algorithms/current_measure.h"
#include "../drivers/timer.h"
#include "../drivers/io_pins.h"

#define FREQ 20

void voltage_PWM(uint8_t frequency, float duty_cycle, enum door_state direction, uint8_t ncycles){

	//full cycle period in ms
	uint8_t period_ms = 1000 / frequency;

	uint8_t on_time = (uint8_t) (duty_cycle * (float)period_ms);
	uint8_t off_time = (uint8_t) ((1 - duty_cycle) * (float)period_ms);

	for (uint8_t i=0; i < ncycles; i++){
		switches(1, direction);
		timer_wait(on_time);
		switches(0, direction);
		timer_wait(off_time);
	}
}

/*for door unattended*/
void closing_force(uint8_t i_ref){
	float duty_cycle = 0.1;
	uint8_t cycles_per_dc = 1;
	int8_t num_dc = 5;
	
	for (uint8_t i=0; i < 3; i++){
	
		for (uint8_t j=0; j < num_dc; j++){
			voltage_PWM(FREQ, duty_cycle, DOOR_CLOSED, cycles_per_dc);
			duty_cycle += 0.2;
			uint8_t i_current = measure_current_rise(DOOR_OPEN,DOOR_CLOSED);
		
			if (i_ref - i_current > 0 && (((abs(i_ref - i_current)*100) / i_ref) > 2)){
				/*door has moved and become more closed*/
			
				/*finish closing it by giving it a PWM just less than the one before*/
				voltage_PWM(FREQ, duty_cycle - 0.2, DOOR_CLOSED, cycles_per_dc);
				return;
			}
		
		}
		duty_cycle = 0.5;
		num_dc = 3;
		cycles_per_dc++;
		
	}
}

void opening_force(){
	voltage_PWM(FREQ, 0.1, DOOR_OPEN, 1);
	voltage_PWM(FREQ, 0.3, DOOR_OPEN, 1);
	voltage_PWM(FREQ, 0.5, DOOR_OPEN, 1);
	voltage_PWM(FREQ, 0.7, DOOR_OPEN, 1);
	voltage_PWM(FREQ, 0.9, DOOR_OPEN, 1);
	/*
	Opening sequence: 250 ms
    Could change this if it doesn't work, just a first guess
	*/
}

/*higher power opening force, if first opening force does not close sufficiently*/
void hp_opening_force(){
	voltage_PWM(FREQ, 0.5, DOOR_OPEN, 2);
	voltage_PWM(FREQ, 0.7, DOOR_OPEN, 2);
	voltage_PWM(FREQ, 0.9, DOOR_OPEN, 2);
	/*
	sequence: 300 ms
	*/
}

