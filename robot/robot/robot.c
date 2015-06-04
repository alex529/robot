/*
* Car_Charger_Station.c
*
* The robot.c file is in charge of scheduling all the functionality
* that needs to be handled by the client.
*
* Created: 3/3/2014 11:13:44 AM
*  Author: Administrator
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "isr.h"
#include "usart.h"
#include "task.h"
#include "common.h"
#include "date.h"
#include "com_prot.h"
#include "motor.h"

#define start(x){do_handler = true; x=true;}

// 1 represents 10 ms
#define CLOCK_INTERVAL				100
#define COMM_PROT_INTERVAL			10
#define MOTOR_INTERVAL				2

volatile bool run_card_reader = false;
volatile void (*control)();
timer_t test;

uint8_t led_int = 15;

void set_pid_int(task_t *task)
{
	led_int=task->data.value;
}
/**
* \brief Represents a scheduler implementation the scheduler ticking every 100ms.
*
* \author Alexandru
*
* \param
*
* \return int
*/
int main(void)
{
	uint8_t clock_timer					= CLOCK_INTERVAL;
	uint8_t com_prot_timer				= COMM_PROT_INTERVAL;
	uint8_t motor_timer					= MOTOR_INTERVAL;
	bool do_handler				= false;
	bool run_clock				= false;
	bool run_com_prot			= false;
	bool run_motor				= false;
	
	DDRB|=(1<<PB7);
	DDRA|=(1<<PA1);
	DDRA|=(1<<PA0);
	//led_off();
	
	status.byte[0]=0;
	USART_init();
	timer1_init();
	recive_task_init();
	motors_init();
	
	DDRB|=(1<<PB7);
	DDRA|=(1<<PA1);
	DDRA|=(1<<PA0);
	led_on1();
	led_on();
	sei();
	
	while(1)
	{
		if(system_tic == true) // 10 millisecond has passed
		{
			system_tic = false;
			if(--clock_timer == 0)
			{
				clock_timer = CLOCK_INTERVAL;
				start(run_clock);
			}
			if(--com_prot_timer == 0)
			{
				com_prot_timer = COMM_PROT_INTERVAL;
				start(run_com_prot);
			}
			if(--motor_timer == 0)
			{
				motor_timer = MOTOR_INTERVAL;
				start(run_motor);
			}
			
		}
		if(do_handler)/*get_line_error();*/
		{
			do_handler = false;
			if (run_clock)
			{
				run_clock = false;
				clock_tick();
			}
			if (run_com_prot)
			{
				run_com_prot = false;
				com_prot_task();
			}
			if (run_motor)
			{
				run_motor = false;
				motor_handler();
			}
		}				
	}
	return 1;
}