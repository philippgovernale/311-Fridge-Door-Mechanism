#include <stdint.h>
#include "door_state.h"

void voltage_PWM(uint8_t frequency, float duty_cycle, enum door_state direction, uint8_t ncycles);
void opening_force();
void hp_opening_force();
