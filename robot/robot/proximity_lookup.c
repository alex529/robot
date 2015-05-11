/*
 * proximity_lookup.c
 *
 * Created: 5/9/2015 6:12:48 PM
 *  Author: adam
 */ 

#include <stdint.h>
#include "proximity_lookup.h"

typedef struct 
{
	uint16_t start; // v*100
	uint16_t end; //v*100
	uint8_t cm; //cm*10
	
} entry_t;

entry_t lookup_table [] = {
	{200, 210, 10},
	{220, 230, 20}
};

#define MAX_VOLTAGE 400
#define MIN_VOLTAGE 190

#define SIZE (sizeof(lookup_table)/sizeof(*lookup_table))
	
	uint8_t lookup(uint16_t voltage) //v*100
	{
		if (voltage < MIN_VOLTAGE || voltage > MAX_VOLTAGE)
		{
			return ERROR;
		}
		int i;
		for (i=0;i<SIZE;i++) {
			if (lookup_table[i].start<=voltage && lookup_table[i].end>=voltage) {
				return lookup_table[i].cm;
			}
		}
		return ERROR;
	}
	
