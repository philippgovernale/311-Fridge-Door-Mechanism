void current_driver_pins_initialise(){
	DDRB |= (1 << 0); /*Output pins for current driver. PB0 is opening direction*/
	DDRB |= (1 << 1);
}

void sensor_pins_initialise(){
	// Set up input pin
	DDRD &= ~(1<<2) // Set pin 2 to be an input pin on D.
	PORTD |= (1<<2) // Enable the pull-up Resistor for PD2
}

void led_pins_initialise(){
	DDRB |= (1<<3); // green led
	DDRB |= (1<<4); // red led
}

void set_touch_interrupt(){
	//enable interrupt for touch sensor
	EICRA |= (1 << ISC00);
	EICRA |= (1 << ISC01);
	
	EIMSK |= (1 << INT0);
}

void clear_touch_interrupt(){
	EIMSK &= (1 << INT0);
}

void set_LEDs(enum door_status state){
	//** If the state variable is 1 that means door is open, turn on LED **//

	if (state == DOOR_OPEN){
		// turn on the LEd
		PORTB |= (1<<PB3); // set green led
		PORTB &= ~(1<<PB4); //clear red led
	}
	else{
		PORTB |= (1 << PB4); // set red low
		PORTB &= ~(1 << PB3); //clear green led
	}
}

void switches(uint8_t on,enum door_state door_state){
  if (!on){
    PORTB &= ~(1 << PB1);
    PORTB &= ~(1 << PB0);
  }
  else {
    /* if current state is door open, then apply opening current, as it will not change the state*/
    if (door_state == DOOR_OPEN){
      PORTB |= (1 << PB0);
    }
    else {
      PORTB |= (1 << PB1);
    }
  }
}

ISR(INT0_vect){
	
}
