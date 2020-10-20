#include <stdint.h>

#ifndef DOOR_STATE_H_
#define DOOR_STATE_H_

enum door_state {DOOR_OPEN, DOOR_CLOSED, UNKNOWN};
	
enum door_state get_door_state_uncalib(enum door_state state, enum door_state intended_state);
enum door_state get_door_state(enum door_state state, enum door_state intended_state);
uint8_t door_closing(uint16_t i_ADC_v);

#endif