#include <stdio.h>
#include <stdlib.h>

#include "buttons.h"

void set_state_to_find_track() {
state = STATE_FIND_TRACK;
printf("State has been set to find track \n");
}

void set_state_to_idle() {
state = STATE_IDLE;
printf("State has been set to idle track \n");
}
