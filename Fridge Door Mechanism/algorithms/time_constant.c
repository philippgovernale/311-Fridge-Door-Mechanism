#define <math.h>

#define ADC_MIN_CURRENT_VALUE
#define MIN_CLOSED_DOOR_TM 0.00417
#define MAX_OPEN_DOOR_TM 0.00212


/*need time constants for particular duty cycle and current direction!*/

float measure_time_constant(enum door_state state, enum door_state intended_state){

	
	/*only start applying voltage for measuring tc when the ADC value is at 99% of steady state from max current at 20degrees (ie current is very low (2.4 A)*/
	/*should add a buffer for noise. Also amplifying */
	while(get_ADC_count() > 5){
		timer_wait(1);
	}
	
	switch(1, intended_state);
	timer_wait(5); // apply 10% duty cycle
	switch(0, intended_state);
	
	uint16_t i_initial = get_ADC_count();
	timer_wait(4); // wait 4 ms
	uint16_t i_final = get_ADC_count();
	
	
	float time_constant = -0.004 / math.log(i_final / i_initial);
	return time_constant;
}
