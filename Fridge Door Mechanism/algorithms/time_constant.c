#define <math.h>

#define ADC_MIN_CURRENT_VALUE
#define MIN_CLOSED_DOOR_TM 0.00417
#define MAX_OPEN_DOOR_TM 0.00212


/*measure current after 4 ms after applying 50% duty cycle*/

uint16_t measure_current_rise(enum door_state state, enum door_state intended_state){

	
	/*only start applying voltage for measuring tc when the ADC value is at 99% of steady state from max current at 20degrees (ie current is very low (2.4 A)*/
	/*should add a buffer for noise. Also amplifying */
	while(get_ADC_count() > 5){
		timer_wait(1);
	}
	
	switch(1, intended_state);
	timer_wait(4); // apply 10% duty cycle
	switch(0, intended_state);
	
	uint16_t ADC_4_ms = get_ADC_count();

	return ADC_4_ms;
}
