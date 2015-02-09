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

uint8_t task_count;
task_function_ptr do_task[MAX_FUNCTION_POINTER_INDEX]; // can be declared as half size


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
	//do_task[ACK_INIT_CONN]		= &ack_communication_init;
	//do_task[ACK_PING]			= &ack_ping;
	//
	//do_task[ACK_ASK_DAY]        = &set_day;
	//do_task[ACK_ASK_MONTH]      = &set_month;
	//do_task[ACK_ASK_YEAR]       = &set_year;
	//
	//do_task[ACK_ASK_SECOND]     = &set_second;
	//do_task[ACK_ASK_MINUTE]     = &set_minute;
	//do_task[ACK_ASK_HOUR]       = &set_hour;
	//
	//do_task[ACK_ASK_PRICE]		= &set_price;
	//
	//do_task[ACK_SEND_CARD1]		= &ack_card1;
	//do_task[ACK_SEND_CARD2]		= &ack_card2;
	//
	//do_task[ACK_CARD_VALID]		= &ack_card_valid;
	//do_task[ACK_SEND_PIN]		= &ack_pin;
	//do_task[ACK_VALID_PIN]		= &ack_pin_valid;
	//
	//do_task[ACK_START_CHARGING] = &ack_start_charging;
	//do_task[ACK_FINISH]			= &ack_finish;
	//do_task[ACK_PARTIAL_COSTS]	= &ack_partial_cost;
	//do_task[ACK_ERROR]			= &ack_error;
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
            temp_task = head->task;
        }
        else
        {
            temp_task = head->task;
            head = NULL;
            tail = NULL;
        }
        task_count--;
		//free(temp_head->task);
        free(temp_head);
        return &temp_task;
    }
}