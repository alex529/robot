/*
 * adc.h
 *
 * Created: 2/19/2015 6:40:29 PM
 *  Author: Adam Orosz, s134615
 */ 


#ifndef ADC_H_
#define ADC_H_

extern volatile int current_channel;
extern volatile bool itr8307sOnly;
extern volatile bool conversionIsInProgress = 0;


void handleMeasurement();
void measureWithBothKindOfSensors();
void measureWithItr8307sOnly();
void measure();
void adc_measurement_init();


#endif /* ADC_H_ */