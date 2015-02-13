/*
* com_prot.c
*
* The com_prot.c file is in charge of the communication layer of the client,
* servicing the pending tasks and the received once.
*
*  Created: 4/28/2014 1:45:03 AM
*  Author: Alexandru Savin
*/

#include <stdlib.h>
#include "task.h"
#include "timer.h"
#include "common.h"
#include "date.h"
#include "usart.h"
#include "com_prot.h"

timer_t ping_tmr,tx_task_tmr,init_con_tmr;


/**
 * \brief This function handles all the communication between the server and the MCU.
 *
 * \author Alexandru
 *
 * \param 
 * 
 * \return void
 */
void com_prot_task(void)
{
	static task_t *tx_task;
	static task_t temp_tx_task;
	static uint8_t task_error_count = 0;
    if ((status.system.ack_received == false) && (status.system.task_sent == false) && (status.system.connected == true)) // check if comport idle if yes send next command
    {//normal operation
        tx_task = delete_task();
        if (tx_task != NULL)
        {
	        temp_tx_task = *tx_task;
            USART_Transmit_command(tx_task);
			tmr_start(&tx_task_tmr,SEC1);
        }
    }
	else if(status.system.connected == false)
    {// init connection
        tx_task = delete_task();
        if (tx_task != NULL)
			if(tx_task->data.command == INIT_CONN)
			{
				temp_tx_task = *tx_task;
				USART_Transmit_command(tx_task);
				tmr_start(&tx_task_tmr,SEC1);
			}        
    }
    else if (status.system.ack_received == true)
    {//parse received command
	    task_t rx_task;	    
	    task_buffer_copy(rx_task.buffer,rx_buffer);
		if (rx_task.data.command == (temp_tx_task.data.command+1))
		{	
		    do_task[rx_task.data.command](&rx_task);
		    status.system.task_sent = false;
		    status.system.ack_received = false;
		}
		else
		{
			USART_Transmit_command(&temp_tx_task);
			tmr_start(&tx_task_tmr,SEC1);
		}
    }
	else if (status.system.task_sent == true && status.system.ack_received == false && tmr_exp(&tx_task_tmr))
	{//ack not received sent command again 3 times after re initialize communication
		USART_Transmit_command(tx_task);
		tmr_start(&tx_task_tmr,SEC1);
		if (task_error_count++ == 3)
		{
			task_error_count = 0;
			status.system.finished_charging = true; // TODO take care if interrupted in mid transaction need to service both com init and finish transaction
			communication_init();
		}
	}						  //taking care of recursion
	if (tmr_exp(&ping_tmr) && status.system.connected == true)
	{// if ack ping not received after 3 seconds
		communication_init();
	}
}


/**
 * \brief Used to initialize the communication. 
 * NOTE!!! The timer and USART functionalities have to be initialized prior to this function call.
 * NOTE!!! The MCU will hang inside the function until a positive responce from the server.
 *
 * \author Alexandru
 *
 * \param 
 * 
 * \return void
 */
void communication_init(void)
{
    task_t comm_init = {.data.command = INIT_CONN, .data.value = 0};
    status.byte[0]=0;status.byte[1]=0;
    while(!status.system.connected)
    {
        if (tmr_exp(&init_con_tmr))
        {
			clear_task_fifo();
            add_task(&comm_init);
            tmr_start(&init_con_tmr,SEC1);
        }
        com_prot_task();
    }
    get_date();
}

/**
 * \brief Handles the ACK message for the INIT_COMM command.
 *
 * \author Alexandru
 *
 * \param task Used to specify a pointer to a specific received task.
 * 
 * \return void
 */
void ack_communication_init(task_t *task)
{
    if (task->data.value == 0)
    {
        status.system.connected = true;
    }
}

/**
 * \brief Handles the ACK message for the FINISH command.
 *
 * \author Alexandru
 *
 * \param task Used to specify a pointer to a specific received task.
 * 
 * \return void
 */
void ack_finish(task_t *task)
{
    if (task->data.value == 0)
    {
        status.system.transaction_in_progres = false;
    }
}

/**
 * \brief Handles the ACK message for the PING command.
 *
 * \author Alexandru
 *
 * \param task Used to specify a pointer to a specific received task.
 * 
 * \return void
 */
void ack_ping(task_t *task)
{
    if (task->data.value != 0)
    {
        task_t ping = {.data.command = PING, .data.value = 0};
        add_task(&ping);
        tmr_start(&ping_tmr,SEC1);
    }
}

/**
 * \brief Handles the ACK message for the ERROR command.
 *
 * \author Alexandru
 *
 * \param task Used to specify a pointer to a specific received task.
 * 
 * \return void
 */
void ack_error(task_t *task)
{
	communication_init();
}

