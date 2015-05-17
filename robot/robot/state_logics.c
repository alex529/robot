/*
 * state_logics.c
 *
 * Created: 5/17/2015 1:51:49 PM
 *  Author: Adam Orosz, s134615
 */ 

#include "state_logics.h"
#include "state_machine.h"
#include "task.h"

void enter_state_idle() {
	//set control logic
	state = STATE_IDLE;
	task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_IDLE};
	add_task(&system_state);
}

void enter_state_find_track() {
	//set control logic
	state = STATE_FIND_TRACK;
	task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_FIND_TRACK};
	add_task(&system_state);
}

void enter_state_follow_track_1() {
	//set control logic
	state = STATE_FOLLOW_TRACK_1;
	task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_FOLLOW_TRACK_1};
	add_task(&system_state);
}

void enter_state_y_intersection() {
	//set control logic
	state = STATE_Y_INTERSECTION;
	task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_Y_INTERSECTION};
	add_task(&system_state);
}

void enter_state_follow_track_2() {
	//set control logic
	state = STATE_FOLLOW_TRACK_2;
	task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_FOLLOW_TRACK_2};
	add_task(&system_state);
}