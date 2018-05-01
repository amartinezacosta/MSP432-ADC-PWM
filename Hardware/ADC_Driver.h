#ifndef HARDWARE_ADC_DRIVER_H_
#define HARDWARE_ADC_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define ADC_BUFFER_SIZE     5
#define ADC_SAMPLE_READY    1

void ADC_Enable(uint32_t ClockSource, uint32_t Predivider, uint32_t Divider);
void ADC_InitSingle(uint32_t Pin, uint32_t ADC_Mem);
void ADC_InitMultiple(uint32_t Pins, uint32_t Start, uint32_t End, uint32_t PinCount);
void ADC_RegisterCallback(void(*Callback)(uint32_t Event));
uint16_t *ADC_GetBuffer(void);
uint16_t ADC_Read(uint32_t ADC_Mem);
void ADC_ReadMultiple(uint16_t *ADC_Buffer);

#endif /* HARDWARE_ADC_DRIVER_H_ */
