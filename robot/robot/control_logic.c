/*
 * control_logic.c
 *
 * Created: 5/16/2015 3:38:30 PM
 *  Author: Adam Orosz, s134615
 */ 
#include "control_logic.h"
#include "task.h"

void state_idle_control_logic() {
	task_t system_state = {.data.command = DEBUG11, .data.timestamp=0, .data.value=1};
	add_task(&system_state);
}

void state_find_track_control_logic() {
	task_t system_state = {.data.command = DEBUG11, .data.timestamp=0, .data.value=2};
	add_task(&system_state);
}

void state_follow_track_1_control_logic() {
	
}

void state_y_intersection_control_logic() {
	
}

void state_follow_track_2_control_logic() {
	
}

void state_take_over_control_logic() {
	
}