/*
 * date.h
 *
 * Created: 4/27/2014 2:41:37 PM
 *  Author: Alexandru
 */ 


#ifndef DATE_H_
#define DATE_H_
#include "task.h"

typedef struct
{
	uint32_t hour;
	uint32_t minute;
	uint32_t second;
} date_t;

extern volatile date_t date;

void clock_tick(void);
void get_date(void);

void date_to_string(char *date_20b);

#endif /* DATE_H_ */