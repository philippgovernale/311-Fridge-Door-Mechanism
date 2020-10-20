#include "current_measure.h"
#include "../drivers/adc.h"
#include "../drivers/timer.h"

/*measure current after 4 ms after applying voltage*/

uint16_t measure_current_rise(enum door_state state, enum door_state intended_state){
	
	/*only start applying voltage for measuring tc when the ADC value is at 99% of steady state from max current at 20degrees (ie current is very low (2.4 A)*/
	/*should add a buffer for noise. Also amplifying */
	while(get_ADC_count() > 5){
		timer_wait(1);
	}
	
	switch(1, intended_state);
	timer_wait(4); // apply 4 ms pulse
	uint16_t ADC_4_ms = get_ADC_count();
	switch(0, intended_state);
	
	return ADC_4_ms;
}
