/*
 * control_logic.h
 *
 * Created: 5/16/2015 3:38:45 PM
 *  Author: Adam Orosz, s134615
 */ 


#ifndef CONTROL_LOGIC_H_
#define CONTROL_LOGIC_H_

void state_idle_control_logic();
void state_find_track_control_logic(); 
void state_follow_track_1_control_logic(); 
void state_y_intersection_control_logic(); 
void state_follow_track_2_control_logic(); 
void state_take_over_control_logic();

typedef struct  
{
	bool not_first_run;
	bool exp;
} state_find_track_data_t;

extern volatile state_find_track_data_t state_find_track_data;
#endif /* CONTROL_LOGIC_H_ */