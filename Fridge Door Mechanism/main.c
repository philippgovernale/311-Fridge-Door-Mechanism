/*
 * Fridge Door Mechanism.c
 *
 * Created: 10/13/2020 7:51:51 PM
 * Author : ashap
 */

#include <avr/io.h>
#include "drivers/timer.h"
#include "drivers/adc.h"
#include "drivers/io_pins.h"
#include "fsm/fsm.h"

#define CURRENT_MEAS_DELAY_MS 4

int main(void)
{
	current_driver_pins_init();
	sensor_pins_init();
	led_pins_init();
	ADC_init();
	timer_0_PWM_init();
	timer_2_init();
	WDT_init();

  while (1)
  {
	  FSM_tick();
  }
	return 0;
}
