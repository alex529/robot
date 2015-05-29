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

#define set_l_forward()	{mot_left_port	|=(1<<mot_left_forw); mot_left_port &=~(1<<mot_left_back);l_motor.direction = FORWARD;l_motor.breaking = OFF;}
#define set_l_backward(){mot_left_port	|=(1<<mot_left_back); mot_left_port &=~(1<<mot_left_forw);l_motor.direction = BACKWARD;l_motor.breaking = OFF;}
#define break_l(break_val){if(l_motor.direction == FORWARD){mot_left_port	|=(1<<mot_left_back); mot_left_port &=~(1<<mot_left_forw);}else{mot_left_port	|=(1<<mot_left_forw); mot_left_port &=~(1<<mot_left_back);}l_motor.breaking = ON;	set_left_m(break_val);}
#define set_l_stop()	{	;mot_left_port	|=(1<<mot_left_back); mot_left_port |=(1<<mot_left_forw);l_motor.breaking = OFF;}

#define set_r_forward()	{mot_right_port	|=(1<<mot_right_forw);mot_right_port&=~(1<<mot_right_back);r_motor.direction = FORWARD;r_motor.breaking = OFF;}
#define set_r_backward(){mot_right_port	|=(1<<mot_right_back);mot_right_port&=~(1<<mot_right_forw);r_motor.direction = BACKWARD;r_motor.breaking = OFF;}
#define break_r(break_val){if(r_motor.direction == FORWARD){mot_right_port	|=(1<<mot_right_back);mot_right_port&=~(1<<mot_right_forw);}else{mot_right_port	|=(1<<mot_right_forw);mot_right_port&=~(1<<mot_right_back);}r_motor.breaking = ON; set_right_m(break_val);}
#define set_r_stop()	{	;mot_right_port	|=(1<<mot_right_forw);mot_right_port|=(1<<mot_right_back);r_motor.breaking = OFF;}

#define set_m_forward()	{set_l_forward() ;set_r_forward() ;l_motor.direction = FORWARD;r_motor.direction = FORWARD;}
#define set_m_backward(){set_l_backward();set_r_backward();l_motor.direction = BACKWARD;r_motor.direction = BACKWARD;}	
#define stoop()			{set_left_m(0); set_right_m(0); mot_left_port &=~(1<<mot_left_back);mot_left_port &=~(1<<mot_left_forw);mot_right_port &=~(1<<mot_right_back);mot_right_port &=~(1<<mot_right_forw);}

#define MAX_RPM 200

typedef enum
{
	FORWARD=0,						//0x00
	BACKWARD=1,						//0x01
} direction_t;

typedef enum
{
	ON=0,						//0x00
	OFF=1,						//0x01
} break_t;

typedef struct
{
	int8_t ref_pulses;
	int16_t error;
	break_t breaking;
	direction_t direction;
	int16_t rpm;
} motor_t;

extern volatile motor_t l_motor, r_motor;

void motors_init(void);
void set_left(task_t *task);
void set_right(task_t *task);
void set_forward(task_t *task);
void set_backward(task_t *task);
void set_motors(task_t *task);
void set_rpm(task_t *task);
void drive(uint8_t a, int8_t mag);
void motor_handler(void);

#endif /* MOTOR_H_ */