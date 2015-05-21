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
#include "motor.h"
#include "adc.h"

#define increment_pulse_timer(x) { if(++x>9){x=0;}}

volatile status_t status;
volatile enable_features_t enable_features;
volatile uint32_t system_tic;
volatile uint16_t p_r, p_l;
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
	}
}

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
	//0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
	
	if (usart_tx_task.data.command==STRING)
	{
		
		if (tx_index==0)
		{
			//string = usart_tx_task.data.str;
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
	static uint8_t pulse_timer = 0,sys_timer = 0;
	if (++pulse_timer>7)//optimized for pid
	{
		pulse_timer =0;
		l_motor.pulses = p_l;
		r_motor.pulses = p_r;
		p_r = 0;
		p_l = 0;
	}
	if (++sys_timer>9)
	{
		sys_timer = 0;
		system_tic = true;
	}
	milliseconds++;
}

ISR(INT0_vect){
	int0_toggle_edge();
	p_r++;
}

ISR(INT1_vect){
	int1_toggle_edge();
	p_l++;
}

ISR(ADC_vect) {
	
		uint32_t value=0;
		uint32_t vstep = 488;
		
		value = ADCL;
		value = value + (ADCH<<8);
		result = value * vstep / 100;
		
		conversionIsInProgress = false;
		new_data_available = true;
		new_data_available_to_transmit = true;
}
