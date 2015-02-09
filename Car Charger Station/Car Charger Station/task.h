/*
 * task.h
 *
 * Created: 2/28/2014 12:46:55 PM
 *  Author: Administrator
 */ 


#ifndef TASK_H_
#define TASK_H_

#include <avr/io.h>
#include <stdlib.h>

/**
* Represents all the commands available for the com protocol
* NOTE!!! if new commands are added to task_command_t MAX_FUNCTION_POINTER_INDEX has to be updated.
*/
typedef enum
{
    INIT_CONN,      ACK_INIT_CONN,
	PING,			ACK_PING,
	
    ASK_DAY,        ACK_ASK_DAY,
    ASK_MONTH,      ACK_ASK_MONTH,
    ASK_YEAR,       ACK_ASK_YEAR,
	
	ASK_SECOND,     ACK_ASK_SECOND,
    ASK_MINUTE,     ACK_ASK_MINUTE,
    ASK_HOUR,       ACK_ASK_HOUR,
	
    ASK_PRICE,      ACK_ASK_PRICE,
	
    SEND_CARD1,     ACK_SEND_CARD1,
    SEND_CARD2,     ACK_SEND_CARD2,
	
    CARD_VALID,     ACK_CARD_VALID,
    SEND_PIN,       ACK_SEND_PIN,
    VALID_PIN,      ACK_VALID_PIN,
	
    START_CHARGING, ACK_START_CHARGING,
    PARTIAL_COST,   ACK_PARTIAL_COSTS,
    FINISH,         ACK_FINISH,
	ERROR,			ACK_ERROR
	// NOTE!!! if new commands are added to task_command_t MAX_FUNCTION_POINTER_INDEX has to be updated.
} task_command_t;

/**
* Represents the maximum number off commands acceptable by the com port.
* NOTE!!! if new commands are added to task_command_t MAX_FUNCTION_POINTER_INDEX has to be updated.
*/
#define MAX_FUNCTION_POINTER_INDEX	36 //if NEW TASKS are added increment value

/**
* Clears the task queue.
*/
#define clear_task_fifo()			{while(delete_task());}

/**
* Uses to create a generic way to handle the in/out data
*/
typedef union
{
    struct
    {
        task_command_t command;
        uint32_t value;
    } data;
    uint8_t buffer[5];
} task_t;

/**
* Encapsulates the function pointer type for the incoming packeges.
*/
typedef void (*task_function_ptr)(task_t*);

extern task_function_ptr do_task[MAX_FUNCTION_POINTER_INDEX];

void task_manager_init(void);
void recive_task_init(void);
void add_task(task_t *task);
uint32_t get_task_number(void);
task_t* delete_task(void);

void send_ack(task_t *task);
void init_recive_task(void);

#endif /* TASK_H_ */