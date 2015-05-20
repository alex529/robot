/*
 * led.h
 *
 * Created: 24/04/2015 20:45:55
 *  Author: SavinA
 */ 


#ifndef LED_H_
#define LED_H_
#include "task.h"
typedef union
{
	struct
	{
		uint8_t sw0 : 1;
		uint8_t sw1 : 1;
		uint8_t sw2 : 1;
		uint8_t sw3 : 1;
		uint8_t sw4 : 1;
		uint8_t sw5 : 1;
		uint8_t sw6 : 1;
		uint8_t sw7 : 1;
	}switches;
	uint8_t array;
} led_t;


void led_init(void);
void get_line_error(void);
void eval(void);
void start_line(task_t *task);
void set_Kp(task_t *task);
void set_Kd(task_t *task);
void set_Ki(task_t *task);


#endif /* LED_H_ */