/*
 * Fridge Door Mechanism.c
 *
 * Created: 10/13/2020 7:51:51 PM
 * Author : ashap
 */

#include <avr/io.h>
#include <stdio.h> // do we need this? I added it. We need it if we want to print out anything

enum door_status = {DOOR_OPEN, DOOR_CLOSED, UNKNOWN};

#define CURRENT_MEAS_DELAY_MS 4
#define FREQ 20

/* OBSOLETE */
/*Threshold voltage for 4 ms voltage build up. This voltage is between expected current values for open/closed state*/
#define OPEN_CLOSED_THRES 386 /*if count is above, then door is open, else closed*/


void voltage_PWM(uint8_t frequency, float duty_cycle, enum door_status direction, uint8_t ncycles){

	//full cycle period in ms
	uint8_t period_ms = 1000 / frequency;

	uint8_t on_time = (uint8_t) (duty_cycle * (float)period_ms);
	uint8_t off_time = (uint8_t) ((1 - duty_cycle) * (float)period_ms);

	for (uint8_t i=0; i < ncycles, i++){
		switches(1, direction);
		timer_wait(on_time);
		switches(0, direction);
		timer_wait(off_time)
	}
}



void opening_force(){
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

/*higher power opening force, if first opening force does not close sufficiently*/
void hp_opening_force(){
	voltage_PWM(FREQ, 0.5, OPEN, 2);
	voltage_PWM(FREQ, 0.7, OPEN, 2);
	voltage_PWM(FREQ, 0.9, OPEN, 2);
	/*
	sequence: 300 ms
	*/
}

int main(void)
{
	current_driver_pins_initialise();
	sensor_pins_initialise();
	led_pins_initialise();
	ADC_initialise();
	timer_0_PWM_initialise();
	timer_2_initialise();

  while (1)
  {
		FSM_tick();
  }
	return 0;
}
