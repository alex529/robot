/*
 * date.c
 *
 * The date.c file is in charge of keeping track of the system clock.
 *
 *  Created: 4/27/2014 2:41:16 PM
 *  Author: Alexandru Savin
 */

#include <avr/io.h>
#include <stdbool.h>
#include "date.h"
#include "task.h"
#include "common.h"

/** 
 * Checks if a year has 365 or 366 days.
 */
#define check_leap_year(year) (((year%4==0)&&(year%100!=0))||(year%400==0))

volatile date_t date;
static const uint8_t month_days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
static bool leap_year = false;


/**
 * \brief Sets the hour.
 *
 * \author Alexandru
 *
 * \param p Used to specify a pointer to an appropriate task struct.
 * 
 * \return void
 */
void set_hour(task_t *p)
{
	date.hour=p->data.value;
}

/**
 * \brief Sets the minute.
 *
 * \author Alexandru
 *
 * \param p Used to specify a pointer to an appropriate task struct.
 * 
 * \return void
 */
void set_minute(task_t *p)
{
	date.minute=p->data.value;
}

/**
 * \brief Sets the second.
 *
 * \author Alexandru
 *
 * \param p Used to specify a pointer to an appropriate task struct.
 * 
 * \return void
 */
void set_second(task_t *p)
{
	date.second=p->data.value;
}

/**
 * \brief Sets the day.
 *
 * \author Alexandru
 *
 * \param p Used to specify a pointer to an appropriate task struct.
 * 
 * \return void
 */
void set_day(task_t *p)
{
	date.day=p->data.value;
}

/**
 * \brief Sets the month.
 *
 * \author Alexandru
 *
 * \param p Used to specify a pointer to an appropriate task struct.
 * 
 * \return void
 */
void set_month(task_t *p)
{
	date.month=p->data.value;
}

/**
 * \brief Sets the year.
 *
 * \author Alexandru
 *
 * \param p Used to specify a pointer to an appropriate task struct.
 * 
 * \return void
 */
void set_year(task_t *p)
{
	date.year=p->data.value;
	leap_year = check_leap_year(date.year);	
}


/**
 * \brief Increments the seconds and checks if all the date related proprieties are valid.
 *
 * \author Alexandru
 *
 * \param 
 * 
 * \return void
 */
void clock_tick(void)
{
	date.second++;
	if (date.second == 60)
	{
		date.second =0;
		date.minute++;
		if (date.minute == 60)
		{
			date.minute =0;
			date.hour++;
			if (date.hour == 24)
			{
				date.hour =0;
				date.day++;
				if(leap_year == true)//check if February
				{
					if (date.month == 1 && date.day == 30)
					{
						date.day =1;
						date.month++;
					}
				}
				else if (date.day == month_days[date.month-1]+1)
				{
					date.day =1;
					date.month++;
					if (date.month == 12+1)
					{
						date.month =1;
						date.year++;
						leap_year = check_leap_year(date.year);
					}
				}
			}
		}
	}
}

/**
 * \brief Decodes the date struct into a string and copy's it to the passed var.
 *
 * \author Alexandru
 *
 * \param date_21b Used to specify  a 21bit char buffer.
 * 
 * \return void
 */
void date_to_string(char *date_21b) //TODO redo all to string function and add terminator caracter(0)
{
	date_21b[ 0]= date.hour/10+48;
	date_21b[ 1]= date.hour%10+48;
	date_21b[ 2]= ':';
	date_21b[ 3]= date.minute/10+48;
	date_21b[ 4]= date.minute%10+48;
	date_21b[ 5]= ':';
	date_21b[ 6]= date.second/10+48;
	date_21b[ 7]= date.second%10+48;
	date_21b[ 8]= ' ';
	date_21b[ 9]= ' ';
	date_21b[10]= date.day/10+48;
	date_21b[11]= date.day%10+48;
	date_21b[12]= '-';
	date_21b[13]= date.month/10+48;
	date_21b[14]= date.month%10+48;
	date_21b[15]= '-';
	date_21b[16]= date.year/1000+48;
	date_21b[17]= date.year/100%10+48;
	date_21b[18]= date.year/10%10+48;
	date_21b[19]= date.year%10+48;
	date_21b[20]= '\n';
    
}


/**
 * \brief Asks the server for a new date struct, synchronizing the date.
 *
 * \author Alexandru
 *
 * \param 
 * 
 * \return void
 */
void get_date(void)
{	
	task_t time;
	time.data.command = ASK_YEAR;
	time.data.value=0;
	add_task(&time);
	add_task(&time);
	time.data.command = ASK_MONTH;
	add_task(&time);
	time.data.command = ASK_DAY;
	add_task(&time);
	time.data.command = ASK_HOUR;
	add_task(&time);
	time.data.command = ASK_MINUTE;
	add_task(&time);
	time.data.command = ASK_SECOND;
	add_task(&time);
}