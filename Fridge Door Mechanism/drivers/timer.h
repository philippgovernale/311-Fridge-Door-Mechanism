#include <stdint.h>

#ifndef TIMER_H_
#define TIMER_H_

#include "../algorithms/door_state.h"

extern volatile uint8_t timer_count_led;
extern volatile uint8_t timer_count_open_PWM;
extern volatile uint16_t timer_count_unattended;

extern volatile uint8_t PWM_state_high; /*used to indicate whether PWM is low/high*/

extern volatile uint8_t door_unattended;

void timer_0_PWM_init();
void timer_2_init();
void set_door_unattended_and_led_interrupt();
void set_door_open_interrupt();
void clear_door_open_interrupt();
void clear_door_unattended_interrupt();
void WDT_init();
void enable_WDT_interrupt();
void disable_WDT_interrupt();
void timer_wait(uint32_t milliseconds);

#endif