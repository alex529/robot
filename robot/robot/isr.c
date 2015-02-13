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
		//task_buffer_copy(usart_tx_task.buffer,usart_rx_task.buffer);
		//enable_uart_transmision();
		//swap_uint8_t(rx_buffer[1],rx_buffer[4]);//bytes need to be swapped due to different arrangement in the server
		//swap_uint8_t(rx_buffer[2],rx_buffer[3]);
		
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
	
	set_usart(usart_tx_task.buffer[tx_index]);
	if (++tx_index>get_array_len(usart_tx_task.buffer)-1)
	{
		tx_index = 0;
		
		disable_uart_transmision();
		status.system.sending_task = false;
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