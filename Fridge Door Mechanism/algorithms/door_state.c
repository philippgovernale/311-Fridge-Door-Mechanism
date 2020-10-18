#include <math.h>

/*!!! are these for closing or opening forces? These are wrong. We need:
 - min opening force 10% duty cycle closed door tc
 - max opening force 10% duty cycle open door tc
 - min closing force 10% duty cycle closed door tc
 - max closing force 10% duty cycle open door tc*/

#define MIN_OPENING_F_10_DC_CLOSED_TC
#define MAX_OPENING_F_10_DC_OPEN_TC
#define MIN_CLOSING_F_10_DC_CLOSED_TC
#define MAX_CLOSING_F_10_DC_OPEN_TC

float closing_f_door_closed_tc = 0;
float closing_f_door_open_tc = 0;

float opening_f_door_closed_tc = 0;
float opening_f_door_open_tc = 0;

enum door_state get_door_state_uncalib(enum door_state state, enum door_state intended_state){
	/*Always measure with door closed when calibrating as when the fridge is first started, this may be safely assumed*/
	float tc;
	if (state == UNKNOWN){
		tc = measure_time_constant(DOOR_CLOSED, DOOR_CLOSED);
	}
	
	else {
		tc = measure_time_constant(state , intended_state);
	}
	

	if (intended_state == DOOR_CLOSED && tc > MIN_CLOSING_F_10_DC_CLOSED_TC){
		closing_f_door_closed_tc = tc;
		return DOOR_CLOSED;
	}
	else if (intended_state == DOOR_CLOSED && tc < MAX_CLOSING_F_10_DC_OPEN_TC){
		closing_f_door_open_tc = tc;
		return DOOR_OPEN;
	}
	else if (intended_state == DOOR_OPEN && tc > MIN_OPENING_F_10_DC_CLOSED_TC){
		opening_f_door_closed_tc = tc;
		return DOOR_CLOSED
	}
	else if (intended_state == DOOR_OPEN && tc < MAX_CLOSING_F_10_DC_OPEN_TC){
		opening_f_door_open_tc = tc;
		return DOOR_OPEN;
	}
	
	/* Potentially readd here code handling tc outside of calculated time constants*/
}

	  /*door closed should always be at least 40% higher than door open. Maximum divergences in door door closed are ~31%
	  !!!! These numbers, I believe are not for 10% */
  enum door_state get_door_state(enum door_state state, enum door_state intended_state){
	  /*if*/
	  if (!closing_f_door_closed_tc || closing_f_door_open_tc || opening_f_door_closed_tc || opening_f_door_open_tc){
		  get_door_state_uncalib(state, intended_state);
	  }

	  float tc = measure_time_constant(state);

	  float door_open, door_closed;
	  
	  if (intended_state == DOOR_CLOSED){
		  door_open = abs(tc-closing_f_door_open_tc)/tc;
		  door_closed = abs(tc-closing_f_door_closed_tc)/tc;
		  
		  if (door_closed < 0.31 && door_open > 0.40){
			  closing_f_door_closed_tc = tc;
			  return DOOR_CLOSED;
		  }
		  else if (door_open < 0.15 & door_closed > 0.40){
			  closing_f_door_open_tc = tc;
			  return DOOR_OPEN;
		  }
		  else{
			  get_door_state_uncalib(state, intended_state);
		  }	  
	  }
	  else if (intended_state == DOOR_OPEN){
		  door_open = abs(tc-opening_f_door_open_tc)/tc;
		  door_closed = abs(tc-closing_f_door_closed_tc)/tc;
		  
		  if (door_closed < 0.31 && door_open > 0.40){
			  opening_f_door_closed_tc = tc;
			  return DOOR_CLOSED;
		  }
		  else if (door_open < 0.15 & door_closed > 0.40){
			  opening_f_door_open_tc = tc;
			  return DOOR_OPEN;
		  }
		  else{
			  get_door_state_uncalib(state, intended_state);
		  }	  
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
