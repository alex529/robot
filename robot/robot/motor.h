/*
* motor.h
*
* Created: 21/04/2015 19:37:23
*  Author: SavinA
*/


#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "task.h"

#define int1_toggle_edge()		{MCUCR ^=  (1 << ISC10);}

#define int0_toggle_edge()		{MCUCR ^=  (1 << ISC00);}

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
#define set_r_forward()	{mot_right_port	|=(1<<mot_right_forw);mot_right_port&=~(1<<mot_right_back);}
#define set_r_backward(){mot_right_port	|=(1<<mot_right_back);mot_right_port&=~(1<<mot_right_forw);}
#define set_m_forward()	{set_l_forward() ;set_r_forward() ;status.system.motor_forward =true;}
#define set_m_backward(){set_l_backward();set_r_backward();status.system.motor_forward =false;}
#define set_l_stop()	{set_left_m(0)	;mot_left_port	|=(1<<mot_left_back); mot_left_port |=(1<<mot_left_forw);}
#define set_r_stop()	{set_right_m(0)	;mot_right_port	|=(1<<mot_right_forw);mot_right_port|=(1<<mot_right_back);}
#define stoop()			{set_left_m(0); set_right_m(0); mot_left_port &=~(1<<mot_left_back);mot_left_port &=~(1<<mot_left_forw);mot_right_port &=~(1<<mot_right_back);mot_right_port &=~(1<<mot_right_forw);}

	
	#define MAX_RPM 200u

typedef struct
{
	uint8_t pulses;
	int16_t error;
	bool direction;
	int16_t rpm;
	int16_t ref_rpm;
} motor_t;

extern volatile motor_t l_motor, r_motor;

void motors_init(void);
void set_left(task_t *task);
void set_right(task_t *task);
void set_forward(task_t *task);
void set_backward(task_t *task);
void set_motors(task_t *task);
void set_rpm(task_t *task);
void motors_controoler(void);
void drive(uint8_t a, int8_t mag);

#endif /* MOTOR_H_ */