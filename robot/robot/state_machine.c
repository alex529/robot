#include <stdio.h>
#include <stdlib.h>

#include "state_machine.h"
#include "state_machine_event_buffer.h"

typedef struct {
    int st;
    int ev;
    void (*fn)();
} transition_t;

transition_t transition_table[] = {
    {STATE_IDLE,  EVENT_BUTTON_START, &set_state_to_find_track},
    {STATE_FIND_TRACK, EVENT_BUTTON_RESET, &set_state_to_idle}
};

#define TRANS_COUNT (sizeof(transition_table)/sizeof(*transition_table))

state_t state = STATE_IDLE;

void state_machine() {
    event_t current_event;
    current_event = delete_event();
    if (current_event == -1) {
        return;
    }
    int i;
    for (i = 0; i < TRANS_COUNT; i++) {
        if ((state == transition_table[i].st) ) {
            if ((current_event == transition_table[i].ev)) {
                (transition_table[i].fn)();
                break;
            }
        }
    }
}


