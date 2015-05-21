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
#include "state_machine.h"
#include "state_machine_event_buffer.h"

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
#define send_pid(x){task_t pid_err = {.data.command = PID_ERROR, .data.value = x};add_task(&pid_err);}
	
#define send_left_m(x) {task_t m_info = {.data.command = MOTOR_L, .data.value = l_motor.rpm};add_task(&m_info);}
#define send_right_m(x){task_t m_info = {.data.command = MOTOR_R, .data.value = r_motor.rpm};add_task(&m_info);}
	
// #define Kp 16
// #define Ki 0
// #define Kd 0

uint16_t Kp=16, Ki=0, Kd=0;

#define aplie_kd(x)(((x)*Kd)>>7)
/*#define ERROR_STEP 50*/

#define LINE_FOUND 127 

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
	//if (status.system.start_line)//TODO if java tool is used uncomment
	{
		static int16_t i_factor,last_error,d_factor,p_factor, pid;
		int8_t error = 0;
		read_switches();
		toggle_led();
		switch (led.array)
		{
			case 0b00011100 : //0
	
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
	// 		l_motor.rpm=190;
	// 		r_motor.rpm=200;
			break;
			case 0b01111111:
	// 		l_motor.rpm=0;
	// 		r_motor.rpm=0;
			break;
			//more cases for the special lines
			default:
			{
				//error =0;
				
			}
			break;
		}
		
		p_factor = error*Kp;
		i_factor =i_factor+( Ki*error);
		if (i_factor<-32)
		{
			i_factor=-32;
		} 
		else if (i_factor>32)
		{
			i_factor=32;
		}
		d_factor =aplie_kd(error-last_error);
		pid=p_factor+i_factor+d_factor;
		
		if(error<=0)
			r_motor.rpm = r_motor.ref_rpm + pid;
		if(error>=0) 
			l_motor.rpm = l_motor.ref_rpm - pid;
			
	
		static uint8_t info_timer=5;//5*70ms = 350ms
		if(--info_timer==0)
		{
			info_timer=10;
// 			send_led_info();
// 			send_left_m();
// 			send_right_m();
 			send_pid(pid);
 			task_t led_info  = {.data.command = PID_KP, .data.value = p_factor};add_task(&led_info);
 			task_t led_info1 = {.data.command = PID_KI, .data.value = i_factor};add_task(&led_info1);
 			task_t led_info2 = {.data.command = PID_KD, .data.value = d_factor};add_task(&led_info2);
		}
}
}

void send_sensor_values(void) {
	read_switches();
	send_led_info();
}

void sensor_eval(void) {
	uint8_t sensor_value = led.array; 
	task_t system_state = {.data.command = DEBUG11, .data.timestamp=0, .data.value=led.array};
	add_task(&system_state);
	if (sensor_value == LINE_FOUND)
	{
		add_event(EVENT_LINE_FOUND);
	}
}

void start_line(task_t *task)
{
	status.system.start_line=task->data.value;
}

void set_Kp(task_t *task)
{
	Kp=task->data.value;
}

void set_Ki(task_t *task)
{
	Ki=task->data.value;
}

void set_Kd(task_t *task)
{
	Kd=task->data.value;
}
void set_pid(task_t *task)
{
	Kp=task->data.u8[0];
	Ki=task->data.u8[1];
	Kd=task->data.u8[2];
	task_t led_info  = {.data.command = PID_KP, .data.value = Kp};add_task(&led_info);
	task_t led_info1 = {.data.command = PID_KI, .data.value = Ki};add_task(&led_info1);
	task_t led_info2 = {.data.command = PID_KD, .data.value = Kd};add_task(&led_info2);
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
	status.system.motor_forward = true;
}