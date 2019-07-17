/**
 * @file heartbeat.c
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @brief Small library for the heartbeat of the dk-tm4c123g eval board.
 * @version 0.1
 * @date 2019-05-19
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "heartbeat.h"
#include "inc/hw_memmap.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

static volatile uint32_t heartbeat_sysmillis = 0;
void hearbeat_systick_handler();

/**
 * @brief Initializes the heartbeat led of the dk-tm4c123g eval board.
 * 
 */
void heartbeat_init() {
    // Set output for heartbeat led.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);

    // Enable SysTick Timer
    SysTickPeriodSet(1000);
    SysTickIntRegister(hearbeat_systick_handler);
    SysTickIntEnable();
    SysTickEnable();
}

/**
 * @brief Updates the state of the heartbeat.
 * 
 * Needs to be called periodically.
 * 
 */
void heartbeat_process() {
    static uint32_t last_beat = 0;
    if(heartbeat_sysmillis - last_beat > 8000) {
        last_beat = heartbeat_sysmillis;
        // Time for some beats. Funk it up.
        if(!GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_2))
            GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_PIN_2);
        else
            GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0);
    }
}

/**
 * @brief Intenral handler gets called if systick overflows.
 * 
 */
void hearbeat_systick_handler() {
    heartbeat_sysmillis++;
}