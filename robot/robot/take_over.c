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
			state_find_track_data.not_first_run=false;
			state_find_track_data.exp=false;
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
		case 5:
			control = &state_wait_before_corner;
			state_wait_before_corner_data.not_first_run=false;
			state_wait_before_corner_data.exp=false;
			break;
		case 6:
			control = &state_approach_corner;
			break;
		case 7:
			control = &state_turn_after_found_corner;
			break;
		case 8:
			control = &state_go_ahead_after_turn;
			break;
		case 9:
			control = &state_go_a_bit_more;
			break;
		case 10:
			control = &state_second_left_turn;
			break;
		case 11:
			control = &state_second_go_ahead;
			break;
		case 12:
			control = &state_second_go_a_bit_more;
			break;
		case 13:
			control = &state_third_left_turn;
			break;
		case 14:
			control = &state_third_go_ahead;
			break;
		case 15:
			control = &state_right_turn;
			break;
		case 16:
			control = &state_last_go_ahead;
			break;
		case 17:
			control = &state_finish;
			break;
		default:
			control = &state_take_over_control_logic;
			break;
	}
}