#ifndef STATE_MACHINE_H_INCLUDED
#define STATE_MACHINE_H_INCLUDED

#include "buttons.h"

typedef enum {STATE_IDLE, STATE_FIND_TRACK, STATE_FOLLOW_TRACK_1, STATE_Y_INTERSECTION, STATE_FOLLOW_TRACK_2} state_t;
typedef enum {EVENT_BUTTON_RESET, EVENT_BUTTON_START} event_t;

extern state_t state;

void state_machine();

#endif // STATE_MACHINE_H_INCLUDED
