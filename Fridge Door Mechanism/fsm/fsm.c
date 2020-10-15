
enum state = {
  START,
  OPENING,
  OPEN,
  CLOSING,
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
      }

      break;

    case OPEN:
      sei();
      set_door_unattended_interrupt(); /*function should enable timer that starts counting to 20 seconds or so (via ISR) and then set the state to door closing*/
      set_led(DOOR_OPEN);

      uint8_t attempted_closing_door = 0;

      /*Poll the voltage across coil. If above certain threshold should switch state to door closing
        should also check door state*. Intermittently check the door state to make sure door is not closed/
        */
      while (1)
      {
        if (coil_voltage > MOVING_DOOR_THRES){
          door_state = CLOSING;
          break;
        }

        if (get_door_state() == DOOR_CLOSED){
          door_state = CLOSED;
          door_unattended = 0;
          break;
        }

        if (door_unattended & !attempted_closing_door){
          closing_force();
          clear_door_unattended_interrupt();
          attempted_closing_door = 1;
        }
      }

      cli();
      /*Polling voltage: if we shut off the top two transistors and turn on only one bottom transistor, then if voltage is across the coil it should flow through transistor*/
      break;

    case CLOSING:
      sei();
      set_flashing_led_interrupt();

      breaking_force();
      closing_force();

      if (get_door_state() == DOOR_CLOSED){
        current_state = CLOSED;
      }else{
        current_state = OPEN;
      }

      cli();
      break;

    case CLOSED:
      set_led(DOOR_CLOSED);

      while (1)
      {
        if (get_door_state() == DOOR_OPEN){
          current_state = OPEN;
          break;
        }

        /*for now any touch will activate opening, actually checking that it is touched for a period of time might prove more challenging than anticipated*/
        if (TOUCHED){
          current_state = OPENING;
          break;
        }

      }
      break;

    default:
      current_state = START;
      break;
  }
}
