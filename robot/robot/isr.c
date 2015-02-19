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
			}
			
		}
	}
	else
	{
		set_usart(usart_tx_task.buffer[tx_index]);
		if (++tx_index>get_array_len(usart_tx_task.buffer)-1)
		{
			tx_index = 0;
			
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
	
	int value=0;

	//the measured value is 2+8 bits long. The following 2 lines creates a 10bit value from the 2+8 bit values
	value = ADCL;
	value = value + (ADCH<<8);
	
	if (itr8307sOnly) {
		if (current_channel < PINA5 ){
			// set channel ++
		} else {
			 //set channel PINA0
		}
	} else {
		if (current_channel < PINA6 ){
			// set channel ++
		} else {
			//set channel PINA0
		}
	}
	
	//store result
	//create task if conversion is finished
	
	conversionIsInProgress = false;
}