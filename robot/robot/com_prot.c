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

timer_t ping_tmr;


/**
* \brief Used to initialize the communication.
* NOTE!!! The timer and USART functionalities have to be initialized prior to this function call.
*
* \author Alexandru
*
* \param
*
* \return void
*/
void communication_init(task_t *task)
{
	if (task->data.value == 0)
	{
		task_t comm_init = {.data.command = ACK_INIT_CONN, .data.value = 0};
		status.system.connected = true;
		clear_task_fifo();
		add_task(&comm_init);
		add_task(&comm_init);
	}
}

/**
* \brief Handles the message for the STOP command.
*
* \author Alexandru
*
* \param task Used to specify a pointer to a specific received task.
*
* \return void
*/
void stop(task_t *task)
{
	if (task->data.value == 0)
	{
		status.system.connected = false;
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
void ping(void)
{
	task_t ping = {.data.command = PING, .data.value = 0};
	
	add_task(&ping);
	tmr_start(&ping_tmr,SEC1);
}


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
	
	if (status.system.connected==true)
	{
		if (tmr_exp(&ping_tmr)) //send ping message every sec
		{
			tmr_start(&ping_tmr,SEC1);
			ping();
		}
		if (status.system.sending_task == false) //if th
		{
			tx_task = delete_task();
			if (tx_task != NULL)
			{
				if (tx_task->data.command == STRING)
				{

				}
				else
				{
					USART_transmit_command(tx_task);
				}
			}
		}
	}
	if (status.system.task_received==true)
	{
		status.system.task_received=false;
		do_task[usart_rx_task.data.command](&usart_rx_task);
	}
}

