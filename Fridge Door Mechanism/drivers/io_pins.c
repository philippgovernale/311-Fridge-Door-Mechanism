void current_driver_pins_initialise(){
	DDRB |= (1 << 0); /*Output pins for current driver*/
	DDRB |= (1 << 1);
}

void sensor_pins_initialise(){
	// Set up input pin
	DDRB &= ~(1<<2) // Set pin 2 to be an input pin on B.
	PORTB |= (1<<2) // Enable the pull-up Resistor for PB2
}

void led_pins_initialise(){
  DDRB |= (1<<3); // green led
  DDRB |= (1<<4); // red led
}
