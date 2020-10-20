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



int main(void)
{
	current_driver_pins_init();
	sensor_pins_init();
	led_pins_init();
	ADC_init();
	timer_0_PWM_init();
	timer_2_init();

  while (1)
  {
		FSM_tick();
  }
	return 0;
}
