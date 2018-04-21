#include "Hardware/ADC_Driver.h"
#include "Hardware/CS_Driver.h"
#include "Hardware/TIMERA_Driver.h"

Timer_A_PWMConfig PWMConfig0 =
{
     TIMER_A_CLOCKSOURCE_SMCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     1280,                                   /*Period of the PWM signal, CAUTION! this is based on 64 KHz signal frequency*/
     TIMER_A_CAPTURECOMPARE_REGISTER_1,
     TIMER_A_OUTPUTMODE_RESET_SET,
     64                                      /*Duty Cycle of the PWM signal, you can find the duty cycle percentage by dividing this number by the period*/
};

Timer_A_PWMConfig PWMConfig1 =
{
     TIMER_A_CLOCKSOURCE_SMCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     1280,                                   /*Period of the PWM signal, CAUTION! this is based on 64 KHz signal frequency*/
     TIMER_A_CAPTURECOMPARE_REGISTER_2,
     TIMER_A_OUTPUTMODE_RESET_SET,
     64                                      /*Duty Cycle of the PWM signal, you can find the duty cycle percentage by dividing this number by the period*/
};

Timer_A_PWMConfig PWMConfig2 =
{
     TIMER_A_CLOCKSOURCE_SMCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     1280,                                   /*Period of the PWM signal, CAUTION! this is based on 64 KHz signal frequency*/
     TIMER_A_CAPTURECOMPARE_REGISTER_3,
     TIMER_A_OUTPUTMODE_RESET_SET,
     64                                      /*Duty Cycle of the PWM signal, you can find the duty cycle percentage by dividing this number by the period*/
};

void main(void)
{
    MAP_WDT_A_holdTimer();
    CS_PWM_Init();

    /*Enable ADC Module*/
    ADC_Enable(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1);
    ADC_InitMultiple(GPIO_PIN5|GPIO_PIN4|GPIO_PIN2, ADC_MEM0, ADC_MEM2, 3);

    /*Setup PWMs*/
    TIMERA_Init(TIMER_A0_BASE, PWM_MODE, &PWMConfig0, GPIO_PIN4);
    TIMERA_Init(TIMER_A0_BASE, PWM_MODE, &PWMConfig1, GPIO_PIN5);
    TIMERA_Init(TIMER_A0_BASE, PWM_MODE, &PWMConfig2, GPIO_PIN6);

    /*Initialize servo motor positions*/
    TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig0, 128);
    TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig1, 96);
    TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig2, 48);

    float Duty_Cycle[3];
    uint16_t ADC_Buffer[5];

    while(1)
    {
        /*Get ADC Samples*/
        ADC_ReadMultiple(ADC_Buffer);

       /*Change Duty cycle to motors accordingly*/
       Duty_Cycle[0] = 0.004*ADC_Buffer[0] + 64.0;
       Duty_Cycle[1] = 0.004*ADC_Buffer[1] + 64.0;
       Duty_Cycle[2] = 0.004*ADC_Buffer[3] + 64.0;

       /*Change PWM Duty cycle according to Input ADC Samples*/
       TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig0, (uint32_t)Duty_Cycle[0]);
       TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig1, (uint32_t)Duty_Cycle[1]);
       TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig2, (uint32_t)Duty_Cycle[2]);

       __delay_cycles(100000);
    }
}
