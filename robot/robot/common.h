/*
 * common.h
 *
 * Created: 2/28/2014 11:58:57 AM
 *  Author: Administrator
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <avr/io.h>
#include "timer.h"
#include "task.h"


/**
 * \brief Toggles the LED that is present on port b pin 0.
 */
#define toggle_led()		{PORTB^=(1<<PB7);}
/*
 * Sets the LED on.
 */
#define led_on()			{PORTB&=~(1<<PB7);}
/*
 * Sets the LED off.
 */
#define led_off()			{PORTB|=(1<<PB7);}
/**
 * \brief Used for copying task buffers to the UART, copies first 5 bits of a buffer.
 */
#define task_buffer_copy(paste_buffer,copy_buffer)		{paste_buffer[0]=copy_buffer[0]; paste_buffer[1]=copy_buffer[1]; paste_buffer[2]=copy_buffer[2]; paste_buffer[3]=copy_buffer[3]; paste_buffer[4]=copy_buffer[4]; paste_buffer[5]=copy_buffer[5]; paste_buffer[6]=copy_buffer[6]; paste_buffer[7]=copy_buffer[7]; paste_buffer[8]=copy_buffer[8];}
/**
 * Decodes an uint value into a string.
 * NOTE!!! Last char has to be manually set to 0 if string parsing wants to be used.
 */
#define uint_to_string(uint_nr,string_ptr, string_size)	{for(int i=0;uint_nr || i>string_size;i++){string_ptr[string_size-i-1]=uint_nr%10+48;	uint_nr/=10;}}
/**
 * Swaps the value of 2 1byte variables.
 */
#define swap_uint8_t(x,y) {uint8_t temp;temp=x;x=y;y=temp;}//if memory needed can be changed with x^=y;y^=x;x^=y;	
	
/**
 * Used for debug porpoises it sends an INIT_CONN command with a desired value attached to it.
 */
task_t test_task;
#define debug_task(u32_value)							{test_task.data.command = INIT_CONN;	test_task.data.value = u32_value;	USART_transmit_command(&test_task);}
	
/**
 * Returns the size of an buffer
 */
#define get_array_len(arrayName) (sizeof(arrayName) / sizeof((arrayName)[0]))
		
/**
* Used for converting different types of values
*/
typedef union
{
    uint8_t b[4];
    uint16_t w[2];
    uint32_t dw;
} u32_union;

/**
* Encapsulates all the system status related flags in one place, for easy assignment and clearing.
*/
typedef union
{
	struct
	{
		uint8_t connected : 1;
		
		uint8_t sending_task : 1;
		uint8_t task_received : 1;
		
		uint8_t java_in_control : 1;
		uint8_t not_used4 : 1;		
		uint8_t not_used5 : 1;		
		uint8_t not_used6 : 1;
		uint8_t not_used7 : 1;
	} system;
	uint8_t byte[1];
} status_t;


timer_t test_timer;
task_t tes1_task;


extern volatile status_t status;
extern volatile bool run_card_reader;

#endif /* COMMON_H_ */