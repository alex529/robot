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
#include "adc.h"

#define STATE_FIND_TRACK_SENSOR_BLACKOUT_INTERVAL		2000
#define STATE_WAIT_BEFORE_CORNER_INTERVAL				3000
//#define STATE_APPROACH_CORNER_INTERVAL					3000 // to be deleted
#define STATE_TURN_AFTER_FOUND_CORNER_TURNING_INTERVAL	1000
//#define STATE_GO_AHEAD_AFTER_TURN_INTERVAL				3000 // to be deleted
#define STATE_GO_A_BIT_MORE_INTERVAL					500
#define STATE_SECOND_LEFT_TURN_INTERVAL					1000
//#define STATE_SECOND_GO_AHEAD_INTERVAL					1000  // to be deleted
#define STATE_SECOND_GO_A_BIT_MORE_INTERVAL				500
#define STATE_THIRD_LEFT_TURN_INTERVAL					1000
//#define STATE_THIRD_GO_AHEAD_INTERVAL					2000 // to be deleted
#define STATE_RIGHT_TURN_INTERVAL						1000
//#define STATE_LAST_GO_AHEAD_INTERVAL					2000 // to be deleted

#define STATE_APPROACH_CORNER_MINIMAL_VOLTAGE			800
#define STATE_APPROACH_CORNER_MAXIMAL_VOLTAGE			1000
#define STATE_GO_AHEAD_AFTER_TURN_VOLTAGE				600
#define STATE_SECOND_GO_AHEAD_VOLTAGE					600
#define STATE_THIRD_GO_AHEAD_MAXIMAL_VOLTAGE			1000
#define STATE_THIRD_GO_AHEAD_MINIMAL_VOLTAGE			800
#define STATE_LAST_GO_AHEAD_MAXIMAL_VOLTAGE				1000
#define STATE_LAST_GO_AHEAD_MINIMAL_VOLTAGE				800



timer_t state_find_track_sensor_blackout_timer;
timer_t state_wait_before_corner_timer;
//timer_t state_approach_corner_timer; // to be deleted
timer_t state_turn_after_found_corner_timer;
//timer_t state_go_ahead_after_turn_timer; // to be deleted
timer_t state_go_a_bit_more_timer;
timer_t state_second_left_turn_timer;
//timer_t state_second_go_ahead_timer; // to be deleted
timer_t state_second_go_a_bit_more_timer;
timer_t state_third_left_turn_timer;
//timer_t state_third_go_ahead_timer; //to be deleted
timer_t state_right_turn_timer;
//timer_t state_last_go_ahead_timer; //to be deleted

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
		tmr_start(&state_find_track_sensor_blackout_timer,STATE_FIND_TRACK_SENSOR_BLACKOUT_INTERVAL); 
	}
	
	if (state_find_track_data.exp == true || tmr_exp(&state_find_track_sensor_blackout_timer)){
		state_find_track_data.exp = true;
		read_switches();
		uint8_t sensor_value = led.array;
		if ((sensor_value & 0x7e)!= 0)
		{
			state_find_track_data.exp=false;
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
		tmr_start(&state_wait_before_corner_timer,STATE_WAIT_BEFORE_CORNER_INTERVAL);
		return;
	}
	
	if (tmr_exp(&state_wait_before_corner_timer)){
		control = &state_approach_corner_logic;
		state_approach_corner_data.not_first_run = false;
		return;
	}	
}

void state_approach_corner_logic() {
		if (state_approach_corner_data.not_first_run == false){
		state_approach_corner_data.not_first_run = true;
		task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_APPROACH_CORNER};
		add_task(&system_state);
		set_m_forward()
		l_motor.rpm = 100;
		r_motor.rpm = 100;
		return;
	}
	
	if (new_data_available){
		new_data_available = false;
		if (result0 >= STATE_APPROACH_CORNER_MINIMAL_VOLTAGE && result0 <= STATE_APPROACH_CORNER_MAXIMAL_VOLTAGE)
		{
			set_m_backward()
			l_motor.rpm = 0;
			r_motor.rpm = 0;
			control = &state_turn_after_found_corner_logic;
			state_turn_after_found_corner_data.not_first_run = false;
			return;
		}
	}
}

void state_turn_after_found_corner_logic() {
		if (state_turn_after_found_corner_data.not_first_run == false){
		state_turn_after_found_corner_data.not_first_run = true;
		task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_TURN_AFTER_FOUND_CORNER};
		add_task(&system_state);
		tmr_start(&state_turn_after_found_corner_timer,STATE_TURN_AFTER_FOUND_CORNER_TURNING_INTERVAL);
		set_m_forward()
		l_motor.rpm = 100;
		r_motor.rpm = 0;
		return;
	}
	
	if (tmr_exp(&state_turn_after_found_corner_timer)){
		set_m_backward()
		r_motor.rpm = 0;
		l_motor.rpm = 0;
		control = &state_go_ahead_after_turn_logic;
		state_go_ahead_after_turn_data.not_first_run = false;
		return;
	}	
}

void state_go_ahead_after_turn_logic() {
		if (state_go_ahead_after_turn_data.not_first_run == false){
			state_go_ahead_after_turn_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_GO_AHEAD_AFTER_TURN};
			add_task(&system_state);
			set_m_forward()
			r_motor.rpm = 100;
			l_motor.rpm = 100;
			return;
		}
		
	if (new_data_available){
		new_data_available = false;
		if (result1 <= STATE_GO_AHEAD_AFTER_TURN_VOLTAGE)
		{
			set_m_backward()
			l_motor.rpm = 0;
			r_motor.rpm = 0;
			control = &state_go_a_bit_more_logic;
			state_go_a_bit_more_data.not_first_run = false;
			return;
		}
	}
}

void state_go_a_bit_more_logic() {
		if (state_go_a_bit_more_data.not_first_run == false){
			state_go_a_bit_more_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_GO_A_BIT_MORE};
			add_task(&system_state);
			tmr_start(&state_go_a_bit_more_timer,STATE_GO_A_BIT_MORE_INTERVAL);
			set_m_forward()
			r_motor.rpm = 100;
			l_motor.rpm = 100;
			return;
		}
		
		if (tmr_exp(&state_go_a_bit_more_timer)){
			set_m_backward()
			r_motor.rpm = 0;
			l_motor.rpm = 0;
			control = &state_second_left_turn_logic;
			state_second_left_turn_data.not_first_run = false;
			return;
		}
}

void state_second_left_turn_logic() {
		if (state_second_left_turn_data.not_first_run == false){
			state_second_left_turn_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_SECOND_LEFT_TURN};
			add_task(&system_state);
			tmr_start(&state_second_left_turn_timer,STATE_SECOND_LEFT_TURN_INTERVAL);
			set_m_forward()
			r_motor.rpm = 100;
			l_motor.rpm = 0;
			return;
		}
		
		if (tmr_exp(&state_second_left_turn_timer)){
			set_m_backward()
			r_motor.rpm = 0;
			l_motor.rpm = 0;
			control = &state_second_go_ahead_logic;
			state_second_go_ahead_data.not_first_run = false;
			return;
		}	
}

void state_second_go_ahead_logic() {
		if (state_second_go_ahead_data.not_first_run == false){
			state_second_go_ahead_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_SECOND_GO_AHEAD};
			add_task(&system_state);
			set_m_forward()
			r_motor.rpm = 100;
			l_motor.rpm = 100;
			return;
		}
		
	if (new_data_available){
		new_data_available = false;
		if (result1 <= STATE_SECOND_GO_AHEAD_VOLTAGE)
		{
			set_m_backward()
			l_motor.rpm = 0;
			r_motor.rpm = 0;
			control = &state_second_go_a_bit_more_logic;
			state_second_go_a_bit_more_data.not_first_run = false;
			return;
		}		
	}
}

void state_second_go_a_bit_more_logic() {
	if (state_second_go_a_bit_more_data.not_first_run == false){
			state_second_go_a_bit_more_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_SECOND_GO_A_BIT_MORE};
			add_task(&system_state);
			tmr_start(&state_second_go_a_bit_more_timer,STATE_SECOND_GO_A_BIT_MORE_INTERVAL);
			set_m_forward()
			r_motor.rpm = 100;
			l_motor.rpm = 100;
			return;
		}
		
		if (tmr_exp(&state_second_go_a_bit_more_timer)){
			set_m_backward()
			r_motor.rpm = 0;
			l_motor.rpm = 0;
			control = &state_third_left_turn_logic;
			state_third_left_turn_data.not_first_run = false;
			return;
		}	
}

void state_third_left_turn_logic() {
	if (state_third_left_turn_data.not_first_run == false){
			state_third_left_turn_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_THIRD_LEFT_TURN};
			add_task(&system_state);
			tmr_start(&state_third_left_turn_timer,STATE_THIRD_LEFT_TURN_INTERVAL);
			set_m_forward()
			r_motor.rpm = 100;
			l_motor.rpm = 0;
			return;
		}
		
		if (tmr_exp(&state_third_left_turn_timer)){
			set_m_backward()
			r_motor.rpm = 0;
			l_motor.rpm = 0;
			control = &state_third_go_ahead_logic;
			state_third_go_ahead_data.not_first_run = false;
			return;
		}
}

void state_third_go_ahead_logic() {
	if (state_third_go_ahead_data.not_first_run == false){
			state_third_go_ahead_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_THIRD_GO_AHEAD};
			add_task(&system_state);
			set_m_forward()
			r_motor.rpm = 100;
			l_motor.rpm = 100;
			return;
		}
		
	if (new_data_available){
		new_data_available = false;
		if (result0 <= STATE_THIRD_GO_AHEAD_MAXIMAL_VOLTAGE && result0 >= STATE_THIRD_GO_AHEAD_MINIMAL_VOLTAGE)
		{
			set_m_backward()
 			r_motor.rpm = 0;
 			l_motor.rpm = 0;
			control = &state_right_turn_logic;
			state_right_turn_data.not_first_run = false;
			return;
		}				
	}
}

void state_right_turn_logic() {
	if (state_right_turn_data.not_first_run == false){
			state_right_turn_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_RIGHT_TURN};
			add_task(&system_state);
			tmr_start(&state_right_turn_timer,STATE_RIGHT_TURN_INTERVAL);
			set_m_forward()
			r_motor.rpm = 0;
			l_motor.rpm = 100;
			return;
		}
		
		if (tmr_exp(&state_right_turn_timer)){
			set_m_backward()
			r_motor.rpm = 0;
			l_motor.rpm = 0;
			control = &state_last_go_ahead_logic;
			state_last_go_ahead_data.not_first_run = false;
			return;
		}
}

void state_last_go_ahead_logic() {
	if (state_last_go_ahead_data.not_first_run == false){
			state_last_go_ahead_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_LAST_GO_AHEAD};
			add_task(&system_state);
			set_m_forward()
			r_motor.rpm = 100;
			l_motor.rpm = 100;
			return;
		}
		
	if (new_data_available){
		new_data_available = false;
		if (result0 <= STATE_LAST_GO_AHEAD_MAXIMAL_VOLTAGE && result0 >= STATE_LAST_GO_AHEAD_MINIMAL_VOLTAGE)
		{
			set_m_backward()
 			r_motor.rpm = 0;
 			l_motor.rpm = 0;
			control = &state_finish_logic;
			state_finish_data.not_first_run = false;
			return;
		}
	}
		
}

void state_finish_logic() {
	if (state_finish_data.not_first_run == false){
			state_finish_data.not_first_run = true;
			task_t system_state = {.data.command = STATE_COMMAND, .data.timestamp=0, .data.value=STATE_FINISH};
			add_task(&system_state);
			set_m_backward()
			r_motor.rpm = 0;
			l_motor.rpm = 0;
		}
}

void state_take_over_control_logic() {
	
}