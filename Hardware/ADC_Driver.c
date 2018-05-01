#include "ADC_Driver.h"

void (*ADC_Callback)(uint32_t Event) = 0x00000000;
uint16_t ADC_Buffer[ADC_BUFFER_SIZE];

void ADC_Enable(uint32_t ClockSource, uint32_t Predivider, uint32_t Divider)
{
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);
}

void ADC_InitSingle(uint32_t Pin, uint32_t ADC_Mem)
{
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, Pin, GPIO_TERTIARY_MODULE_FUNCTION);
    ADC14_configureSingleSampleMode(ADC_Mem, false);

    switch(Pin)
    {
    case GPIO_PIN5:
        ADC14_configureConversionMemory(ADC_Mem, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);
        break;
    case GPIO_PIN4:
        ADC14_configureConversionMemory(ADC_Mem, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1, false);
        break;
    case GPIO_PIN3:
        ADC14_configureConversionMemory(ADC_Mem, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A2, false);
        break;
    case GPIO_PIN2:
        ADC14_configureConversionMemory(ADC_Mem, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A3, false);
        break;
    }

    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    ADC14_enableConversion();
}

uint16_t ADC_Read(uint32_t ADC_Mem)
{
    uint16_t ADC_Sample;

    ADC14_toggleConversionTrigger();
    while(ADC14_isBusy());
    ADC_Sample = ADC14_getResult(ADC_Mem);

    return ADC_Sample;
}

void ADC_InitMultiple(uint32_t Pins, uint32_t Start, uint32_t End, uint32_t PinCount)
{
    /* Configuring GPIOs for Analog In */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, Pins, GPIO_TERTIARY_MODULE_FUNCTION);

    /*Configure ADC Memory*/
    ADC14_configureMultiSequenceMode(Start, End, false);
    uint32_t i;
    for(i = 0; i < PinCount; i++)
    {
        ADC14_configureConversionMemory(i << 1, ADC_VREFPOS_INTBUF_VREFNEG_VSS, i, false);
    }

    /* Setting up the sample timer to automatically step through the sequence convert.*/
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /*Enable ADC Conversion*/
    ADC14_enableConversion();
}

void ADC_ReadMultiple(uint16_t *ADC_Buffer)
{
    ADC14_toggleConversionTrigger();

    while(ADC14_isBusy());
    ADC14_getMultiSequenceResult(ADC_Buffer);
}

uint16_t *ADC_GetBuffer(void)
{
    return ADC_Buffer;
}

void ADC_RegisterCallback(void(*Callback)(uint32_t Event))
{
    ADC_Callback = Callback;
}

void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (status & ADC_INT4)
    {
        MAP_ADC14_getMultiSequenceResult(ADC_Buffer);
        if(ADC_Callback)
        {
            ADC_Callback(ADC_SAMPLE_READY);
        }
    }
}

