/**
* usart.c
* 
* The usart.c file is in charge of initializing the USART module and  
* contains functionality for sending task structures to the server.
*
*  Created: 2/17/2014 12:48:55 PM
*  Author: Alexandru Savin
*/

#include <avr/io.h>
#include <stdbool.h>
#include "usart.h"
#include "task.h"
#include "common.h"

	
volatile char tx_buffer[USART_FRAME_SIZE]="AT+BAUD9", rx_buffer[USART_FRAME_SIZE];

/**
* \brief Setting up the USART module full duplex 8 bit frame 19200bps.
*
* \author Alexandru
*
* \return void
*/
void USART_init(void)
{
    ///* Set baud rate*/
    //UBRRH = (unsigned char)(baud>>8);
    //UBRRL = (unsigned char)baud;
    
    
    //URSEL: Register Select
    //use data sheet or http://www.wormfood.net/avrbaudcalc.php to calculate
    UBRRH&=~(1<<URSEL);
    UBRRL = 129; //baud rate set to 19200bps //fosc/(8*baud)-1
    
    //U2X: Double the USART Transmission Speed
    UCSRA =(1<<U2X);
    
    //RXCIE: RX Complete Interrupt Enable
    //UDRIE: USART Data Register Empty Interrupt Enable
    //TXEN: Transmitter Enable
    //RXEN: Receiver Enable
    UCSRB|=(1<<RXCIE)|(1<<RXEN)|(1<<TXEN);
    
    //URSEL: Register Select
    //UCSZ1:0: Character Size 8bit frame size
    UCSRC|=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}


/**
* \brief Decodes the task structure into a series of bytes rearranging them for proper communication with the server and starts transmitting.
*
* \author Alexandru
*
* \param task Used to specify a pointer to a task, that needs to be transmitted.
* 
* \return void
*/
void USART_Transmit_command(task_t* task)
{
    task_buffer_copy(tx_buffer,task->buffer);
    swap_uint8_t(tx_buffer[1],tx_buffer[4]);
    swap_uint8_t(tx_buffer[2],tx_buffer[3]);
	enable_uart_transmision();
	
	status.system.ack_received = false;
	status.system.task_sent = true;
}