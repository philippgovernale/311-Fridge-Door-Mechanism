
enum state = {
  START,
  OPENING,
  OPEN,
  CLOSING,
  CLOSED,
}
void FSM_tick(){
  static enum state current_state = START

  static int calibrate = 1;

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
      set_flashing_led(); /*Function should enable timer that triggers interrupt changing the led status*/
      opening_force();

      if (get_door_state() == DOOR_OPEN){
        current_state = OPEN;
      }
      else {
        ; /*do alternative attempt to open door*/
      }
      break;

    case OPEN:
      start_open_door_timer(); /*function should enable timer that starts counting to 20 seconds or so (via ISR) and then set the state to door closing*/
      set_led_on();

      /*Poll the voltage across coil. If above certain threshold should switch state to door closing
        should also check door state*. Intermittently check the door state to make sure door is not closed/
        */
      while(1){
        if (coil_voltage > MOVING_DOOR_THRES){
          door_state = CLOSING;
          break;
        }

        if (get_door_state() == DOOR_CLOSED){
          door_state = CLOSED;
          break;
        }
      }

      /*Polling voltage: if we shut off the top two transistors and turn on only one bottom transistor, then if voltage is across the coil it should flow through transistor*/
      break;

    case CLOSING:
      set_flashing_led();
      breaking_force();
      closing_force();
      break;

    case CLOSED:
      break;
    default:
      current_state = START;
      break;
  }
}
