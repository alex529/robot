#include <stdio.h>
#include <stdlib.h>

#include "state_machine.h"
#include "state_machine_event_buffer.h"
#include "state_logics.h"

#define ERROR 255

typedef struct {
    int st;
    int ev;
    void (*fn)();
} transition_t;

transition_t transition_table[] = {
    {STATE_IDLE,  EVENT_BUTTON_START, &find_track_state_change},
    {STATE_FIND_TRACK, EVENT_BUTTON_RESET, &idle_state_change},
	{STATE_ANY, JAVA_IDLE, &enter_state_idle},
	{STATE_ANY, JAVA_FIND_TRACK, &enter_state_find_track},
	{STATE_ANY, JAVA_FOLLOW_TRACK_1, &enter_state_follow_track_1},
	{STATE_ANY, JAVA_Y_INTERSECTION, &enter_state_y_intersection},
	{STATE_ANY, JAVA_FOLLOW_TRACK_2, &enter_state_follow_track_2}
};



#define TRANS_COUNT (sizeof(transition_table)/sizeof(*transition_table))

state_t state = STATE_IDLE;

void state_machine() {
    event_t current_event;
    current_event = delete_event();
    int i;
	while (current_event != ERROR) {
    for (i = 0; i < TRANS_COUNT; i++) {
        if ((state == transition_table[i].st || transition_table[i].st ==  STATE_ANY) ) {
            if ((current_event == transition_table[i].ev)) {
                (transition_table[i].fn)();
                break;
            }
        }
    }
	current_event = delete_event();
	}
}


