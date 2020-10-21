#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdint.h>

#include "../drivers/timer.h"
#include "../drivers/io_pins.h"
#include "../algorithms/pwm.h"
#include "../algorithms/door_state.h"
#include "../algorithms/current_measure.h"

enum state {
  START,
  OPENING,
  OPEN,
  CLOSED
};


void FSM_tick(){
  static enum state current_state = START;

  switch(current_state){
    case START:
	
		/*
		switch(1, DOOR_CLOSED);
		timer_wait(1000);
		switch(0, DOOR_CLOSED);
		*/
		
		if (get_door_state(UNKNOWN, UNKNOWN) == DOOR_OPEN){
			current_state = OPEN;
		}
		else {
			current_state = CLOSED;
		}
		break;

    case OPENING:
      opening_force();

      if (get_door_state(DOOR_OPEN, DOOR_OPEN) == DOOR_OPEN){
        current_state = OPEN;
      }
      else {
        /*do alternative attempt to open door*/
		hp_opening_force();
      }

      break;

    case OPEN:
		sei();
		set_door_unattended_and_led_interrupt(); /*function should enable timer that starts counting to 30s (via ISR) and then set the state to door closing*/

		door_unattended = 0; /*reset flag*/
	  
		uint16_t i_value = measure_current_rise(DOOR_OPEN, DOOR_OPEN);

		while (1)
		{
			if (get_door_state(DOOR_OPEN, DOOR_OPEN) == DOOR_CLOSED){
			current_state = CLOSED;
			door_unattended = 0;
			break;
			}
		
			if (door_closing(i_value)){
			 //breaking force
				switches(1, DOOR_OPEN);
				timer_wait(5); //10%
				switches(0, DOOR_OPEN);
				closing_force(); //then finish closing the door
			}
		
			else if (door_unattended ){
				closing_force();
				clear_door_unattended_interrupt();
			
			/*if the door is still closed at this point then don't try to close it anymore as its probably too far away*/
			}
		
		/*Perform 1 10% cycle, to break door if necessary*/
			/*
			set_door_open_interrupt();
			timer_wait(50);
			clear_door_open_interrupt();
			*/
		}
		clear_door_open_interrupt(); 
		cli();
		break;

    case CLOSED:
      set_LED(DOOR_CLOSED);
	  set_touch_interrupt();	  

      while (1)
      {
        if (get_door_state(DOOR_CLOSED, DOOR_CLOSED) == DOOR_OPEN){
          current_state = OPEN;
          break;
        }
		
		if (touched){
			touched = 0;
			current_state = OPENING;
			break;
		}
		
		/*sleep for 1s by going to power down mode*/
		enable_WDT_interrupt();
		SMCR |= (1 << SM1);//power down sleep mode
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
		cli();
		

      }
	  disable_WDT_interrupt();
	  clear_touch_interrupt();
      break;

    default:
      current_state = START;
      break;
  }
}
