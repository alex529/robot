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
volatile bool conversionIsInProgress;
volatile bool enabled;
volatile adc_values_t adc_values;
volatile adc_values_t adc_values_empty;

void adc_measurement_init() {
	
	/** setting AD0-AD7 as input */
	DDRA&=~(1<<PINA0);
	DDRA&=~(1<<PINA1);

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
	
	conversionIsInProgress = false;
	current_channel = PINA0;
}

// starts the first conversion, the rest will be started in the ISR
void measure() {
	
	//reset adc_values
	adc_values = adc_values_empty;
	
	setChannel(PINA0);
	
	// it is set false in the ISR 
	conversionIsInProgress = true;
	
	 /** starting conversion */
	ADCSRA |= (1<<ADSC);
	// channel is changed in the ISR 
	// ISR is located in the ISR.c file
}

void enableADC() {
	enabled = true;
}

void disableADC() {
	enabled = false;
}

void handleMeasurement() {

	 if (enabled)
	{

		if (conversionIsInProgress==false)
		{
			measure();
			} else {
			return;
		}
	}
	
}

void send_adc_value() {
	
	task_t string_task = {.data.command = ADC1, .data.timestamp=0, .data.value = adc_values.results[0};
	task_t string_task = {.data.command = ADC2, .data.timestamp=0, .data.value = adc_values.results[1};
	add_task(&string_task);	
}

