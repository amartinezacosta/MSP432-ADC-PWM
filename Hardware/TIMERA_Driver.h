#ifndef HARDWARE_TIMERA_DRIVER_H_
#define HARDWARE_TIMERA_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/timer_a.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>

#define CONTINUOS_MODE  0
#define UP_MODE         1
#define UPDOWN_MODE     2
#define CAPTURE_MODE    3
#define COMPARE_MODE    4
#define PWM_MODE        5

#define TIMER_A0_EVENT  10
#define TIMER_A1_EVENT  11

void TIMERA_Init(uint32_t TIMER, uint32_t Mode, void *Config, uint32_t Pins);
void TIMERA_DutyCycle(uint32_t TIMER, Timer_A_PWMConfig *pwm, uint32_t DutyCycle);
void TIMERA_RegisterCallback(uint32_t TIMER, void(*TimerCallback)(uint32_t Event));
void TIMERA_StartTimer(uint32_t TIMER, uint32_t Mode);

#endif /* HARDWARE_TIMERA_DRIVER_H_ */
