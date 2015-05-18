/*
 * take_over.c
 *
 * Created: 5/15/2015 2:31:41 PM
 *  Author: Adam Orosz, s134615
 */ 

#include <stdlib.h>
#include "task.h"
#include "common.h"
#include "take_over.h"
#include "state_machine.h"
#include "state_machine_event_buffer.h"
#include "adc.h"

void take_over_command(task_t *task) {
	enable_adc();
	enable_features.send_adc_value = true;
	drive(0,0,0);
	state = STATE_TAKE_OVER;
	//turn off controller
	task_t confirm_take_over_task = {.data.command = TAKE_OVER_ACK, .data.timestamp=0, .data.value=0};
	add_task(&confirm_take_over_task);
	task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_TAKE_OVER};
	add_task(&system_state);
}

void give_back_control_command(task_t *task) {
	disable_adc();
	uint8_t event = task->data.u8[0];
	add_event(event);
	task_t confirm_give_back_control_task = {.data.command = GIVE_BACK_CONTROL_ACK, .data.timestamp=0, .data.value=event};
	add_task(&confirm_give_back_control_task);
}