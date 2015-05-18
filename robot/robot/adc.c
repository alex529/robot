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

volatile uint8_t h_ch0;
volatile uint8_t l_ch0;
volatile uint8_t h_ch1;
volatile uint8_t l_ch1;
volatile uint16_t results[2];
volatile bool new_data_available;
volatile bool new_data_available_to_transmit;
volatile uint8_t stage;
volatile bool conversionIsInProgress;
volatile bool enabled;

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
	stage = PINA0;
}

// starts the first conversion, the rest will be started in the ISR
void measure() {
	
	task_t system_state = {.data.command = DEBUG11, .data.timestamp=0, .data.u8[0]=stage};
		add_task(&system_state);	

	if (stage==PINA0){
		setChannel(PINA0);
		conversionIsInProgress = true;
		ADCSRA |= (1<<ADSC);		
	}
	
	if (stage==PINA1){
		setChannel(PINA1);
		conversionIsInProgress = true;
		ADCSRA |= (1<<ADSC);
	} 
	
	if (stage==STAGE_FINISH)
	{
		uint32_t value=0;
		uint32_t vstep = 488;
		
		value = l_ch0;
		value = value + (h_ch0<<8);
		results[0] = value * vstep / 100;
		
		value=0;
		
		value = l_ch1;
		value = value + (h_ch1<<8);
		results[1] = value * vstep / 100;
		
		stage = PINA0;
		new_data_available = true;
	}
	
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

void enable_adc() {
	stage=PINA0;
	enable_features.adc = true;
}

void disable_adc() {
	enable_features.adc=false;
}
 
void send_adc_value_to_pc() {	
	if (new_data_available)
	{
		new_data_available = false;
		task_t adc_value_task;
		adc_value_task.data.command = ADC1;
		adc_value_task.data.timestamp = 0;
		adc_value_task.data.u8[0] = results[0] & 0xff;
		adc_value_task.data.u8[1] = results[0]>>8; 
		adc_value_task.data.u8[2] = results[1] & 0xff;
		adc_value_task.data.u8[3] = results[1]>>8;
		add_task(&adc_value_task);	
	}
}

