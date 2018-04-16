#ifndef HARDWARE_ADC_DRIVER_H_
#define HARDWARE_ADC_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define ADC_BUFFER_SIZE     5

void ADC_Init(void);
void ADC_InitMultiple(void);
void ADC_WaitSample(void);
uint16_t *ADC_GetBuffer(void);

#endif /* HARDWARE_ADC_DRIVER_H_ */
