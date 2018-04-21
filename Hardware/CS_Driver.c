#include "CS_Driver.h"

void CS_Init(void)
{
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);
}

void CS_PWM_Init(void)
{
    MAP_CS_setReferenceOscillatorFrequency(CS_REFO_128KHZ);
    MAP_CS_initClockSignal(CS_SMCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_2);
}


