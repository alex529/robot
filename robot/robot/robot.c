/*
* Car_Charger_Station.c
*
* The Car_Charger_Station.c file is in charge of scheduling all the functionality
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

// 1 represents 100 ms
#define CLOCK_INTERVAL		10
#define CHARGING_INTERVAL	10
#define PING_INTERVAL		10
#define COMM_PROT_INTERVAL	2
#define CARD_READ_INTERVAL	5
#define LCD_UPDATE_INTERVAL	5
#define KEY_INTERVAL		1

volatile bool run_card_reader = false;
timer_t test;

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
	uint8_t clock_timer = CLOCK_INTERVAL;
	uint8_t com_prot_timer = COMM_PROT_INTERVAL;
	
	bool run_clock = false;
	bool run_com_prot = false;

	DDRB|=(1<<PB0);
	led_off();
	
	status.byte[0]=0;
	USART_init();
	timer1_init();
	recive_task_init();
	
	sei();
	
	while(1)
	{
		if(system_tic == true) // 1millisecond has passed
		{
			system_tic = false;
			if(--clock_timer == 0)
			{
				clock_timer = CLOCK_INTERVAL;
				run_clock = true;
			}
			if(--com_prot_timer == 0)
			{
				com_prot_timer = COMM_PROT_INTERVAL;
				run_com_prot = true;
			}
			
		}
		if (run_clock) 
		{
			run_clock = false;
			clock_tick();
		}
		if (run_com_prot)
		{
			run_clock = false;
			com_prot_task();
		}
	}
	return 1;
}