#include "Hardware/ADC_Driver.h"
#include "Hardware/CS_Driver.h"
#include "Hardware/TIMERA_Driver.h"
#include "Structures/queue.h"
#include "Structures/message.h"

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
     128,                                   /*Period of the PWM signal, CAUTION! this is based on 64 KHz signal frequency*/
     TIMER_A_CAPTURECOMPARE_REGISTER_3,
     TIMER_A_OUTPUTMODE_RESET_SET,
     64                                      /*Duty Cycle of the PWM signal, you can find the duty cycle percentage by dividing this number by the period*/
};

Timer_A_UpModeConfig UpModeConfig =
{
     TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
     TIMER_A_CLOCKSOURCE_DIVIDER_1,
     1280,
     TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
     TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
     TIMER_A_DO_CLEAR                        // Clear value
};


uint16_t ADC_Buffer[5];
float Duty_Cycle[3];

void EventCallback(uint32_t Event)
{
    switch(Event)
    {
    case ADC_SAMPLE_READY:
        break;
    case TIMER_A1_EVENT:
        ADC_ReadMultiple(ADC_Buffer);
        Duty_Cycle[0] = 0.017*ADC_Buffer[0] + 27.0;
        TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig0, (uint32_t)Duty_Cycle[0]);
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
        break;
    case IDLE_MESSAGE:
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        break;
    }
}

void main(void)
{
    MAP_WDT_A_holdTimer();
    CS_PWM_Init();

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);

    /*Enable ADC Module*/
    ADC_Enable(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1);
    ADC_InitMultiple(GPIO_PIN5|GPIO_PIN4|GPIO_PIN2, ADC_MEM0, ADC_MEM2, 3);

    /*Setup PWMs*/
    TIMERA_Init(TIMER_A0_BASE, PWM_MODE, &PWMConfig0, GPIO_PIN4);
    TIMERA_Init(TIMER_A0_BASE, PWM_MODE, &PWMConfig1, GPIO_PIN5);
    TIMERA_Init(TIMER_A0_BASE, PWM_MODE, &PWMConfig2, GPIO_PIN6);

    /*Setup Sampling Timer*/
    TIMERA_Init(TIMER_A1_BASE, UP_MODE, &UpModeConfig, 0);

    /*Setup Hardware callbacks*/
    ADC_RegisterCallback(Hardware_Callback);
    TIMERA_RegisterCallback(TIMER_A1_BASE, Hardware_Callback);

    TIMERA_StartTimer(TIMER_A1_BASE, TIMER_A_UP_MODE);

    /*Initialize servo motor positions*/
    TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig0, 128);
    TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig1, 96);
    TIMERA_DutyCycle(TIMER_A0_BASE, &PWMConfig2, 64);

    /*Main message dispatcher callback register*/
    RegisterMessageCallback(EventCallback);

    Interrupt_enableMaster();
    uint32_t Msg;

    while(1)
    {
       Msg = GetMessage();
       DispatchMessage(Msg);
    }
}


