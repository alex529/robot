/*
* motor.c
*
* Created: 21/04/2015 19:37:11
*  Author: SavinA
*/

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

#define set_left_m(x)	{OCR0 = x;}
#define set_right_m(x)	{OCR2 = x;}
#define get_left_m()	(OCR0)
#define get_right_m()	(OCR2)
#define mot_left_forw	PC6
#define mot_left_back	PC7
#define mot_right_forw	PB4
#define mot_right_back	PB1
#define mot_right_port	PORTB
#define mot_left_port	PORTC

#define set_l_backward()	{mot_left_port	|=(1<<mot_left_forw); mot_left_port &=~(1<<mot_left_back);}
#define set_l_forward()		{mot_left_port	|=(1<<mot_left_back); mot_left_port &=~(1<<mot_left_forw);}
#define set_r_backward()	{mot_right_port	|=(1<<mot_right_forw);mot_right_port&=~(1<<mot_right_back);}
#define set_r_forward()		{mot_right_port	|=(1<<mot_right_back);mot_right_port&=~(1<<mot_right_forw);}
	
#define is_in_bounds(x) (x<255&&x>>-255)
#define MAX_RPS 6

/**
 * \brief Kp for the motor controller
 * the actual coefisient has to be multiplied by 128 and an integer value has to be provided
 */
#define Kp 115

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
	uint16_t l_ref_pulses = l_motor.rps<<2;//some error is multiplied by 256 insted of 258(=17.2*15) which generates a 0.77% error
	uint16_t r_ref_pulses = r_motor.rps<<2;
	
	l_motor.error = l_ref_pulses-l_motor.pulses;
	r_motor.error = r_ref_pulses-r_motor.pulses;
	
	int16_t motor = get_left_m()+((Kp*l_motor.error)>>7);
	if (motor<0)
	{
		motor = 0;
	} else if(motor>255)
	{
		motor = 255;
	}
	set_left_m(motor);
	motor = get_right_m()+((Kp*r_motor.error)>>7);
	if (motor<0)
	{
		motor = 0;
	} else if(motor>255)
	{
		motor = 255;
	}
	set_left_m(motor);
	set_right_m(motor);

}

void drive(int8_t x,int8_t y, uint8_t mag)
{
	int16_t l_ref, r_ref;
	mag = mag<<1;
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
	{
		r_ref = mag+x+y;
		l_ref = int16_abs_Q(x+y);
		set_r_backward();
		set_l_backward();
	}
	else 
	{
		r_ref = x+int16_abs_Q(y);
		l_ref = mag-x+y;
		set_r_backward();
		set_l_backward();
	}

	l_motor.rps=(l_ref*MAX_RPS)>>7;
	r_motor.rps=(r_ref*MAX_RPS)>>7;
}

void set_left(task_t *task)
{
	set_left_m(task->data.value);
}

void set_right(task_t *task)
{
	set_left_m(task->data.value);
}

void set_motors(task_t *task)
{
	u32_union temp;
	temp.dw=task->data.value;
	drive(temp.b[2],temp.b[3],temp.b[1]);
}

void init_motors(void)
{
	init_ext_int();
	init_pwm();
	set_l_forward();
	set_r_forward();
}