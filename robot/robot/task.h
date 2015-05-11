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
	STRING=0,
	INIT_CONN=1,    
	ACK_INIT_CONN=2,
	PING=3,			
	STOP_CONN=4,
	
	MOTOR_X_Y=20,
	MOTOR_R=21,
	MOTOR_L=22,
	
	PID_ERROR=40,			
	
	ADC1=60,
	ADC2=63,
	ADC_PI_ERROR=61,
	LED=62,
	
	DEBUG11=80
	// NOTE!!! if new commands are added to task_command_t MAX_FUNCTION_POINTER_INDEX has to be updated.
} task_command_t;

/**
* Represents the maximum number off commands acceptable by the com port.
* NOTE!!! if new commands are added to task_command_t MAX_FUNCTION_POINTER_INDEX has to be updated.
*/
#define FUNCTION_POINTER_SIZE	11

/**
* Clears the task queue.
*/
#define clear_task_fifo()			{while(delete_task());}

/**
* Uses to create a generic way to handle the in/out data
* NOTE!!! if the size of the buffer is changed task_buffer_copy() has to be updated
*/
typedef union
{
    struct
    {
        task_command_t command;
        uint32_t timestamp;
		union{
			uint32_t value;
			uint8_t u8[4];
			char *str;
		};
    } data;
    uint8_t buffer[9];
} task_t;

/**
* Encapsulates the function pointer type for the incoming packages.
*/
typedef void (*task_function_ptr)(task_t*);

extern task_function_ptr do_task[FUNCTION_POINTER_SIZE];

void task_manager_init(void);
void recive_task_init(void);
void add_task(task_t *task);
uint32_t get_task_number(void);
task_t* delete_task(void);

void send_ack(task_t *task);
void init_recive_task(void);

#endif /* TASK_H_ */