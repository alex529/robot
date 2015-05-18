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
#include "led.h"
#include "adc.h"
#include "state_machine.h"

#define start(x){do_handler = true; x=true;}

// 1 represents 10 ms
#define CLOCK_INTERVAL				100
#define COMM_PROT_INTERVAL			20
#define MOTOR_INTERVAL				1
#define LED_INTERVAL				7
#define PING_INTERVAL				100
#define COMM_PROT_INTERVAL			20
#define ADC_INTERVAL				50
#define SEND_ADC_VALUE_INTERVAL		50
#define STATE_MACHINE_INTERVAL		5
#define SENSOR_EVAL_INTERVAL		30

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
	uint8_t clock_timer			= CLOCK_INTERVAL;
	uint8_t com_prot_timer		= COMM_PROT_INTERVAL;
	uint8_t adc_timer			= COMM_PROT_INTERVAL;
	uint8_t motor_timer			= MOTOR_INTERVAL;
	uint8_t led_timer			= LED_INTERVAL;
	uint8_t send_adc_value_timer= SEND_ADC_VALUE_INTERVAL;
	uint8_t state_machine_value_timer = STATE_MACHINE_INTERVAL;
	uint8_t sensor_eval_timer	=SENSOR_EVAL_INTERVAL;
	
	bool do_handler				= false;
	bool run_com_prot			= false;
	bool run_adc				= false;
	bool run_send_adc_value		= false;
	bool run_motor				= false;
	bool run_led				= false;
	bool run_state_machine		= false;
	bool run_clock				= false;
	bool run_sensor_eval		= false;
	
	DDRB|=(1<<PB7);
	led_off();
	
	status.byte[0]=0;
// 	enableADC();
// 	adc_measurement_init();
	USART_init();
	timer1_init();
	recive_task_init();
	motors_init();
	led_init();
	
	enable_features.adc=false;
	enable_features.send_adc_value=false;
	enable_features.find_line=false;
	enable_features.controller=false;
	
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
			if(--led_timer == 0)
			{
				led_timer = LED_INTERVAL;
				start(run_led);
			}
// 			if(enable_features.adc == true && --adc_timer == 0)
// 			{
// 				adc_timer = ADC_INTERVAL;
// 				start(run_adc);
// 			}
// 			if(enable_features.send_adc_value == true && --send_adc_value_timer == 0)
// 			{
// 				send_adc_value_timer = SEND_ADC_VALUE_INTERVAL;
// 				start(run_send_adc_value);
// 			}
// 			if(--state_machine_value_timer == 0)
// 			{
// 				state_machine_value_timer = STATE_MACHINE_INTERVAL;
// 				start(run_state_machine);
// 			}
// 			if(--sensor_eval_timer == 0)
// 			{
// 				sensor_eval_timer = SENSOR_EVAL_INTERVAL;
// 				start(run_sensor_eval);
// 			}
			
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
				motors_controoler();
			}
			if (run_led)
			{
				run_led = false;
				//get_line_error();
			}
			
// 			if (run_adc)
// 			{
// 				run_adc = false;
// 				handleMeasurement();
// 			}
// 			
// 			if (run_send_adc_value)
// 			{
// 				run_send_adc_value = false;
// 				send_adc_value_to_pc();
// 			}			
// 			if (run_state_machine)
// 			{
// 				run_state_machine = false;
// 				state_machine();
// 			}
// 			if (run_sensor_eval)
// 			{
// 				run_sensor_eval = false;
// 				eval();
// 			}
			
		}
		
		
	}
	return 1;
}