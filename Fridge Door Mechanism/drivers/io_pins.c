
volatile uint8_t touched = 0;

void current_driver_pins_init(){
	DDRB |= (1 << 0); /*Output pins for current driver. PB0 is opening direction*/
	DDRB |= (1 << 1);
}

void sensor_pins_init(){
	// Set up input pin
	DDRD &= ~(1<<2) // Set pin 2 to be an input pin on D.
	PORTD |= (1<<2) // Enable the pull-up Resistor for PD2
}

void led_pins_init(){
	DDRB |= (1<<3); // led PIN
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

void set_LEDs(uint8_t state){
	//** If the state variable is 1 that means door is open, turn on LED **//

	if (state){
		// turn on the LEd
		PORTB |= (1<<PB3); // set led
	}
	else{
		PORTB &= ~(1 << PB3); //clear clear
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
	touched = 1;
}
