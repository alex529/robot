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

#define set_left_m(x)	{if(x>255){OCR2=255;}else {OCR2 = x;}}
#define set_right_m(x)	{if(x>255){OCR0=255;}else {OCR0 = x;}}
#define get_left_m()	(OCR2)
#define get_right_m()	(OCR0)
#define mot_right_forw	PC6
#define mot_right_back	PC7
#define mot_left_forw	PB4
#define mot_left_back	PB1
#define mot_left_port	PORTB 
#define mot_right_port	PORTC

#define set_lf()	{mot_left_port	|=(1<<mot_left_forw); mot_left_port &=~(1<<mot_left_back);l_motor.m_dir = FORWARD;l_motor.breaking = OFF;}
#define set_lb(){mot_left_port	|=(1<<mot_left_back); mot_left_port &=~(1<<mot_left_forw);l_motor.m_dir = BACKWARD;l_motor.breaking = OFF;}
#define break_l(break_val){if(l_motor.m_dir == FORWARD){mot_left_port	|=(1<<mot_left_back); mot_left_port &=~(1<<mot_left_forw);}else{mot_left_port	|=(1<<mot_left_forw); mot_left_port &=~(1<<mot_left_back);}l_motor.breaking = ON;	set_left_m(break_val);}
#define set_ls()	{mot_left_port	|=(1<<mot_left_back); mot_left_port |=(1<<mot_left_forw);l_motor.breaking = OFF;set_left_m(0);}

#define set_rf()	{mot_right_port	|=(1<<mot_right_forw);mot_right_port&=~(1<<mot_right_back);r_motor.m_dir = FORWARD;r_motor.breaking = OFF;}
#define set_rb(){mot_right_port	|=(1<<mot_right_back);mot_right_port&=~(1<<mot_right_forw);r_motor.m_dir = BACKWARD;r_motor.breaking = OFF;}
#define break_r(break_val){if(r_motor.m_dir == FORWARD){mot_right_port	|=(1<<mot_right_back);mot_right_port&=~(1<<mot_right_forw);}else{mot_right_port	|=(1<<mot_right_forw);mot_right_port&=~(1<<mot_right_back);}r_motor.breaking = ON; set_right_m(break_val);}
#define set_rs()	{mot_right_port	|=(1<<mot_right_forw);mot_right_port|=(1<<mot_right_back);r_motor.breaking = OFF;set_right_m(0);}

#define set_l_m_forward() {set_lf()	;l_motor.r_dir = FORWARD;}
#define set_l_m_backward(){set_lb()	;l_motor.r_dir = BACKWARD;}
#define set_r_m_forward() {set_rf()	;r_motor.r_dir = FORWARD;}
#define set_r_m_backward(){set_rb()	;r_motor.r_dir = BACKWARD;}
#define set_m_forward()	  {set_l_m_forward() ;set_r_m_forward();}
#define set_m_backward()  {set_l_m_backward();set_r_m_backward();}	
	
#define stoop()			{set_left_m(0); set_right_m(0); mot_left_port &=~(1<<mot_left_back);mot_left_port &=~(1<<mot_left_forw);mot_right_port &=~(1<<mot_right_back);mot_right_port &=~(1<<mot_right_forw);}

#define MAX_RPM 200

typedef enum
{
	BACKWARD = -1,
	FORWARD = 1,
	LEFT = 2,
	RIGHT = 3,			
} direction_t;

typedef enum
{
	ON=0,						//0x00
	OFF=1,						//0x01
} break_t;

typedef enum
{
	C0=0,						//0x01
	C45=180,						//0x01
	C90=380,						//0x00
	CIRCLE_RADIUS = 1610,			//0x00
	CIRCLE_CIRCUMFERENCE = 9523,
} corner_t;

typedef struct
{
	int8_t ref_pulses;
	uint16_t pulse_count;
	corner_t corner;
	int16_t error;
	break_t breaking;
	uint8_t break_count;
	direction_t r_dir;
	direction_t m_dir;
	int16_t rpm;
} motor_t;

extern volatile motor_t l_motor, r_motor;

void motors_init(void);
void set_rpm(task_t *task);
void set_corner_task(task_t *task);
void motor_handler(void);
void set_corner(int16_t rpm, corner_t corner, direction_t d);
void start_circle(task_t *task);
void set_circle_time(task_t *task);
void do_cirecle(void);

#endif /* MOTOR_H_ */