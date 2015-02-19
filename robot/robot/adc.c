/*
 * adc.c
 *
 * Created: 2/19/2015 6:40:17 PM
 *  Author: Adam Orosz, s134615
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

static volatile int current_channel = PINA0;
static volatile bool itr8307sOnly;
static volatile bool conversionIsInProgress = 0;

void adc_measurement_init() {
	
	/** setting AD0-AD7 as input */
	DDRA&=~(1<<PINA0);
	DDRA&=~(1<<PINA1);
	DDRA&=~(1<<PINA2);
	DDRA&=~(1<<PINA3);
	DDRA&=~(1<<PINA4);
	DDRA&=~(1<<PINA5);
	DDRA&=~(1<<PINA6);
	DDRA&=~(1<<PINA7);

	/** selecting ref voltage to AVCC */
	ADMUX&=~(1<<REFS1);
	ADMUX|=(1<<REFS0);

	/** enabling ADC */
	ADCSRA|=(1<<ADEN);
	
	/** enabling interrupt service */
	ADCSRA|=(1<<ADIE);

	/** selecting clock devision, 12  */
	ADCSRA|=(1<<ADPS2);
	ADCSRA|=(1<<ADPS1);
	ADCSRA|=(1<<ADPS0);
}

void measure() {
	
	// is it set false in the ISR 
	conversionIsInProgress = true;
	
	 /** starting conversion */
	ADCSRA |= (1<<ADSC);
	// channel is changed in the ISR 
	// ISR is located in the ISR.c file
}

void measureWithItr8307sOnly() {
	
	itr8307sOnly = true;
}

void measureWithBothKindOfSensors() {
	
	itr8307sOnly = false;
}

void handleMeasurement() {
	if (!conversionIsInProgress)
	{
		measure();
	} else {
		return;
	}
}

