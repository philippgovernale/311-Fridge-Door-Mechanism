
enum state = {
  START,
  OPENING,
  OPEN,
  CLOSED,
}
void FSM_tick(){
  static enum state current_state = START;

  static uint8_t calibrated = 0;

  switch(current_state){
    case START:
      if (get_door_state() == DOOR_OPEN){
        current_state = OPEN;
      }
      else {
        current_state = CLOSED;
      }
      break;

    case OPENING:
      opening_force();

      if (get_door_state() == DOOR_OPEN){
        current_state = OPEN;
      }
      else {
        ; /*do alternative attempt to open door*/
		hp_opening_force();
      }

      break;

    case OPEN:
      sei();
	  set_door_open_interrupt();
      set_door_unattended_interrupt(); /*function should enable timer that starts counting to 20 seconds or so (via ISR) and then set the state to door closing*/
      set_led(DOOR_OPEN);

      uint8_t attempted_closing_door = 0;
	  door_unattended = 0; /*reset flag in case still high*/
	  
	  uint16_t tc = measure_time_constant();

      while (1)
      {
        if (get_door_state() == DOOR_CLOSED){
          door_state = CLOSED;
          door_unattended = 0;
          break;
        }
		
		/* door is still closer to being closed but not quite closed so we apply a higher closing force*/
		else if (tc_incr_closing(tc) && attempted_closing_door){
			hp_closing_force();
			tc = measure_time_constant();
		}
		
		else if (tc_incr_closing(tc)){
			closing_force();
			tc = measure_time_constant();
			attempted_closing_door = 1;
		}

        else if (door_unattended & !attempted_closing_door){
          closing_force();
          clear_door_unattended_interrupt();
          attempted_closing_door = 1;
        }
     }
	 clear_door_unattended_interrupt();
	 clear_door_open_interrupt(); 
     cli();
     break;

    case CLOSED:
      set_led(DOOR_CLOSED);
	  set_touch_interrupt();
	  setup_WDT(); /*I think this can go in the main*/
	  

      while (1)
      {
        if (get_door_state() == DOOR_OPEN){
          current_state = OPEN;
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
	  clear_touch_interrupt();
      break;

    default:
      current_state = START;
      break;
  }
}
