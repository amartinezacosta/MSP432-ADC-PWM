#include "Hardware/ADC_Driver.h"
#include "Hardware/CS_Driver.h"
#include "Hardware/TIMERA_Driver.h"

Timer_A_PWMConfig PWMConfig =
{
     TIMER_A_CLOCKSOURCE_SMCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     1280,                                   /*Period of the PWM signal, CAUTION! this is based on 64 KHz signal frequency*/
     TIMER_A_CAPTURECOMPARE_REGISTER_1,
     TIMER_A_OUTPUTMODE_RESET_SET,
     64                                      /*Duty Cycle of the PWM signal, you can find the duty cycle percentage by dividing this number by the period*/
};

void main(void)
{
    MAP_WDT_A_holdTimer();
    CS_Init();
    CS_PWM_Init();
    //ADC_Init();
    ADC_InitMultiple();

    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();

    TIMERA_Init(TIMER_A0_BASE, PWM_MODE, &PWMConfig);

    TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig, 128);
    MAP_Interrupt_enableMaster();

    /*Pointer to global ADC Buffer*/
    uint16_t *ADC_Buffer;
    float Duty_Cycle;

    while(1)
    {
       /*Wait for ADC Samples, read them when returning*/
       ADC_WaitSample();
       ADC_Buffer = ADC_GetBuffer();

       /*Change Duty cycle to motors accordingly*/
       Duty_Cycle = 0.004*ADC_Buffer[0] + 64.0;
       TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig, (uint32_t)Duty_Cycle);
    }
}
