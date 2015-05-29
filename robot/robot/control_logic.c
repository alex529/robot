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
#define STATE_APPROACH_CORNER_INTERVAL 5000 // to be deleted
#define STATE_TURN_AFTER_FOUND_CORNER_TURNING_INTERVAL	500

timer_t state_find_track_sensor_blackout_timer;
timer_t state_wait_before_corner_timer;
timer_t state_approach_corner_timer; // to be deleted
timer_t state_turn_after_found_corner_timer;

volatile state_find_track_data_t state_find_track_data;
volatile state_wait_before_corner_data_t state_wait_before_corner_data;
volatile state_approach_corner_data_t state_approach_corner_data;
volatile state_turn_after_found_corner_data_t state_turn_after_found_corner_data;
volatile state_go_ahead_after_turn_data_t state_go_ahead_after_turn_data;
volatile state_go_a_bit_more_data_t state_go_a_bit_more_data;
volatile state_second_left_turn_data_t state_second_left_turn_data;
volatile state_second_go_ahead_data_t state_second_go_ahead_data;
volatile state_second_go_a_bit_more_data_t state_second_go_a_bit_more_data;
volatile state_third_left_turn_data_t state_third_left_turn_data;
volatile state_third_go_ahead_data_t state_third_go_ahead_data;
volatile state_right_turn_data_t state_right_turn_data;
volatile state_last_go_ahead_data_t state_last_go_ahead_data;
volatile state_finish_data_t state_finish_data;

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
			state_wait_before_corner_data.not_first_run=false;
			state_wait_before_corner_data.exp=false;
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

void state_wait_before_corner_logic() {
	if (state_wait_before_corner_data.not_first_run == false){
		state_wait_before_corner_data.not_first_run = true;
		task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_WAIT_BEFORE_CORNER};
		add_task(&system_state);
		set_m_backward()
		l_motor.rpm = 0;
		r_motor.rpm = 0;
		state_wait_before_corner_data.exp == false;
		tmr_start(&state_wait_before_corner_timer,STATE_WAIT_BEFORE_CORNER_INTERVAL);
	}
	
	if (state_wait_before_corner_data.exp == true || tmr_exp(&state_wait_before_corner_timer)){
		state_wait_before_corner_data.exp = true;
		set_m_forward()
		l_motor.rpm = 100;
		r_motor.rpm = 100;
		control = &state_approach_corner_logic;
		state_approach_corner_data.not_first_run=false;
		state_approach_corner_data.exp=false;
		return;
	}	
}

void state_approach_corner_logic() {
		if (state_approach_corner_data.not_first_run == false){
		state_approach_corner_data.not_first_run = true;
		task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_APPROACH_CORNER};
		add_task(&system_state);
		state_approach_corner_data.exp == false;
		tmr_start(&state_approach_corner_timer,STATE_APPROACH_CORNER_INTERVAL);
	}
	
	if (state_approach_corner_data.exp == true || tmr_exp(&state_approach_corner_timer)){
		state_approach_corner_data.exp = true;
		set_m_backward()
		l_motor.rpm = 0;
		r_motor.rpm = 0;
		state_turn_after_found_corner_data.not_first_run=false;
		state_turn_after_found_corner_data.exp=false;
		control = &state_turn_after_found_corner_logic;
		return;
	}	
}

void state_turn_after_found_corner_logic() {
		if (state_turn_after_found_corner_data.not_first_run == false){
		state_turn_after_found_corner_data.not_first_run = true;
		task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_TURN_AFTER_FOUND_CORNER};
		add_task(&system_state);
		state_turn_after_found_corner_data.exp == false;
		tmr_start(&state_turn_after_found_corner_timer,STATE_TURN_AFTER_FOUND_CORNER_TURNING_INTERVAL);
		set_l_backward()
		set_r_forward()
		r_motor.rpm = 50;
		l_motor.rpm = 50;
	}
	
	if (state_turn_after_found_corner_data.exp == true || tmr_exp(&state_turn_after_found_corner_timer)){
		state_turn_after_found_corner_data.exp = true;
		set_r_backward()
		set_l_forward()
		r_motor.rpm = 0;
		l_motor.rpm = 0;
		state_go_ahead_after_turn_data.not_first_run=true;
		state_go_ahead_after_turn_data.exp=true;
		control = &state_go_ahead_after_turn_logic;
		return;
	}	
}

void state_go_ahead_after_turn_logic() {
	
}

void state_go_a_bit_more_logic() {
	
}

void state_second_left_turn_logic() {
	
}

void state_second_go_ahead_logic() {
	
}

void state_second_go_a_bit_more_logic() {
	
}

void state_third_left_turn_logic() {
	
}

void state_third_go_ahead_logic() {
	
}

void state_right_turn_logic() {
	
}

void state_last_go_ahead_logic() {
	
}

void state_finish_logic() {
	
}
void state_take_over_control_logic() {
	
}