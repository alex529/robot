/*
 * com_prot.h
 *
 *  Created: 4/28/2014 2:07:06 AM
 *  Author: Alexandru
 */ 


#ifndef COM_PROT_H_
#define COM_PROT_H_


#include "timer.h"


extern timer_t ping_tmr;

void com_prot_task(void);

void communication_init(task_t *task);
void stop(task_t *task);


#endif /* COM_PROT_H_ */