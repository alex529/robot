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
#include "control_logic.h"

#define send_led_info(){task_t led_info = {.data.command = LED, .data.value = led.array};add_task(&led_info);}
#define send_state_info(){task_t sys_info = {.data.command = STATE_COMMAND, .data.value = sys};add_task(&sys_info);}
#define send_pid(x){task_t pid_err = {.data.command = PID_ERROR, .data.value = x};add_task(&pid_err);}

#define send_left_m(x) {task_t m_info = {.data.command = MOTOR_L, .data.value = l_motor.rpm};add_task(&m_info);}
#define send_right_m(x){task_t m_info = {.data.command = MOTOR_R, .data.value = r_motor.rpm};add_task(&m_info);}

#define clear_rpm(){l_motor.ref_rpm = 0;r_motor.ref_rpm = 0;l_motor.rpm = 0;r_motor.rpm = 0;}
	
#define state_delay(time,next_state){if (do_once){do_once=false;tmr_start(&delay,time);}if (tmr_exp(&delay)){c_state = next_state;do_once=true;}}


// #define Kp 16
// #define Ki 0
// #define Kd 0

static int16_t Kp=24, Ki=0, Kd=0;
static uint8_t err4=6, err5=9, err6=11;

#define aplie_Kd(x)(((x)*Kd)/2)
#define aplie_Ki(x)(((x)*Ki))
/*#define ERROR_STEP 50*/

#define REF_RPM 500
#define MAX_DEVIATION 300
#define MAX_I_DEVIATION 40

volatile led_t led;


typedef enum
{
	IDLE,
	STOP,
	START_TRACK,
	GET_ON_TRACK,
	FALLOW_TRACK1,
	DO_CIRCLE,
	FALLOW_TRACK2,
	DO_WALL,
	S_DELAY_1,
	S_DELAY_2,
	S_DELAY_3,
	S_DELAY_4,
	S_DELAY_5,
	S_DELAY_6,
	S_DELAY_7,
	S_DELAY_8,
	S_DELAY_9,
} sys_state_t;

sys_state_t sys = IDLE;

/**
* \brief  Returns the line error
*
*
* \return in8_t Returns a value between -3 and 3 where -3 represents the biggest left error and 3 the biggest right error  0 no error 10 if no line //TODO: rephrase  check if switches react as they should
*/
void get_line_error(void)
{
	if(status.system.start_track == true)
	{
		static int16_t i_factor,last_error,d_factor,p_factor, pid;
		static int8_t error = 0,k=0;
		static bool do_once=true;
		static timer_t sys_tmr;
		read_switches();
		if (++k==20)
		{
			k=0;
			send_state_info();
		}
		switch (led.array)//- line on right +line on left
		{
			case 0b01100011 : //0
			{
				error = 0;
				i_factor = 0;
				toggle_led();
			}
			break;
			case 0b01100111 : //-1
			error = 1;
			break;
			case 0b01110011 : //1
			error = -1;
			break;
			case 0b01000111 : //-2
			error = 2;
			break;
			case 0b01110001 : //2
			error = -2;
			break;
			case 0b01001111 : //-3
			error = 3;
			break;
			case 0b01111001: //3
			error = -3;
			break;
			case 0b00001111: //-4
			error = err4;
			break;
			case 0b01111000: //4
			error = -err4;
			break;
			case 0b00011111: //-5
			error = err5;
			break;
			case 0b01111100: //5
			error = -err5;
			break;
			case 0b00111111: //-6
			error = err6;
			break;
			case 0b01111110: //6
			error = -err6;
			break;
			case 0b00000000:
			{
				switch (sys)
				{
					break;
					case START_TRACK:
					{
						if (do_once)
						{
							do_once=false;
							clear_rpm();
							sys = S_DELAY_1;
							tmr_start(&sys_tmr,350);							
						}
					}
					case FALLOW_TRACK1:
					{
						if (do_once)
						{
							do_once=false;
							clear_rpm();
							sys=DO_CIRCLE;
							do_cirecle();
							status.system.start_line=false;
							status.system.circle = true;
						}
					}
					break;
					case FALLOW_TRACK2:
					{
						if (do_once)
						{
							do_once=false;
							clear_rpm();
							sys = DO_WALL;
							do_wall();
							status.system.start_line=false;
							status.system.start_track=false;
							status.system.wall =true;
						}
					}
					default:
					/* Your code here */
					break;
				}
				
			}
			break;
			case 0b01111111:
			
			break;
			//more cases for the special lines
			default:
			{
			}
			break;
		}
		
		if(led.array!=0)
		{
			switch (sys)
			{
				case IDLE:
				{
					if (do_once)
					{
						do_once=false;
						sys = START_TRACK;
						l_motor.rpm = REF_RPM;
						r_motor.rpm = REF_RPM;
					}
				}
				break;
				case START_TRACK:
				{
					do_once=true;
				}
				break;
				case S_DELAY_1:
				{
					if (tmr_exp(&sys_tmr))
					{
						sys = GET_ON_TRACK;
						do_once=true;
					}
				}
				break;
				case GET_ON_TRACK:
				{
					if (do_once)
					{
						do_once = false;
						set_movement(140,C45,RIGHT);
					}
					else if (movement_finished())
					{
						do_once=true;
						set_m_forward();
						clear_rpm();
						sys = S_DELAY_2;
						tmr_start(&sys_tmr,350);
					}
				}
				break;
				case S_DELAY_2:
				{
					if (tmr_exp(&sys_tmr))
					{
						sys = FALLOW_TRACK1;
						set_m_forward();
						do_once=true;
						status.system.start_line = true;
						l_motor.ref_rpm = REF_RPM;
						l_motor.rpm = REF_RPM;
						r_motor.ref_rpm = REF_RPM;
						r_motor.rpm = REF_RPM;
					}
				}
				break;
				case FALLOW_TRACK1:
				{
					do_once=true;
				}
 				break;
				case DO_CIRCLE:
				{
					do_once=true;
					if (status.system.circle==false)
					{
						sys = S_DELAY_3;
						tmr_start(&sys_tmr,350);
					}
				}
				break;
				case S_DELAY_3:
				{
					do_once=true;					
					if (tmr_exp(&sys_tmr))
					{
						sys = FALLOW_TRACK2;
						set_m_forward();
						do_once=true;
						status.system.start_line = true;
						l_motor.ref_rpm = REF_RPM;
						l_motor.rpm = REF_RPM;
						r_motor.ref_rpm = REF_RPM;
						r_motor.rpm = REF_RPM;
					}
				}
 				break;				
				case FALLOW_TRACK2:
				{
					do_once=true;
				}
				default:
					//do_once=true;
				break;
			}
		}
		
		
		
		
		
		if (status.system.start_line)
		{
			p_factor = error*Kp;
			
// 			i_factor +=error;
// 			if (i_factor>MAX_I_DEVIATION)
// 			{
// 				i_factor=MAX_I_DEVIATION;
// 			}
// 			else if(i_factor<-MAX_I_DEVIATION)
// 			{
// 				i_factor=-MAX_I_DEVIATION;
// 			}

			d_factor =aplie_Kd(error-last_error);
			
			pid=p_factor+aplie_Ki(i_factor)+d_factor;
			
// 			if (pid>MAX_DEVIATION)
// 			{
// 				pid=MAX_DEVIATION;
// 			}
// 			else if (pid<-MAX_DEVIATION)
// 			{
// 				pid= -MAX_DEVIATION;
// 			}
			l_motor.rpm = l_motor.ref_rpm - pid;
			r_motor.rpm = r_motor.ref_rpm + pid;
			if (l_motor.rpm<17)
			{
				l_motor.rpm=17;
		}
		if (r_motor.rpm<17)
		{
			r_motor.rpm=17;
		}
			last_error = error;
			
		}
		static uint8_t info_timer=10;//5*70ms = 350ms
		if(--info_timer==0)
		{
			info_timer=10;
			//send_led_info();
			send_left_m();
			send_right_m();
			//send_pid(pid);
			//task_t led_info  = {.data.command = PID_L_KP, .data.value = p_factor};add_task(&led_info);
			//task_t led_info1 = {.data.command = PID_L_KI, .data.value = i_factor};add_task(&led_info1);
			//task_t led_info2 = {.data.command = PID_L_KD, .data.value = d_factor};add_task(&led_info2);
		}
	}

}

void send_sensor_values(void) {
	read_switches();
	send_led_info();
}

void start_line(task_t *task)
{
	if (task->data.u8[3]>0)
	{
		status.system.start_line=1;
		l_motor.ref_rpm=task->data.u8[3]*2;
		r_motor.ref_rpm=task->data.u8[3]*2;
		l_motor.rpm=task->data.u8[3]*2;
		r_motor.rpm=task->data.u8[3]*2;
	}
	else
	{
		status.system.start_line=0;
		l_motor.ref_rpm=0;
		r_motor.ref_rpm=0;
		l_motor.rpm=0;
		r_motor.rpm=0;
	}
}

void set_l_Kp(task_t *task)
{
	Kp=task->data.u8[3];
	task_t led_info  = {.data.command = PID_L_KP, .data.value = Kp};add_task(&led_info);
}
void start_track(task_t *task)
{
	status.system.start_track = task->data.u8[3];
	if (status.system.start_track)
	{
		sys = IDLE;
		clear_rpm();
		set_m_forward();
	}
	else
	{
		clear_rpm();
		set_m_forward();
		status.system.circle = false;
		status.system.start_line = false;
		status.system.wall=false;
		sys = STOP;
	}
}

void set_l_Ki(task_t *task)
{
	Ki=task->data.u8[3];
	task_t led_info  = {.data.command = PID_L_KI, .data.value = Ki};add_task(&led_info);
}

void set_l_Kd(task_t *task)
{
	Kd=task->data.u8[3];
	task_t led_info  = {.data.command = PID_L_KD, .data.value = Kd};add_task(&led_info);
}

void set_err4(task_t *task)
{
	err4=task->data.u8[3];
	task_t led_info  = {.data.command = PID_ERR4, .data.value = err4};add_task(&led_info);
}

void set_err5(task_t *task)
{
	err5=task->data.u8[3];
	task_t led_info  = {.data.command = PID_ERR5, .data.value = err5};add_task(&led_info);
}

void set_err6(task_t *task)
{
	err6=task->data.u8[3];
	task_t led_info  = {.data.command = PID_ERR6, .data.value = err6};add_task(&led_info);
}

void set_err_p1(task_t *task)
{
	err4++;
	err5++;
	err6++;
	task_t led_info  = {.data.command = PID_ERR4, .data.value = err4};add_task(&led_info);
	task_t led_info1  = {.data.command = PID_ERR5, .data.value = err5};add_task(&led_info1);
	task_t led_info2  = {.data.command = PID_ERR6, .data.value = err6};add_task(&led_info2);
}

void set_pid(task_t *task)
{
	Kp=task->data.u8[0];
	Ki=task->data.u8[1];
	Kd=task->data.u8[2];
	task_t led_info  = {.data.command = PID_L_KP, .data.value = Kp};add_task(&led_info);
	task_t led_info1 = {.data.command = PID_L_KI, .data.value = Ki};add_task(&led_info1);
	task_t led_info2 = {.data.command = PID_L_KD, .data.value = Kd};add_task(&led_info2);
}

/**
* \brief initializes the pins for the light switches
*
* \param
*
* \return void
*/
void led_init(void)
{
	set_pin_as_input(A,0);
	set_pin_as_input(A,1);
	set_pin_as_input(A,2);
	set_pin_as_input(A,3);
	set_pin_as_input(A,4);
	set_pin_as_input(A,5);
	set_pin_as_input(B,0);
	l_motor.ref_rpm=129;
	r_motor.ref_rpm=129;
}