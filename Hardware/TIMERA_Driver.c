#include "TIMERA_Driver.h"

void (*TIMER_A0_CB)(void) = 0x0000000;
void (*TIMER_A1_CB)(void) = 0x0000000;

void TIMERA_Init(uint32_t TIMER, uint32_t Mode, void *Config, uint32_t Pins)
{
    switch(Mode)
    {
    case CONTINUOS_MODE:
        MAP_Timer_A_configureContinuousMode(TIMER, (Timer_A_ContinuousModeConfig*)Config);
        break;
    case UP_MODE:
        MAP_Timer_A_configureUpMode(TIMER, (Timer_A_UpModeConfig*)Config);
        break;
    case UPDOWN_MODE:
        MAP_Timer_A_configureUpDownMode(TIMER, (Timer_A_UpDownModeConfig*)Config);
        break;
    case CAPTURE_MODE:
        MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, Pins,
                    GPIO_PRIMARY_MODULE_FUNCTION);
        MAP_Timer_A_initCapture(TIMER, (Timer_A_CaptureModeConfig*)&Config);
        break;
    case COMPARE_MODE:
        MAP_Timer_A_initCompare(TIMER, (Timer_A_CompareModeConfig*)Config);
        break;
    case PWM_MODE:
        MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, Pins,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
        MAP_Timer_A_generatePWM(TIMER, (Timer_A_PWMConfig*)Config);
        break;
    default:
        break;
    }

    switch(TIMER)
    {
    case TIMER_A0_BASE:
        MAP_Interrupt_enableInterrupt(INT_TA0_0);
        break;
    case TIMER_A1_BASE:
        MAP_Interrupt_enableInterrupt(INT_TA1_0);
        break;
    }
}

void TIMERA_RegisterCallback(uint32_t TIMER, void(*TimerCallback)(void))
{
    if(TimerCallback)
    {
        switch(TIMER)
        {
        case TIMER_A0_BASE:
            TIMER_A0_CB = TimerCallback;
            break;
        case TIMER_A1_BASE:
            TIMER_A1_CB = TimerCallback;
            break;
        default:
            break;
        }
    }
}

void TIMERA_StartTimer(uint32_t TIMER, uint32_t Mode)
{
    switch(TIMER)
    {
    case TIMER_A0_BASE:
        MAP_Timer_A_startCounter(TIMER, Mode);
        break;
    case TIMER_A1_BASE:
        MAP_Timer_A_startCounter(TIMER, Mode);
        break;
    }
}

void TIMERA_DutyCycle(uint32_t TIMER, Timer_A_PWMConfig *pwm, uint32_t DutyCycle)
{
    pwm->dutyCycle = DutyCycle;
    MAP_Timer_A_generatePWM(TIMER, pwm);
}

void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
                   TIMER_A_CAPTURECOMPARE_REGISTER_0);
    if(TIMER_A0_CB)
    {
        TIMER_A0_CB();
    }
}

void TA1_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
               TIMER_A_CAPTURECOMPARE_REGISTER_0);
    if(TIMER_A1_CB)
    {
        TIMER_A1_CB();
    }
}

