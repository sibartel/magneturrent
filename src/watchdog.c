/**
 * @file watchdog.c
 * @author Silas Bartel (arvius@web.de)
 * @brief Small library for the internal watchdog.
 * @version 0.1
 * @date 2019-05-17
 * 
 */

#include <stdint.h>
#include <stdbool.h>

#include "watchdog.h"
#include "inc/hw_memmap.h"
#include "driverlib/watchdog.h"
#include "driverlib/sysctl.h"

/**
 * @brief Initializes the internal watchdog 0.
 *
 * To reset the countdown call watchdog_kick().
 * 
 */
void watchdog_init() {
    // Enable wathcdog peripheral and wait until ready
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WDOG0));

    // Unlock registers if locked, and kick the dog
    if(WatchdogLockState(WATCHDOG0_BASE) == true)
        WatchdogUnlock(WATCHDOG0_BASE);
    wathcdog_kick();

    // Remove muzzle and chains
    WatchdogResetEnable(WATCHDOG0_BASE);
    WatchdogEnable(WATCHDOG0_BASE);
}

/**
 * @brief Resets the watchdog countdown.
 * 
 */
void watchdog_kick() {
    WatchdogReloadSet(WATCHDOG0_BASE, 0xFEEFEE);
}