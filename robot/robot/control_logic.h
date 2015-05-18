/*
 * control_logic.h
 *
 * Created: 5/16/2015 3:38:45 PM
 *  Author: adam
 */ 


#ifndef CONTROL_LOGIC_H_
#define CONTROL_LOGIC_H_

void state_idle_control_logic();
void state_find_track_control_logic(); 
void state_follow_track_1_control_logic(); 
void state_y_intersection_control_logic(); 
void state_follow_track_2_control_logic(); 
void state_take_over_control_logic();


#endif /* CONTROL_LOGIC_H_ */