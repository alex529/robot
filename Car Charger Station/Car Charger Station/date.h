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
	uint32_t day;
	uint32_t month;
	uint32_t year;	
} date_t;

extern volatile date_t date;

void set_hour(task_t *p);
void set_minute(task_t *p);
void set_second(task_t *p);
void set_day(task_t *p);
void set_month(task_t *p);
void set_year(task_t *p);
void clock_tick(void);
void get_date(void);

void date_to_string(char *date_20b);

#endif /* DATE_H_ */