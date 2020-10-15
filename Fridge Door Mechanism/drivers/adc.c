


void ADC_initialise(void){

	/* Use Pin C0 as ADC input*/
	DDRC &= ~(1<<0) // Set PC0 to be an input pin
	PORTC |= (1<<0) // Enable the pull-up Resistor for PC0

	/* Use 1.1V ref*/
	ADMUX |= (1 << REFS0);
	ADMUX |= (1 << REFS1);

	/*Ensure lower 4 bits are disabled to select ADC0 pin*/
	ADMUX &= 0xF0;
}

uint16_t get_ADC_count (){
	// Use this function when voltage goes to 0.

	ADCSRA |= (1 << ADEN); /*enables ADC*/

	timer_wait(CURRENT_MEAS_DELAY_MS);

	ADCSRA |= (1 << ADSC) /* start conversion*/

	while(ADCSRA & (1 << ADIF) ==0){;}; /*wait for the ADC to convert value*/

	uint16_t ADC_value = ADCL | (ADCH<<8); /*ADC value is split in two registers so we combine them together*/

	return ADC_value;
}
