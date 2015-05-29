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
#include "adc.h"
#include "motor.h"
#include "control_logic.h"

void take_over_command(task_t *task) {
	enable_features.adc = true;
	enable_features.controller = false;
	enable_features.send_adc_value = true;
	control = & state_take_over_control_logic;
	l_motor.rpm=0;
	r_motor.rpm=0;
	task_t confirm_take_over_task = {.data.command = TAKE_OVER_ACK, .data.timestamp=0, .data.value=0};
	add_task(&confirm_take_over_task);
	task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_TAKE_OVER};
	add_task(&system_state);
}

void give_back_control_command(task_t *task) {
	enable_features.adc = false;
	enable_features.send_adc_value = false;
	uint8_t state = task->data.u8[0];
	switch(state) {
		case 0:
			control = &state_idle_control_logic;
			break;
		case 1:
			control = &state_find_track_control_logic;
			break;
		case 2:
			control = &state_follow_track_1_control_logic;
			break;
		case 3:
			control = &state_y_intersection_control_logic;
			break;
		case 4:
			control = &state_follow_track_2_control_logic;
			break;
		default:
			control = &state_take_over_control_logic;
			break;
	}
}