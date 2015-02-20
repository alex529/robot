/*
 * adc.c
 *
 * Created: 2/19/2015 6:40:17 PM
 *  Author: Adam Orosz, s134615
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdint.h>
#include "adc.h"
#include "task.h"

volatile int8_t current_channel;
volatile bool itr8307sOnly;
volatile bool conversionIsInProgress;
volatile bool enabled;
volatile adc_values_t adc_values;
volatile adc_values_t adc_values_empty;

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
	
	conversionIsInProgress = 0;
	current_channel = PINA0;
}

// starts the first conversion, the rest will be started in the ISR
void measure() {
	
	//reset adc_values
	adc_values = adc_values_empty;
	
	ADMUX&=~(1<<MUX4);
	ADMUX&=~(1<<MUX3);
	ADMUX&=~(1<<MUX2);
	ADMUX&=~(1<<MUX1);
	ADMUX&=~(1<<MUX0);
	
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

void enableADC() {
	enabled = true;
}

void disableADC() {
	enabled = false;
}

void handleMeasurement() {
	
	task_t adc_task;
	adc_task.data.command = ADC1;
	adc_task.data.timestamp=0;
	adc_task.data.value = 3;

	add_task(&adc_task);
	
	/**
	if (enabled)
	{
		if (!conversionIsInProgress)
		{
			measure();
			} else {
			return;
		}
	}*/
}

