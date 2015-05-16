/*
 * switch_value_eval.c
 *
 * Created: 5/16/2015 4:13:42 PM
 *  Author: Adam Orosz, s134615
 */ 
#include "sensor_value_eval.h"
#include "state_machine_event_buffer.h"
#include "state_machine.h"
#include "led.h"

#define ALL_SENSORS_BLACK 0 //TODO pt real value

//how to access 

void eval() {
	uint8_t sensor_value = 0; //TODO put real value
	task_t sensor_value_task = {.data.command = SENSOR, .data.timestamp=0, .data.value = sensor_value};
	add_task(sensor_value_task);
	if (sensor_value == ALL_SENSORS_BLACK)
	{
		 add_event(EVENT_ALL_SENSORS_BLACK);
	}
	/*
	if (sensor_value == )
	{
	}
	*/
}