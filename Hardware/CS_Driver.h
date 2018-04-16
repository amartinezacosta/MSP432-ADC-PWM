#ifndef HARDWARE_CS_DRIVER_H_
#define HARDWARE_CS_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/cs.h>
#include <ti/devices/msp432p4xx/driverlib/pcm.h>
#include <ti/devices/msp432p4xx/driverlib/flash.h>
#include <ti/devices/msp432p4xx/driverlib/wdt_a.h>
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>

void CS_Init(void);
void CS_PWM_Init(void);

#endif /* HARDWARE_CS_DRIVER_H_ */
