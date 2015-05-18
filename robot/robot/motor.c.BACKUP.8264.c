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
#define toggle_edges()			{int0_toggle_edge();int1_toggle_edge();}
#define eneable_external_int()	{GICR |= (1 << INT0) | (1 << INT1);}

#define set_left_m(x)	{OCR2 = x;}
#define set_right_m(x)	{OCR0 = x;}
#define get_left_m()	(OCR2)
#define get_right_m()	(OCR0)
#define mot_right_forw	PC6
#define mot_right_back	PC7
#define mot_left_forw	PB4
#define mot_left_back	PB1
#define mot_left_port	PORTB
#define mot_right_port	PORTC

#define set_l_forward()	{mot_left_port	|=(1<<mot_left_forw); mot_left_port &=~(1<<mot_left_back);}
#define set_l_backward(){mot_left_port	|=(1<<mot_left_back); mot_left_port &=~(1<<mot_left_forw);}
#define set_m_forward()	{set_l_forward() ;set_r_forward() ;status.system.motor_forward =true;}
#define set_m_backward(){set_l_backward();set_r_backward();status.system.motor_forward =false;}
#define set_r_forward()	{mot_right_port	|=(1<<mot_right_forw);mot_right_port&=~(1<<mot_right_back);}
#define set_r_backward(){mot_right_port	|=(1<<mot_right_back);mot_right_port&=~(1<<mot_right_forw);}
#define set_l_stop()	{set_left_m(0)	;mot_left_port	|=(1<<mot_left_back); mot_left_port |=(1<<mot_left_forw);}
#define set_r_stop()	{set_right_m(0)	;mot_right_port	|=(1<<mot_right_forw);mot_right_port|=(1<<mot_right_back);}

#define is_in_bounds(x) (x<255&&x>>-255)
#define ANGLE 248

<<<<<<< HEAD
#define send_left_m(x) {task_t m_info = {.data.command = MOTOR_L, .data.value = get_left_m()};add_task(&m_info);}
#define send_right_m(x){task_t m_info = {.data.command = MOTOR_R, .data.value = get_right_m()};add_task(&m_info);}
=======
static const uint16_t rpm_speed[30]={15,29,44,58,73,87,102,116,131,145,160,174,189,203,218,233,247,262,276,291,305,320,334,349,363,378,392,407,422};
>>>>>>> test

motor_t l_motor, r_motor;

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


void motors_controoler(void)
{
	
	if (l_motor.rpm>MAX_RPM)
	{
		l_motor.rpm=MAX_RPM;
	}
	if (r_motor.rpm>MAX_RPM)
	{
		r_motor.rpm=MAX_RPM;
	}
	int16_t l_ref_pulses = l_motor.rpm>>4;
	int16_t r_ref_pulses = r_motor.rpm>>4;
	
	l_motor.error = l_ref_pulses-l_motor.pulses;
	r_motor.error = r_ref_pulses-r_motor.pulses;
	
	int16_t motor = get_left_m()+((Kp*l_motor.error)>>7);
	if (motor<0||l_motor.error<0)
	{
		if (status.system.motor_forward == true)
		{
			set_l_backward();
		}
		else
		{
			set_l_forward();
		}
		motor = 20;
	}
	else if(motor>255)
	{
		motor = 255;
	}
	else
	{
		if (status.system.motor_forward == true)
		{
			set_l_forward();
		}
		else
		{
			set_l_backward();
		}
	}
	set_left_m(motor);

	motor = get_right_m()+((Kp*r_motor.error)>>7);
	if (motor<0||r_motor.error<0)
	{
		if (status.system.motor_forward == true)
		{
			set_r_backward();
		}
		else
		{
			set_r_forward();
		}
		motor = 19;
	} else if(motor>255)
	{
		motor = 255;
	}
	else
	{
		if (status.system.motor_forward == true)
		{
			set_r_forward();
		}
		else
		{
			set_r_backward();
		}
	}
	set_right_m(motor);
}

void drive(uint8_t a, int8_t mag)
{
<<<<<<< HEAD
	int16_t l_ref = 0, r_ref = 0;
	mag = mag<<1;
	x=x<<1;
	y=y<<1;
	if(x>0&&y>0)
	{
		r_ref = x+y;
		l_ref = mag-x-y;
		set_r_forward();
		set_l_forward();
	}
	else if(x<0&&y>0)
	{
		r_ref = mag-y+x;
		l_ref = y+int16_abs_Q(x);
		set_r_forward();
		set_l_forward();
	}
	else if(x<0&&y<0)
=======
	int16_t l_ref, r_ref;
	if (mag>0)
>>>>>>> test
	{
		r_ref = a;
		l_ref = ANGLE-a;
		set_m_forward();
	}
	else if (x == 0&&y==0)//TODO more advanced stopping procedure is needed
	{
		set_l_stop();
		set_r_stop();		
	}
	else
	{
		l_ref = a;
		r_ref = ANGLE-a;
		set_m_backward();
	}
<<<<<<< HEAD
	set_left_m(l_ref);
	set_right_m(r_ref);
	
	send_left_m();
	send_left_m();
	
=======
	
	mag = int8_abs_Q(mag);
	l_motor.rpm=(((l_ref*mag)/128));
	r_motor.rpm=(((r_ref*mag)/128));
	
	task_t motor = {.data.command = MOTOR_ANGLE, .data.value = a};
	add_task(&motor);
	task_t motor1 = {.data.command = MOTOR_MAGNITUDE, .data.value = mag};
	add_task(&motor1);
	task_t motor2 = {.data.command = MOTOR_L, .data.value = l_motor.rpm};
	add_task(&motor2);
	task_t motor3 = {.data.command = MOTOR_R, .data.value = r_motor.rpm};
	add_task(&motor3);
>>>>>>> test
}

void set_left(task_t *task)
{
	set_left_m(task->data.value);
}

void set_right(task_t *task)
{
	set_right_m(task->data.value);
}

void set_forward(task_t *task)
{
	set_l_forward();
	set_r_forward();
}

void set_backward(task_t *task)
{
	set_l_backward();
	set_r_backward();
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
	set_l_forward();
	set_r_forward();
<<<<<<< HEAD
	l_motor.rpm=0;
	r_motor.rpm=0;
	/*
	l_motor.rpm=MAX_RPM;
	r_motor.rpm=MAX_RPM;
	*/
// 	l_motor.ref_rpm=190;
// 	r_motor.ref_rpm=200;
}

void set_speed(task_t *task) {
	uint8_t speed = task->data.u8[0];
=======
	l_motor.ref_rpm=100;
	r_motor.ref_rpm=100;
>>>>>>> test
}