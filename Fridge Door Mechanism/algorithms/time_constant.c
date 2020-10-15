#define <math.h>

#define ADC_MIN_CURRENT_VALUE
#define MIN_CLOSED_DOOR_TM 0.00417
#define MAX_OPEN_DOOR_TM 0.00212

float door_closed_tm;
float door_open_tm;

float measure_time_constant(enum door_state state){
  uint16_t ADC_count_start = get_ADC_count();

  if (ADC_count < ADC_MIN_CURRENT_VALUE){
    switch(1, state);
    timer_wait(5);
    ADC_count_start = get_ADC_count();
    switch(0, state);
  }

  timer_wait(4);
  uint16_t ADC_count_end = get_ADC_count();
  float time_constant = -0.004 / math.log(ADC_count_end / ADC_count_start);
  return time_constant;
}
