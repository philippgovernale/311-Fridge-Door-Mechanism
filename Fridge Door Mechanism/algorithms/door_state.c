#include <math.h>

#define MIN_CLOSED_DOOR_TC 0.00417
#define MAX_OPEN_DOOR_TC 0.00212

float door_closed_tc = 0;
float door_open_tc = 0;

enum door_state get_door_state_uncalib(enum door_state state, enum door_state intended_state){
  float tc = measure_time_constant(state);

  if (tm > MIN_CLOSED_DOOR){
    door_closed_tc = tc;
    return CLOSED_DOOR;
  }
  else if (tm < MAX_OPEN_DOOR){
    door_open_tc = tc;
    return OPEN_DOOR;
  }
  else {
    /* if the time constant is not in between these two previosuly measured values, then we first see which state is closest*/

    if (tc > MAX_OPEN_DOOR_TC + (MIN_CLOSED_DOOR_TC - MAX_OPEN_DOOR_TC)){
      /*Most likely to be a closed door state as the time constant is closer to that value*/
      /*Apply a very short pulse to see if the time constant changes. If it does, it suggest the door may have been nearly closed*/

      if (intended_state == DOOR_OPEN){
        return DOOR_CLOSED;
      }

      switch(1, DOOR_CLOSED);
      timer_wait(40);
      switch(0, DOOR_CLOSED);
      float tc_comp = measure_time_constant(state);

      /* if the change in measurement is less than 2%, then assume the door has been closed all the time*/
      if (abs(tc_comp - tc)/tc < 0.02){
        door_closed_tc = tc;
        return DOOR_CLOSED;
      }

      /*Otherwise assume that the force changed the door position, which means the door may still be open*/
      return get_door_state(DOOR_OPEN, DOOR_CLOSED);

    }
    else {
      /*Most likely to be an open door state, as the time constant is closer to that value*/

      /* don't try to apply a closing force as we want to have the door open*/
      if (intended_state == DOOR_CLOSED){
        return DOOR_OPEN;
      }

      switch(1, DOOR_OPEN);
      timer_wait(40);
      switch(0, DOOR_OPEN);
      float tc_comp = measure_time_constant(enum door_state state);

      if (abs(tc_comp - tc)/tc < 0.02){
        door_open_tc = tc;
        return DOOR_OPEN;
      }

      /*Assume the force changed the position which means the door may still be somewhat closed*/
      return get_door_state(DOOR_OPEN, DOOR_CLOSED);
    }
  }

  }
}

enum door_state get_door_state(enum door_state state, enum door_state intended_state){
  /*if*/
  if (!door_closed_tc || !door_open_tc){
    get_door_state_uncalib(state, intended_state);
  }

  float tc = measure_time_constant();

  float door_open = abs(tc-door_open_tc)/tc;
  float door_closed = abs(tc-door_closed_tc)/tc;


  /*door closed should always be at least 40% higher than door open. Maximum divergences in door door closed are ~31%*/
  if (door_closed < 0.31 && door_open > 0.40){
    return DOOR_CLOSED;
  }
  else if (door_open < 0.15 & door_closed > 0.40){
    return DOOR_OPEN;
  }
  else{
    get_door_state_uncalib(state, intended_state);
  }
}

uint8_t tc_incr_closing(uint16_t tc){
	uint16_t tc_comp = measure_time_constant();
	
	if (tc > tc_comp && (abs(tc - tc_comp)/tc) > 0.1){
		return 1; /*door is likely to be more closed*/
	}
	else {
		return 0; 
	}
}
