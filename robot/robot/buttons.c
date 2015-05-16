#include <stdio.h>
#include <stdlib.h>

#include "buttons.h"

void find_track_state_change() {
state = STATE_FIND_TRACK;
printf("State has been set to find track \n");
}

void idle_state_change() {
state = STATE_IDLE;
printf("State has been set to idle track \n");
}
