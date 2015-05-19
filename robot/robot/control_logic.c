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

#define STATE_FIND_TRACK_SENSOR_BLACKOUT_INTERVAL	2000
timer_t state_find_track_sensor_blackout_timer;

volatile state_find_track_data_t state_find_track_data;

void state_idle_control_logic() {
	// left bank intentionally 
}

void state_find_track_control_logic() {
	if (state_find_track_data.init == true){
		state_find_track_data.init = false;
		drive(124,127);
		enable_features.generate_events=false;
		//can I restart timer without redeclaration
		tmr_start(&state_find_track_sensor_blackout_timer,STATE_FIND_TRACK_SENSOR_BLACKOUT_INTERVAL); 
	}
	
	//how to make it execute only onces
	if (tmr_exp(&state_find_track_sensor_blackout_timer)){
		enable_features.generate_events=true;
	}
	
}

void state_follow_track_1_control_logic() {
	
}

void state_y_intersection_control_logic() {
	
}

void state_follow_track_2_control_logic() {
	
}

void state_take_over_control_logic() {
	
}