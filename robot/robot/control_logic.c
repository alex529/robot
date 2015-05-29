/*
 * control_logic.c
 *
 * Created: 5/16/2015 3:38:30 PM
 *  Author: Adam Orosz, s134615
 */ 

#include <stdbool.h>
#include "control_logic.h"
#include "task.h"
#include "motor.h"
#include "common.h"
#include "timer.h"
#include "led.h"

#define STATE_FIND_TRACK_SENSOR_BLACKOUT_INTERVAL	2000
#define STATE_WAIT_BEFORE_CORNER_INTERVAL	5000
timer_t state_find_track_sensor_blackout_timer;
timer_t state_wait_before_corner_timer;

volatile state_find_track_data_t state_find_track_data;
volatile state_wait_before_corner_data_t state_wait_before_corner_data;

void state_idle_control_logic() {
	// left bank intentionally 
}

void state_find_track_control_logic() {			
	if (state_find_track_data.not_first_run == false){
		state_find_track_data.not_first_run = true;
		task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_IDLE};
		add_task(&system_state);
		set_m_forward()
		l_motor.rpm = 100;
		r_motor.rpm = 100;
		state_find_track_data.exp == false;
		//can I restart timer without redeclaration
		tmr_start(&state_find_track_sensor_blackout_timer,STATE_FIND_TRACK_SENSOR_BLACKOUT_INTERVAL); 
	}
	
	if (state_find_track_data.exp == true || tmr_exp(&state_find_track_sensor_blackout_timer)){
		state_find_track_data.exp = true;
		read_switches();
		uint8_t sensor_value = led.array;
		if ((sensor_value & 0x7e)!= 0)
		{
			control = & state_follow_track_1_control_logic;	
		}
		return;	
	}
}

void state_follow_track_1_control_logic() {
		task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=2};
		add_task(&system_state);
		set_m_backward()
		l_motor.rpm = 0;
		r_motor.rpm = 0;
}

void state_y_intersection_control_logic() {
	
}

void state_follow_track_2_control_logic() {
	
}

void state_wait_before_corner() {
	if (state_wait_before_corner_data.not_first_run == false){
		state_wait_before_corner_data.not_first_run = true;
		task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_WAIT_BEFORE_CORNER};
		add_task(&system_state);
		set_m_backward()
		l_motor.rpm = 0;
		r_motor.rpm = 0;
		state_wait_before_corner_data.exp == false;
		//can I restart timer without redeclaration
		tmr_start(&state_wait_before_corner_timer,STATE_WAIT_BEFORE_CORNER_INTERVAL);
	}
	
	if (state_wait_before_corner_data.exp == true || tmr_exp(&state_wait_before_corner_data)){
		state_wait_before_corner_data.exp = true;
		set_m_forward()
		l_motor.rpm = 100;
		r_motor.rpm = 100;
		control = &state_approach_corner;
		return;
	}	
}

void state_approach_corner() {
	
}

void state_turn_after_found_corner() {
	
}

void state_go_ahead_after_turn() {
	
}

void state_go_a_bit_more() {
	
}

void state_second_left_turn() {
	
}

void state_second_go_ahead() {
	
}

void state_second_go_a_bit_more() {
	
}

void state_third_left_turn() {
	
}

void state_third_go_ahead() {
	
}

void state_right_turn() {
	
}

void state_last_go_ahead() {
	
}

void state_finish() {
	
}
void state_take_over_control_logic() {
	
}