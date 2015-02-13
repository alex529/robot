/*
 * usart.h
 *
 * Created: 2/17/2014 12:49:08 PM
 *  Author: Alexandru Savin
 */


#ifndef USART_H_
#define USART_H_

#include "task.h"


/************************************************************************
 * Eable/dissable_uart_transmision has to be used because if not the UDRE 
 * interrupt will always be fired.                                      
 ************************************************************************/
/**
* \brief enables uart udre interrupt
*
*/
#define enable_uart_transmision()           {UCSRB|=(1<<UDRIE);}
/**
* \brief disables uart udre interrupt
*
*/
#define disable_uart_transmision()          {UCSRB&=~(1<<UDRIE);}
  
/**
* \brief Check if the UDRE interrupt is enable
* 
*/
#define check_udr_is_empty()				{UCSRB&(1<<TXCIE);}
    
/**
* \brief Returns the 8bit value that was received by the USART port
*
*/
#define get_usart()                         (UDR)
/**
* \brief Sets the 8bit value that will be sent by the USART port
*
*/
#define set_usart(value)                    {UDR=value;}


extern task_t usart_tx_task,usart_rx_task;


void USART_init(void);
void USART_transmit_command(task_t* task);
task_t* USART_receive_command(void);



#endif /* USART_H_ */