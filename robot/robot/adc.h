/*
* adc.h
*
* Created: 2/19/2015 6:40:29 PM
*  Author: Adam Orosz, s134615
*/


#ifndef ADC_H_
#define ADC_H_

#define setChannel(pin_number){ADMUX= ((pin_number && 0x1F) | (ADMUX&0xE0));}
#define STAGE_FINISH 99


extern volatile uint8_t h_ch0;
extern volatile uint8_t l_ch0;
extern volatile	uint8_t h_ch1;
extern volatile uint8_t l_ch1;
extern volatile	uint16_t result;
extern volatile bool new_data_available;
extern volatile bool new_data_available_to_transmit;
extern volatile uint8_t stage;
extern volatile bool conversionIsInProgress;

void enable_adc();
void disable_adc();
void handleMeasurement();
void measureWithBothKindOfSensors();
void measureWithItr8307sOnly();
void measure();
void adc_measurement_init();
void send_adc_value_to_pc();



#endif /* ADC_H_ */