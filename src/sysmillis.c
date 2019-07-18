/**
 * @file sysmillis.c
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @author Martin Bauer
 * @author Moritz Amann
 * @brief Small library to get system millis since startup.
 * @version 0.1
 * @date 2019-07-17
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "sysmillis.h"
#include "inc/hw_memmap.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

static volatile uint32_t sysmillis = 0;
static bool sysmillis_initialized = false;
void sysmillis_handler();

/**
 * @brief Initializes the systick.
 * 
 */
void sysmillis_init() {
    if(sysmillis_initialized)
        return;

    SysTickPeriodSet(16000);
    SysTickIntRegister(sysmillis_handler);
    SysTickIntEnable();
    SysTickEnable();

    sysmillis_initialized = true;
}

/**
 * @brief Returns the system millis since startup.
 * 
 */
uint32_t sysmillis_get() {
    return sysmillis;
}

/**
 * @brief Internal handler gets called if systick overflows.
 * 
 */
void sysmillis_handler() {
    sysmillis++;
}