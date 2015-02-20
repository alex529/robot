/*
 * adc.h
 *
 * Created: 2/19/2015 6:40:29 PM
 *  Author: Adam Orosz, s134615
 */ 


#ifndef ADC_H_
#define ADC_H_

typedef struct {
	uint16_t results[8];
	bool itr8307ResultsPresent;
	bool proximitySensorResultPresent;
}adc_values_t;

extern volatile int8_t current_channel;
extern volatile bool itr8307sOnly;
extern volatile bool conversionIsInProgress;
extern volatile adc_values_t adc_values;

void disableADC(); 
void enableADC();
void handleMeasurement();
void measureWithBothKindOfSensors();
void measureWithItr8307sOnly();
void measure();
void adc_measurement_init();



#endif /* ADC_H_ */