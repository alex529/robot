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
#define Kp 256

#include <avr/io.h>
#include "motor.h"
#include "common.h"
#include "task.h"
#include "math_Q.h"


#define set_int1_to_rising()	{MCUCR |= (1 << ISC11) | (1 << ISC10);}
#define set_int1_to_falling()	{MCUCR &=  ~(1 << ISC10);}
#define set_int0_to_rising()	{MCUCR |= (1 << ISC01) | (1 << ISC00);}
#define set_int0_to_falling()	{MCUCR &=  ~(1 << ISC00);}
#define eneable_external_int()	{GICR |= (1 << INT0) | (1 << INT1);}

#define is_in_bounds(x) (x<255&&x>>-255)
#define ANGLE 248

#define send_left_m(x) {task_t m_info = {.data.command = MOTOR_L, .data.value = get_left_m()};add_task(&m_info);}
#define send_right_m(x){task_t m_info = {.data.command = MOTOR_R, .data.value = get_right_m()};add_task(&m_info);}

static const uint16_t rpm_speed[30]={15,29,44,58,73,87,102,116,131,145,160,174,189,203,218,233,247,262,276,291,305,320,334,349,363,378,392,407,422};

volatile motor_t l_motor, r_motor;
static volatile bool motor_speed_change = false;

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

void drive(uint8_t a, int8_t mag)
{
	int16_t l_ref, r_ref;
	if (mag>0)
	{
		r_ref = a;
		l_ref = ANGLE-a;
		set_m_forward();
	}
	else
	{
		l_ref = a;
		r_ref = ANGLE-a;
		set_m_backward();
	}	
	mag = int8_abs_Q(mag);
	l_motor.rpm=(((l_ref*mag)/128));
	r_motor.rpm=(((r_ref*mag)/128));
}


void motor_handler(void)
{
	l_motor.ref_pulses = 8;
	r_motor.ref_pulses = 6;
// 	static int16_t last_l_rpm=0,last_r_rpm=0;
// 	if(last_l_rpm!=l_motor.rpm)
// 	{		
// 		if (l_motor.rpm>MAX_RPM)
// 		{
// 			l_motor.rpm = MAX_RPM;
// 		}
// 		l_motor.ref_pulses = l_motor.rpm / 8;
// 	}
// 	if (last_r_rpm!=r_motor.rpm)
// 	{
// 		if (r_motor.rpm>MAX_RPM)
// 		{
// 			r_motor.rpm = MAX_RPM;
// 		}
// 		r_motor.ref_pulses = r_motor.rpm / 8;
// 	}
// 	last_l_rpm = l_motor.rpm;
// 	last_r_rpm = r_motor.rpm;
}


void set_left(task_t *task)
{
	set_left_m(task->data.value);
}

void set_right(task_t *task)
{
	set_right_m(task->data.value);
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

void set_forward(task_t *task)
{
	set_lf();
	set_rf();
}

void set_backward(task_t *task)
{
	set_lb();
	set_rb();
}

void set_motors(task_t *task)
{
	u32_union temp;
	temp.dw=task->data.value;
	drive(temp.b[3],temp.b[2]);
}

void motors_init(void)
{
	init_ext_int();
	init_pwm();
	set_l_m_forward()
	set_r_m_forward()
	//l_motor.rpm=70;
	r_motor.rpm=70;
}