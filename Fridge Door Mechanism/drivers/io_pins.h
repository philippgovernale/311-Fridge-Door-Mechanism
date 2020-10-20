#include <stdint.h>
#include "../algorithms/door_state.h"

extern volatile uint8_t touched;

void current_driver_pins_init();
void sensor_pins_init();
void led_pins_init();
void set_touch_interrupt();
void clear_touch_interrupt();
void set_LED(uint8_t state);
void switches(uint8_t on,enum door_state state);
