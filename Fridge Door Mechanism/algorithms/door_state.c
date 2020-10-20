#include <math.h>


/* these are ADC counts*/
/* closing current minimum value (closed): 1.174 A
   closing current maximum value (open): 2.14 A
*/
#define MIN_OPENING_50_DC_4_MS
#define MAX_OPENING_50_DC_4_MS
#define MIN_CLOSING_50_DC_4_MS
#define MAX_CLOSING_50_DC_4_MS

#define AVERAGE_CLOSING_50_DC_4_MS (MAX_CLOSING_50_DC_4_MS - MIN_CLOSING_50_DC_4_MS)/2 + MIN_CLOSING_50_DC_4_MS
#define AVERAGE_OPENING_50_DC_4_MS (MAX_OPENING_50_DC_4_MS - MIN_OPENING_50_DC_4_MS)/2 + MIN_OPENING_50_DC_4_MS

float closing_door_closed_current = 0;
float closing_door_open_current = 0;

float opening_door_closed_current = 0;
float opening_door_open_current = 0;

enum door_state get_door_state_uncalib(enum door_state state, enum door_state intended_state){
	/*Always measure with door closed when calibrating as when the fridge is first started, this may be safely assumed*/
	
	uint16_t ADC_count_4_ms;
	if (state == UNKNOWN){
		ADC_count_4_ms = measure_current_rise(DOOR_CLOSED, DOOR_CLOSED);
	}
	
	else {
		ADC_count_4_ms = measure_current_rise(state , intended_state);
	}
	
	/*if below average, most likely closing*/
	if (intended_state == DOOR_CLOSED && ADC_count_4_ms < AVERAGE_OPENING_50_DC_4_MS){
		/*15% variation allowed*/
		if ((((ADC_count_4_ms - MIN_CLOSING_50_DC_4_MS)*100) / ADC_count_4_ms) < 15){
			closing_door_closed_current = ADC_count_4_ms;
			return DOOR_CLOSED:
		}
		
		/*if less than minimum, definitely open*/
		else if ((ADC_count_4_ms - MIN_CLOSING_50_DC_4_MS) < 0){
			closing_door_closed_current = ADC_count_4_ms;
			return DOOR_CLOSED;
		}
		
		else {
			/*Door is likely to be slightly open*/
			switch(1, DOOR_CLOSED);
			timer_wait(15);
			switch(0, DOOR_CLOSED);
			
			uint16_t repeat_ADC_count = measure_current_rise(DOOR_CLOSED, DOOR_CLOSED);
			
			if (((repeat_ADC_count - ADC_count_4_ms) < 0) && (((repeat_ADC_count - ADC_count_4_ms)*100)/repeat_ADC_count > 2)) {
				get_door_state_uncalib(state, intended_state);
			} 
			else{
				closing_door_closed_current = ADC_count_4_ms;
				return DOOR_CLOSED;
			}
		}
	}
	else if (intended_state == DOOR_CLOSED && ADC_count_4_ms > AVERAGE_CLOSING_50_DC_4_MS){
		/*15% variation allowed*/
		if ((((ADC_count_4_ms - MAX_CLOSING_50_DC_4_MS)*100)/ADC_count_4_ms) < 15){
			closing_door_open_current = ADC_count_4_ms;
			return DOOR_OPEN;
		}
		else if ((ADC_count_4_ms - MAX_CLOSING_50_DC_4_MS) > 0){
			closing_door_open_current = ADC_count_4_ms;
			return DOOR_OPEN;
		}
		else {
			switch(1, DOOR_CLOSED);
			timer_wait(15);
			switch(0, DOOR_CLOSED);
			
			uint16_t repeat_ADC_count = measure_current_rise(DOOR_CLOSED, DOOR_CLOSED);
			
			if ((repeat_ADC_count - ADC_count_4_ms) > 0 && ((((repeat_ADC_count) - ADC_count_4_ms)*100)/repeat_ADC_count) / repeat_ADC_count > 2){
				get_door_state_uncalib(state, intended_state);
			}
			else {
				return DOOR_OPEN;
			}
		}
	}
	else if (intended_state == DOOR_OPEN && ADC_count_4_ms > AVERAGE_OPENING_50_DC_4_MS){
		if ((((ADC_count_4_ms - MAX_OPENING_50_DC_4_MS)*100)/ADC_count_4_ms) < 15){
			opening_door_open_current = ADC_count_4_ms;
			return DOOR_OPEN;
		}
		else if ((ADC_count_4_ms - MAX_OPENING_50_DC_4_MS) > 0){
			opening_door_open_current = ADC_count_4_ms;
			return DOOR_OPEN;
		}
		else {
			/*Door is likely to be slightly closed / slightly open*/
			switch(1, DOOR_OPEN);
			timer_wait(15);
			switch(0, DOOR_OPEN);
			
			uint16_t repeat_ADC_count = measure_current_rise(DOOR_OPEN, DOOR_OPEN);
			
			if ((repeat_ADC_count - ADC_count_4_ms) > 0 && ((((repeat_ADC_count) - ADC_count_4_ms)*100)/repeat_ADC_count) / repeat_ADC_count > 2){
				return DOOR_OPEN;
			}
			else {
				return DOOR_CLOSED;
			}		
		}
	}
	else if (intended_state == DOOR_OPEN && ADC_count_4_ms < AVERAGE_OPENING_50_DC_4_MS){
		if (((ADC_count_4_ms - MIN_OPENING_50_DC_4_MS)*100 / ADC_count_4_ms) < 15){
			opening_door_closed_current = ADC_count_4_ms;
			return DOOR_CLOSED;
		}
		
		else if ((ADC_count_4_ms - MIN_OPENING_50_DC_4_MS) < 0){
			opening_door_closed_current = ADC_count_4_ms;
			return DOOR_CLOSED;
		}
		else {
			switch(1, DOOR_OPEN);
			timer_wait(15);
			switch(0, DOOR_OPEN);
			
			uint16_t repeat_ADC_count = measure_current_rise(DOOR_OPEN, DOOR_OPEN);
			
			if ((repeat_ADC_count - ADC_count_4_ms) > 0 && ((((repeat_ADC_count) - ADC_count_4_ms)*100)/repeat_ADC_count) / repeat_ADC_count > 2){
				get_door_state_uncalib(state, intended_state);
			}
			else {
				return DOOR_CLOSED;
			}			
		}
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

  uint8_t door_closing(uint16_t i_ADC_v){
	  uint16_t i_ADC_comp = measure_current_rise();
	  
	  /*if the decrease in current is by more than 5%, then door is likely to be closing*/
	  if (i_ADC_comp < i_ADC_v && (abs(i_ADC_comp - i_ADC_v)*100/i_ADC_comp) > 5){
		  return 1; /*door is likely to be more closed*/
	  }
	  else {
		  return 0;
	  }
  }
