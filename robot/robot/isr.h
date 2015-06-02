/*
 * isr.h
 *
 * Created: 2/17/2014 12:57:05 PM
 *  Author: Alexandru Savin
 */ 


#ifndef ISR_H_
#define ISR_H_
#include "task.h"
extern volatile uint32_t system_tic;

void set_Kp(task_t *task);
void set_Kd(task_t *task);

#endif /* ISR_H_ */