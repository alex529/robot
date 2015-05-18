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
	STRING=0,							//0x00
	INIT_CONN=1,						//0x01
	ACK_INIT_CONN=2,					//0x02
	PING=3,								//0x03
	STOP_CONN=4,						//0x04
	
	MOTOR_X_Y=20,						//0x14
	MOTOR_R=21,							//0x15
	MOTOR_L=22,							//0x16
	MOTOR_ANGLE=23,                     //0x17
	MOTOR_MAGNITUDE=24,                 //0x18
	MOTOR_F=25,                         //0x19
	MOTOR_B=26,                         //0x1A
	
	PID_ERROR=40,						//0x28
	
	ADC1=60,							//0x3C
	ADC_PI_ERROR=61,					//0x3D
	LED=62,								//0x3E
	
	TAKE_OVER = 80,						//0x50
	TAKE_OVER_ACK = 81,					//0x51
	GIVE_BACK_CONTROL = 82,				//0x52
	GIVE_BACK_CONTROL_ACK = 83,			//0x53
	STATE_COMMAND = 84,					//0x54
	SET_SPEED =85,						//0x55
	
	DEBUG11=100							//0x64
	// NOTE!!! if new commands are added to task_command_t MAX_FUNCTION_POINTER_INDEX has to be updated.
} task_command_t;

/**
* Represents the maximum number off commands acceptable by the com port.
* NOTE!!! if new commands are added to task_command_t MAX_FUNCTION_POINTER_INDEX has to be updated.
*/
#define FUNCTION_POINTER_SIZE	255

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