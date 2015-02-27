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
#include "adc.h"

// 1 represents 100 ms
#define CLOCK_INTERVAL		10
#define CHARGING_INTERVAL	10
#define PING_INTERVAL		10
#define COMM_PROT_INTERVAL	2
#define ADC_INTERVAL		2
#define CARD_READ_INTERVAL	5
#define LCD_UPDATE_INTERVAL	5
#define KEY_INTERVAL		1
#define SEND_ADC_VALUE		5

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
	uint8_t adc_timer = COMM_PROT_INTERVAL;
	uint8_t send_adc_value = SEND_ADC_VALUE;
	
	bool run_clock = false;
	bool run_com_prot = false;
	bool run_adc = false;
	bool run_send_adc_value = false;

	DDRB|=(1<<PB0);
	led_off();
	
	status.byte[0]=0;
	adc_measurement_init();
	enableADC();
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
			if(--adc_timer == 0)
			{
				adc_timer = ADC_INTERVAL;
				run_adc = true;
			}
			if(--run_send_adc_value == 0)
			{
				run_send_adc_value = SEND_ADC_VALUE;
				run_send_adc_value = true;
			}
			
		}
		if (run_clock) 
		{
			run_clock = false;
			clock_tick();
			//get_date_to_string(date1p);
			//USART_transmit_string("AT+BAUD8");
			
		}
		if (run_com_prot)
		{
			run_com_prot = false;
			com_prot_task();
		}
		
		if (run_adc)
		{
			run_adc = false;
			handleMeasurement();
		}
		
		if (run_send_adc_value)
		{
			run_send_adc_value = false;
			send_adc_value_to_pc();
		}
	}
	return 1;
}