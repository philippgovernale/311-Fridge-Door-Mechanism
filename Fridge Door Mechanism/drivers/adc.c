


void ADC_initialise(void){

	/* Use Pin C0 as ADC input*/
	DDRC &= ~(1<<0) // Set PC0 to be an input pin
	PORTC |= (1<<0) // Enable the pull-up Resistor for PC0

	/*Ensure lower 4 bits are disabled to select ADC0 pin*/
	ADMUX &= 0xF0;
}

uint16_t get_ADC_count (){
	// Use this function when voltage goes to 0.

	ADCSRA |= (1 << ADEN); /*enables ADC*/

	ADCSRA |= (1 << ADSC) /* start conversion*/

	while(ADCSRA & (1 << ADIF) ==0){;}; /*wait for the ADC to convert value*/

	uint16_t ADC_value = ADCL | (ADCH<<8); /*ADC value is split in two registers so we combine them together*/
	
	ADCSRA &= ~(1 << ADEN); /*disable ADC when done*/

	return ADC_value;
}
