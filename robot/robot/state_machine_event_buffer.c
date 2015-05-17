#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "state_machine.h"

#define MAX_QUEUE_SIZE 30
#define ERROR 255

event_t events [MAX_QUEUE_SIZE];
int8_t front = -1;
uint8_t count = 0;

uint8_t delete_event() {
    if (count > 0) {
        --count;
        uint8_t event;
        event = events[front];
        if (front == 0) {
            front = MAX_QUEUE_SIZE-1;
        } else {
            --front;
        }
        return event;
        } else {
            return ERROR;
        }
    }

int add_event(event_t event) {
    if (count <= (MAX_QUEUE_SIZE-1)) {
        ++count;
        if(front <= (MAX_QUEUE_SIZE-1)) {
            ++front;
            events[front] = event;
        } else {
            front = 0;
            events[0] = event;
        }
        return 0;
        }else {
        return -1;}
    }

void reset_events() {
    front = -1;
    count = 0;

}


