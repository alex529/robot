/**
 * isr.c
 *
 * The isr.c file is in charge of servicing the ISR routines.
 *
 *  Created: 2/17/2014 12:57:18 PM
 *  Author: Alexandru Savin
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "usart.h"
#include "isr.h"
#include "timer.h"
#include "common.h"

volatile status_t status;
volatile uint32_t system_tic;
    
/**
 * \brief Adds the received data to the rx_buffer when the frame is received a flag is raised.
 *			When the frame is ready the value bites are swapped.
 *
 * \author Alexandru
 *
 * \param Vector for the interrupt.
 * 
 * \return 
 */
uint8_t data;
ISR(USART_RXC_vect)
{
    data = get_usart();
    toggle_led();
    enable_uart_transmision(); 
	//static uint8_t rx_index = 0;
	//status.interrupt.uart_rx = true;
    //rx_buffer[rx_index++]=get_usart();
	//if(rx_index==USART_FRAME_SIZE)
	//{
		//swap_uint8_t(rx_buffer[1],rx_buffer[4]);//bytes need to be swapped due to different arrangement in the server
		//swap_uint8_t(rx_buffer[2],rx_buffer[3]);
		//rx_index = 0;
		//status.system.ack_received = true;
	//}
}
    
/**
 * \brief Copies the tx_buffer byte by byte to the U SART module buffer.
 *
 * \author Alexandru
 *
 * \param Vector for the  interrupt.
 * 
 * \return 
 */
uint8_t index;
ISR(USART_UDRE_vect)
{
    
    set_usart(tx_buffer[index++]);
    if (index > 20)
    {
        index = 0;
        disable_uart_transmision();
    }
}
    
/**
 * \brief Increments the global millisecond variable that is needed for the timer.c module and
 *			facilitates a system tick(100ms) used for scheduling purposes in the main loop.
 *
 * \author Alexandru
 *
 * \param Vector for the  interrupt.
 * 
 * \return 
 */
ISR(TIMER1_COMPA_vect)
{
    milliseconds++;
    if (milliseconds%100 == 0) //small error every ~50 days max number being 4294967295, so every 50 days the systems will have a 5 milliseconds error if the system is rebooted between this periods the system will act normally
    {
        system_tic = true;
    }
}
    
///**
 //* \brief When the card reader can supply a valid card id a flag is raised.
 //* 
 //* \param Vector for the  interrupt.
 //* 
 //* \return 
 //*/
//ISR(INT0_vect)
//{
	//run_card_reader = true;	
	////tmr_start(&debounce_timer,KEYPAD_DEBOUNCE_DELAY);//TODO if used with spi check for right level and also take care of the release debounce
//}
//
//
    //
///**
 //* \brief ADC measure completed service routine handler.
 //*
 //* \author Adam
 //*
 //* \param Vector for the  interrupt
 //* 
 //* \return 
 //*/
//ISR(ADC_vect)
//{	
	////temp value
	//int value=0;
//
	////the measured value is 2+8 bits long. The following 2 lines creates a 10bit value from the 2+8 bit values
	//value = ADCL;
	//value = value + (ADCH<<8);
	//
	////checks which ADC channel is being used and puts the value to the right place
	//if ((ADMUX & (1<<MUX0))==0) 
	//{
		//update_circuit_values_ad0(value);
	//}
	//else
	//{
		//update_circuit_values_ad1(value);
	///* the new values are being "injected" here. these values will be the current ones (in file circuit_calculation).
	 //* the values which were the current ones BEFORE the call, will be shifted and become the previous values.*/
//
	//
	 ///* does all the calculation job and updates the the global consumption variable.
	 //* the first call of this function won't update the global variable since the init values are 0
	 //* and that would create an invalid calculation.*/
	//calculate_circuit_values();
	//}
	//
	//
//}