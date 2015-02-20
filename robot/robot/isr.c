/**
* isr.c
*
* The isr.c file is in charge of servicing the ISR routines.
*
*  Created: 2/17/2014 12:57:18 PM
*  Author: Alexandru Savin
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include "usart.h"
#include "isr.h"
#include "timer.h"
#include "common.h"
#include "adc.h"

volatile status_t status;
volatile uint32_t system_tic;

/**
* \brief Adds the received data to the rx_buffer when the frame is received a flag is raised.
*			When the frame is ready the value bites are swapped.
*
* \author Alexandru
*
* \param Vector for the interrupt.
*
* \return
*/
ISR(USART_RXC_vect)
{
	static uint8_t rx_index = 0;
	usart_rx_task.buffer[rx_index]=get_usart();
	if(++rx_index>get_array_len(usart_rx_task.buffer)-1)
	{
		rx_index = 0;
		status.system.task_received=true;
		//swap_uint8_t(rx_buffer[1],rx_buffer[4]);//bytes need to be swapped due to different arrangement in the server
		//swap_uint8_t(rx_buffer[2],rx_buffer[3]);
	}
}




void p_s(char* str){
	static char *s;
	static int i=0;
	if(i++==0)s=str;
	set_usart(*s);
	if(!*++s){
		i=0;
		set_usart('\n');
	}
}

task_t d = {.data.command=STRING, .data.str="Hello World!"};


/**
* \brief Copies the tx_buffer byte by byte to the U SART module buffer.
*
* \author Alexandru
*
* \param Vector for the  interrupt.
*
* \return
*/
ISR(USART_UDRE_vect)
{
	static uint8_t tx_index = 0;
	static char *string = "Hello World!";
	if (usart_tx_task.data.command==STRING)
	{
		
		if (tx_index==0)
		{
			string = usart_tx_task.data.str;
		}
		if (tx_index++ < 5)
		{
			//set_usart(usart_tx_task.buffer[tx_index]);
		}
		else
		{
			set_usart(*string);
			if(!*++string){
				tx_index=0;
				disable_uart_transmision();
				status.system.sending_task = false;
				toggle_led();
			}
			
		}
	}
	else
	{
		set_usart(usart_tx_task.buffer[tx_index]);
		if (++tx_index>get_array_len(usart_tx_task.buffer)-1)
		{
			tx_index = 0;
			toggle_led();
			disable_uart_transmision();
			status.system.sending_task = false;
		}
	}
}

/**
* \brief Increments the global millisecond variable that is needed for the timer.c module and
*			facilitates a system tick(100ms) used for scheduling purposes in the main loop.
*
* \author Alexandru
*
* \param Vector for the  interrupt.
*
* \return
*/
ISR(TIMER1_COMPA_vect)
{
	if (++milliseconds%100 == 0) //small error every ~50 days max number being 4294967295, so every 50 days the systems will have a 5 milliseconds error if the system is rebooted between this periods the system will act normally
	{
		system_tic = true;
	}
}

ISR(ADC_vect) {
	
	uint32_t value=0;
	uint32_t vstep = 488;

	//the measured value is 2+8 bits long. The following 2 lines creates a 10bit value from the 2+8 bit values
	value = ADCL;
	value = value + (ADCH<<8);
	
	value = value * 488;
	
	task_t string_task;
	string_task.data.command = ADC1;
	string_task.data.timestamp=0;
	string_task.data.value = value;
	add_task(&string_task);
	
	/* task_t string_task2;
	string_task2.data.command = ADC1;
	string_task2.data.timestamp=0;
	string_task2.data.value = ADCH;
	add_task(&string_task2);
	*/
	/*
	adc_values.results[current_channel] = value * vstep;
	
		if (current_channel == PINA0 ){
			setChannel(PINA1);
			// starting next conversion
			ADCSRA |= (1<<ADSC);
			
			task_t string_task;
			string_task.data.command = ADC1;
			string_task.data.timestamp=0;
			string_task.data.value = 3;
			add_task(&string_task);
			
		} else {
			setChannel(PINA0);
			
			task_t string_task;
			string_task.data.command = ADC1;
			string_task.data.timestamp=0;
			string_task.data.value = 3;
			add_task(&string_task);
		}
	*/
	conversionIsInProgress = false;
	
}
