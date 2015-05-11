/*
 * led.h
 *
 * Created: 24/04/2015 20:45:55
 *  Author: SavinA
 */ 


#ifndef LED_H_
#define LED_H_

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


#endif /* LED_H_ */