/*
* motor.c
*
* Created: 21/04/2015 19:37:11
*  Author: SavinA
*/

/************************************************************************/
/* change Kp value                                                                     */
/************************************************************************/

/**
* \brief Kp for the motor controller
* the actual coefisient has to be multiplied by 128 and an integer value has to be provided
*/

#include <avr/io.h>
#include "motor.h"
#include "common.h"
#include "task.h"
#include "math_Q.h"
#include "timer.h"


#define set_int1_to_rising()	{MCUCR |= (1 << ISC11) | (1 << ISC10);}
#define set_int1_to_falling()	{MCUCR &=  ~(1 << ISC10);}
#define set_int0_to_rising()	{MCUCR |= (1 << ISC01) | (1 << ISC00);}
#define set_int0_to_falling()	{MCUCR &=  ~(1 << ISC00);}
#define eneable_external_int()	{GICR |= (1 << INT0) | (1 << INT1);}

#define is_in_bounds(x) (x<255&&x>>-255)
#define ANGLE 248

#define send_left_m(x) {task_t m_info = {.data.command = MOTOR_L, .data.value = get_left_m()};add_task(&m_info);}
#define send_right_m(x){task_t m_info = {.data.command = MOTOR_R, .data.value = get_right_m()};add_task(&m_info);}

#define circle_delay(time,next_state){if (do_once){do_once=false;tmr_start(&delay,time);}if (tmr_exp(&delay)){c_state = next_state;do_once=true;}}

static const uint16_t rpm_speed[30]={15,29,44,58,73,87,102,116,131,145,160,174,189,203,218,233,247,262,276,291,305,320,334,349,363,378,392,407,422};

volatile motor_t l_motor, r_motor;
static volatile bool motor_speed_change = false;


typedef enum
{
	PRE_DELAY,						//0x00
	FORWARD_RADIUS,						//0x00
	FIRST_DELAY,
	FIRST_CORNER,						//0x00
	SECOND_DELAY,
	CIRCLE,
	THIRD_DELAY,
	SECOND_CORNR,
	FORTH_DELAY,
	REINIT,
} circle_state;





/**
* \brief
*
* \param
*
* \return void
*/
void init_ext_int(void)
{
	set_int1_to_rising();
	set_int0_to_rising();
	eneable_external_int();
}

void init_pwm(void){

	TCCR0 |= (1 << WGM00)|(1 << COM01)|(1 << WGM01)|(1 << CS00); //fast pwm ,64 prescalar, 8 bit
	TCCR2 |= (1 << WGM20)|(1 << WGM21)|(1 << COM21)|(1 << CS20); //fast pwm, 64 prescalar, 8 bit
	
	DDRB |= (1 << PB3);
	DDRD |= (1 << PD7);
}

void check_corner(volatile motor_t* motor)
{
	if (motor->corner!=C0)
	{
		switch (motor->corner)
		{
			case C90:
			{
				if (motor->pulse_count>C90)
				{
					motor->rpm = 0;
					motor->corner = C0;
				}
			}
			break;
			case C45:
			{
				if (motor->pulse_count>C45)
				{
					motor->rpm = 0;
					motor->corner = C0;
				}
			}
			break;
			case CIRCLE_RADIUS:
			{
				if (motor->pulse_count>CIRCLE_RADIUS)
				{
					motor->rpm = 0;
					motor->corner = C0;
				}
			}
			break;
			default:
			{
				motor->rpm = 0;
				motor->corner = C0;
			}
			break;
		}
	}
}

void set_corner(int16_t rpm, corner_t corner, direction_t d)
{
	switch (d)
	{
		case LEFT:
		{
			set_l_m_backward();
			set_r_m_forward();
		}
		break;
		case RIGHT:
		{
			set_l_m_forward();
			set_r_m_backward();
		}
		break;
		case FORWARD:
		{
			set_l_m_forward();
			set_r_m_forward();
		}
		break;
		case BACKWARD:
		{
			set_l_m_backward();
			set_r_m_backward();
		}
		break;
		default:
		{
			set_l_m_forward();
			set_r_m_forward();
		}
		break;
	}
	l_motor.rpm = rpm;
	r_motor.rpm = rpm;
	l_motor.corner = corner;//TODO maybe close interupts for all the assignments
	r_motor.corner = corner;
	r_motor.pulse_count=0;
	l_motor.pulse_count=0;
}
void set_corner_task(task_t *task)
{
	static corner_t temp_corner= C0;
	switch (task->data.u8[2])
	{
		case 0:
		temp_corner = C0;
		break;
		case 1:
		temp_corner = C45;
		break;
		case 2:
		temp_corner = C90;
		break;
		default:
		temp_corner = C0;
		break;
	}
	set_corner(task->data.u8[3],temp_corner,task->data.u8[1]);
}

void motor_handler(void)
{
	static int16_t last_l_rpm=0,last_r_rpm=0;
	
	if(last_l_rpm!=l_motor.rpm)
	{
		if (l_motor.rpm>MAX_RPM)
		{
			l_motor.rpm = MAX_RPM;
		}
		l_motor.ref_pulses = l_motor.rpm / 16;
		last_l_rpm = l_motor.rpm;
	}
	if (last_r_rpm!=r_motor.rpm)
	{
		if (r_motor.rpm>MAX_RPM)
		{
			r_motor.rpm = MAX_RPM;
		}
		r_motor.ref_pulses = r_motor.rpm / 16;
		last_r_rpm = r_motor.rpm;
	}
	check_corner(&l_motor);
	check_corner(&r_motor);
	if (status.system.not_used4 == true)
	{
		do_cirecle();
	}
}


void set_rpm(task_t *task)
{
	u32_union temp;
	temp.dw = task->data.value;
	int16_t l = (int16_t)temp.w[1],r = (int16_t)temp.w[0];
	
	if (l>=0)
	{
		set_m_forward()
		l_motor.rpm = l;
		r_motor.rpm = r;
	}
	else
	{
		set_m_backward()
		l_motor.rpm = int16_abs_Q(l);
		r_motor.rpm = int16_abs_Q(r);
	}
	task_t motor2 = {.data.command = MOTOR_L, .data.value = l_motor.rpm};
	add_task(&motor2);
	task_t motor3 = {.data.command = MOTOR_R, .data.value = r_motor.rpm};
	add_task(&motor3);
}
uint16_t circle_time = 13500;
void do_cirecle(void)
{
	// 	TODO: create a state so nobody fucks with the robot
	static circle_state c_state = PRE_DELAY;
	static bool do_once = true;
	static timer_t delay;
	switch (c_state)
	{
		case PRE_DELAY:
		{
			circle_delay(500,FORWARD_RADIUS);
		}
		break;
		case FORWARD_RADIUS:
		{
			if(do_once)
			{
				do_once=false;
				set_corner(100,CIRCLE_RADIUS,FORWARD);
			}
			if (l_motor.corner == C0&&r_motor.corner==C0)
			{
				do_once = true;
				c_state=FIRST_DELAY;
			}
			
		}
		case FIRST_DELAY:
		{
			circle_delay(500,FIRST_CORNER);
		}
		break;
		case FIRST_CORNER:
		{
			if (do_once)
			{
				do_once = false;
				set_corner(50,C90,LEFT);
			}
			if (l_motor.corner == C0&&r_motor.corner==C0)
			{
				do_once=true;
				c_state=SECOND_DELAY;
			}
		}
		break;
		case SECOND_DELAY:
		{
			circle_delay(500,CIRCLE);
		}
		break;
		case CIRCLE:
		{
			if (do_once)
			{
				set_l_m_forward();
				set_r_m_forward();
				l_motor.rpm = 6*16+1;
				r_motor.rpm = 8*16+1;
				do_once=false;
				tmr_start(&delay,circle_time);
			}
			if (tmr_exp(&delay))
			{
				l_motor.rpm = 0;
				r_motor.rpm = 0;
				c_state = THIRD_DELAY;
				do_once=true;
			}
		}
		break;
		case THIRD_DELAY:
		{
			circle_delay(500,SECOND_CORNR);
		}
		break;
		case SECOND_CORNR:
		{
			if(do_once)
			{
				do_once=false;
				set_corner(50,C90,RIGHT);
			}
			if (l_motor.corner == C0&&r_motor.corner==C0)
			{
				do_once=true;
				c_state=FORTH_DELAY;
			}
		}
		break;
		case FORTH_DELAY:
		{
			circle_delay(500,REINIT)
		}
		break;
		default:
		status.system.not_used4 =false;
		c_state = PRE_DELAY;
		break;
	}
}


void start_circle(task_t *task)
{
	do_cirecle();
	status.system.not_used4 =true;
}

void set_circle_time(task_t *task)
{
	circle_time = task->data.u8[3]*100;
}


void motors_init(void)
{
	init_ext_int();
	init_pwm();
	set_l_m_forward()
	set_r_m_forward()
	l_motor.rpm=0;
	r_motor.rpm=0;
}