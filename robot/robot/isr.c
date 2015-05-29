/**
* isr.c
*
* The isr.c file is in charge of servicing the ISR routines.
*
*  Created: 2/17/2014 12:57:18 PM
*  Author: Alexandru Savin
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include "usart.h"
#include "isr.h"
#include "timer.h"
#include "common.h"
#include "motor.h"

#define increment_pulse_timer(x) { if(++x>9){x=0;}}

volatile status_t status;
volatile enable_features_t enable_features;
volatile uint32_t system_tic;
volatile uint16_t p_r, p_l;
/**
* \brief Adds the received data to the rx_buffer when the frame is received a flag is raised.
*			When the frame is ready the value bites are swapped.
*
* \author Alexandru
*
* \param Vector for the interrupt.
*
* \return
*/
ISR(USART_RXC_vect)
{
	static uint8_t rx_index = 0;
	usart_rx_task.buffer[rx_index]=get_usart();
	if(++rx_index>get_array_len(usart_rx_task.buffer)-1)
	{
		rx_index = 0;
		status.system.task_received=true;
	}
}

/**
* \brief Copies the tx_buffer byte by byte to the U SART module buffer.
*
* \author Alexandru
*
* \param Vector for the  interrupt.
*
* \return
*/
ISR(USART_UDRE_vect)
{
	static uint8_t tx_index = 0;
	static char *string;
	
	if (usart_tx_task.data.command==STRING)
	{
		
		if (tx_index==0)
		{
			//string = usart_tx_task.data.str;
		}
		if (tx_index++ < 5)
		{
			//set_usart(usart_tx_task.buffer[tx_index]);
		}
		else
		{
			set_usart(*string);
			if(!*++string){
				tx_index=0;
				disable_uart_transmision();
				status.system.sending_task = false;
			}
			
		}
	}
	else
	{
		set_usart(usart_tx_task.buffer[tx_index]);
		if (++tx_index>get_array_len(usart_tx_task.buffer)-1)
		{
			tx_index = 0;
			
			disable_uart_transmision();
			status.system.sending_task = false;
		}
	}
}

/**
* \brief Increments the global millisecond variable that is needed for the timer.c module and
*			facilitates a system tick(100ms) used for scheduling purposes in the main loop.
*
* \author Alexandru
*
* \param Vector for the  interrupt.
*
* \return
*/
ISR(TIMER1_COMPA_vect)
{
	static uint8_t pulse_timer = 0,sys_timer = 0;
	static int16_t l_m,r_m;
	
	if (++pulse_timer>15)//optimized for pid
	{
		pulse_timer = 0;
		
		l_motor.error = l_motor.ref_pulses - p_l;
		r_motor.error = r_motor.ref_pulses - p_r;
		p_r = 0;
		p_l = 0;
		
		{//left motor
			if(l_motor.error<0)//too fast
			{
				uint8_t break_val= l_motor.error*2;
				break_l(0);
				l_m+=l_motor.error;
				if (l_m<0)
				{
					l_m=0;
				}
			}
			// 			else if(l_motor.error<0)
			// 			{
			// 				uint16_t motor = get_left_m();
			// 				motor -= l_motor.error;
			// 				if (motor<0)
			// 				{
			// 					motor=0;
			// 				}
			// 				set_left_m(motor);
			// 			}
			else if (l_motor.error == 0)// no error keep it going
			{
				if (l_motor.ref_pulses==0)//break
				{
					set_l_stop();
					set_left_m(0);
				}
				else//motor has the proper speed don't change shit
				{		
					if (l_motor.direction == FORWARD)
					{
						set_l_forward();
					}
					else
					{
						set_l_backward();
					}
				}
				
			}
			else//acceleration needed
			{
				if (l_motor.breaking == OFF)
				{
					l_m = get_left_m();
				}
				l_m += l_motor.error;
				if (l_m>255)
				{
					l_m=255;
				}
				if (l_motor.direction == FORWARD)
				{
					set_l_forward();
				}
				else
				{
					set_l_backward();
				}
				set_left_m(l_m);
			}
		}
		{//right motor
			if(r_motor.error<0)//too fast
			{
				uint8_t break_val= r_motor.error*2;
				break_r(0);
				r_m+=r_motor.error;
				if (r_m<0)
				{
					r_m=0;
				}
			}
			else if (r_motor.error == 0)// no error keep it going
			{
				toggle_led();
				if (r_motor.ref_pulses==0)//break
				{
					set_r_stop();
					set_right_m(0);
				}
				else//motor has the proper speed don't change shit
				{
					
				}
				
			}
			else//acceleration needed
			{
				if (r_motor.breaking == OFF)
				{
					r_m = get_right_m();
				}
				r_m += r_motor.error;
				if (r_m>255)
				{
					r_m=255;
				}
				if (r_motor.direction == FORWARD)
				{
					set_r_forward();
				}
				else
				{
					set_r_backward();
				}
				set_right_m(r_m);
			}
		}
	}
	if (++sys_timer>9)
	{
		sys_timer = 0;
		system_tic = true;
	}
	milliseconds++;
}

ISR(INT0_vect){
	int0_toggle_edge();
	p_r++;
}

ISR(INT1_vect){
	int1_toggle_edge();
	p_l++;
}
