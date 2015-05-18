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
	
	
	#define MAX_RPM 200

typedef struct
{
	uint16_t pulses;
	int16_t error;
	bool direction;
	int16_t rpm;
	int16_t ref_rpm;
} motor_t;

extern motor_t l_motor, r_motor;

void motors_init(void);
void set_left(task_t *task);
void set_right(task_t *task);
void set_forward(task_t *task);
void set_backward(task_t *task);
void set_motors(task_t *task);
void motors_controoler(void);
void drive(uint8_t a, int8_t mag);

#endif /* MOTOR_H_ */