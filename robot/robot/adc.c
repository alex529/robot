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
#include "common.h"

volatile uint16_t result;
volatile bool new_data_available;
volatile bool new_data_available_to_transmit;
volatile bool conversionIsInProgress;
volatile bool enabled;

void adc_measurement_init() {
	
	/** setting AD7 as input */
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
	
	conversionIsInProgress = false;
}

// starts the first conversion, the rest will be started in the ISR
void measure() {
	
		setChannel(PINA7);
		conversionIsInProgress = true;
		ADCSRA |= (1<<ADSC);		
	
}


void handleMeasurement() {
	
	 if (enable_features.adc)
	{

		if (conversionIsInProgress==false)
		{
			measure();
			} else {
			return;
		}
	}
	
}

 
void send_adc_value_to_pc() {	
	if (new_data_available_to_transmit)
	{
		new_data_available_to_transmit = false;
		task_t adc_value_task;
		adc_value_task.data.command = ADC1;
		adc_value_task.data.timestamp = 0;
		adc_value_task.data.u8[0] = result & 0xff;
		adc_value_task.data.u8[1] = result>>8; 
		//adc_value_task.data.u8[2] = results[1] & 0xff;
		//adc_value_task.data.u8[3] = results[1]>>8;
		adc_value_task.data.u8[2]=0;
		adc_value_task.data.u8[3]=0;
		add_task(&adc_value_task);	
	}
}

