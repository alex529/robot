#ifndef STATE_MACHINE_EVENT_BUFFER_H_INCLUDED
#define STATE_MACHINE_EVENT_BUFFER_H_INCLUDED

#include    "state_machine.h"
#include    <stdint.h>

uint8_t delete_event();
int add_event(event_t event);
void reset_events();


#endif // STATE_MACHINE_EVENT_BUFFER_H_INCLUDED
