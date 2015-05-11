/*
 * led.c
 *
 * Created: 24/04/2015 20:45:45
 *  Author: SavinA
 */ 

#include <avr/io.h>
#include <stdint.h>

#include "led.h"
#include "motor.h"
#include "math_Q.h"
#include "task.h"
#include "common.h"

//TODO: check if this works
#define read_switch(x)	(PINA & (1<<PA##x))
#define SW0 read_switch(0)
#define SW1 read_switch(1)
#define SW2 read_switch(2)
#define SW3 read_switch(3)
#define SW4 read_switch(4)
#define SW5 read_switch(5)
#define SW6 read_switch(6)
#define SW7 read_switch(7)

#define ON	1
#define OFF 0

#define send_led_info(){task_t led_info = {.data.command = LED, .data.value = led.array};add_task(&led_info);}
	
#define Kp 30
#define Ki 20
#define ERROR_STEP 50

//TODO: delete either 0 or 7
#define read_switches(){led.array = 0b01111111&(~(PINA));/*led.switches.sw0=SW0;\
	led.switches.sw1=SW1;\
	led.switches.sw2=SW2;\
	led.switches.sw3=SW3;\
	led.switches.sw4=SW4;\
	led.switches.sw5=SW5;\
	led.switches.sw6=SW6;\
/*led.switches.sw7=SW7;*/}

led_t led;


/**
* \brief  Returns the line error
*
*
* \return in8_t Returns a value between -3 and 3 where -3 represents the biggest left error and 3 the biggest right error  0 no error 10 if no line //TODO: rephrase  check if switches react as they should
*/
void get_line_error(void)
{
	static int16_t last_error;
	int16_t error = 0, p_factor, i_factor=0;
	read_switches();
	switch (led.array)
	{
		case 0b00011100 : //0
// 		if (r_motor.rpm<MAX_RPM-33)
// 		{
// 			r_motor.rpm += 33;
// 		}
// 		if (l_motor.rpm<MAX_RPM-33)
// 		{
// 			l_motor.rpm += 33;
// 		}
// 			return;

		l_motor.rpm=90;
		r_motor.rpm=100;

		break;
		case 0b00011000 : //-1
			error = -1;
		break;
		case 0b00001100 : //1
			error = 1;
		break;
		case 0b00111000 : //-2
			error = -2;
		break;
		case 0b00001110 : //2
			error = 2;
		break;
		case 0b00110000 : //-3
			error = -3;
		break;
		case 0b00000110: //3
			error = 3;
		break;
		case 0b01110000: //-4
			error = -4;
		break;
		case 0b00000111: //4
			error = 4;
		break;
		case 0b01100000: //-5
			error = -5;
		break;
		case 0b00000011: //5
			error = 5;
		break;
		case 0b01000000: //-6
			error = -6;
		break;
		case 0b00000001: //6
			error = 6;
		break;
		case 0b00000000:
		l_motor.rpm=290;
		r_motor.rpm=300;
		break;
		case 0b01111111:
		l_motor.rpm=0;
		r_motor.rpm=0;
		break;
		//more cases for the special lines
		default:
		{
			//error =0;
			
		}
		break;
	}
	toggle_led();
	p_factor = 0;///*Kp**/error;
	i_factor = /*Ki**/(error+last_error)>>1;
	last_error = error;
	status.byte[1]=i_factor;
	int16_t new_rpm;
	new_rpm = r_motor.rpm + i_factor-p_factor;
	if (new_rpm<33)
	{
		new_rpm = 33;
	}
	r_motor.rpm = new_rpm;
	new_rpm = l_motor.rpm - i_factor+p_factor;
	if (new_rpm<33)
	{
		new_rpm = 33;
	}
	l_motor.rpm = new_rpm;
	
// 	int16_t new_rpm;
// 		if (error<0)
// 		{
// 			new_rpm = r_motor.rpm+p_factor/*-int8_abs_Q(i_factor)*/;
// 			if (new_rpm<33)
// 			{
// 				new_rpm = 33;
// 			}
// 			r_motor.rpm = new_rpm;
// // 			if(error+last_error<-10)
// // 			{
// // 				r_motor.rpm-=ERROR_STEP;
// // 				l_motor.rpm-=ERROR_STEP;
// // 			}
// 		}
// 		else
// 		{
// 			new_rpm = l_motor.rpm-p_factor/*-int8_abs_Q(i_factor)*/;
// 			if (new_rpm<33)
// 			{
// 				new_rpm = 33;
// 			}
// 			l_motor.rpm = new_rpm;
// // 			if(error+last_error>10)
// // 			{
// // 				l_motor.rpm-=ERROR_STEP;
// // 				r_motor.rpm-=ERROR_STEP;
// // 			}
// 		}
	static uint8_t info_timer=5;//5*20ms = 100ms
	if(--info_timer==0)
	{
		info_timer=5;
		//send_led_info();
	}
}

void led_init(void)
{
	set_pin_as_input(A,0);
	set_pin_as_input(A,1);
	set_pin_as_input(A,2);
	set_pin_as_input(A,3);
	set_pin_as_input(A,4);
	set_pin_as_input(A,5);
	set_pin_as_input(A,6);
}