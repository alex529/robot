/**
* task.c
*
* The task.c file is in charge of managing the task handling queue and integrates
* all the ack functions in an array of function pointers
*
*
* Created: 2/28/2014 11:37:53 AM
* Author: Alexandru Savin
*/

#include <avr/io.h>
#include <stdlib.h>
#include "common.h"
#include "task.h"
#include "date.h"
#include "task.h"
#include "usart.h"
#include "com_prot.h"
#include "motor.h"
#include "take_over.h"

uint8_t task_count;
task_function_ptr do_task[FUNCTION_POINTER_SIZE];


/**
* Base skeleton for the queue.
*/
struct node
{
	task_t task;
	struct node *next;
} *head,*tail,*temp_head;


/**
* \brief Initializes the function pointer array with references to appropriate functions.
*
* \author Alexandru
*
* \param
*
* \return void
*/
void recive_task_init(void)
{
	//do_task[STRING]        = &set_day;
	do_task[INIT_CONN]        = &communication_init;
	//do_task[PING]			= &ack_ping;
	do_task[STOP_CONN]        = &stop;
	
	
	do_task[MOTOR_L]        = &set_left;
	do_task[MOTOR_R]        = &set_right;
	do_task[MOTOR_X_Y]      = &set_motors;
	do_task[TAKE_OVER]		= &take_over_command;	
	do_task[GIVE_BACK_CONTROL] =&give_back_control_command;
//
	//do_task[PID_ERROR]       = &set_year;
//
	//do_task[ADC]     = &set_second;
//
	//do_task[DEBUG]     = &set_minute;
}
/**
* \brief Initializes the head and tail of the task queue.
*
* \author Alexandru
*
* \param
*
* \return void
*/
void task_manager_init(void)
{
	head = NULL;
	tail = NULL;
}

/**
* \brief
*
* \author Alexandru
*
* \param
*
* \return int Returns the number of task in the queue.
*/
uint32_t get_task_number(void)
{
	return task_count;
}

/**
* \brief Adds a copy of a specific task to the task queue.
*
* \author Alexandru
*
* \param task Used to specify a pointer to a task.
*
* \return void
*/
void add_task(task_t *task)
{
	if(task==NULL) return;
	
	struct node *temp_node = malloc(1*sizeof(*temp_node));
	//task_t *temp_task = malloc(1*sizeof(*temp_task));

	//temp_task->data.command=task->data.command;
	//temp_task->data.value=task->data.value;

	temp_node->task=*task;
	temp_node->next=NULL;

	if (head == NULL || tail == NULL)
	{
		tail = temp_node;
		head = temp_node;
		head->next=tail;
	}
	else
	{
		tail->next = temp_node;
		tail = temp_node;
	}
	task_count++;
}

/**
* \brief Deletes the first task in the list.
*
* \author Alexandru
*
* \param
*
* \return task_t* Returns a pointer the task that was deleted.
*/
task_t* delete_task(void)
{
	static task_t temp_task; //hack
	temp_task.data.command = 0;
	temp_task.data.value = 0;
	
	temp_head = head;
	
	if (temp_head == NULL || tail == NULL || task_count == 0)
	{
		return NULL;
	}
	else
	{
		if (temp_head->next != NULL)
		{
			head = head->next;
			temp_task = temp_head->task;
		}
		else
		{
			temp_task = temp_head->task;
			head = NULL;
			tail = NULL;
		}
		task_count--;
		//free(temp_head->task);
		free(temp_head);
		return &temp_task;
	}
}