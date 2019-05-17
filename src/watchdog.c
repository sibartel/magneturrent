#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "watchdog.h"
#include "inc/hw_memmap.h"
#include "driverlib/watchdog.h"
#include "driverlib/sysctl.h"

void watchdog_init() {
    // Enable the Watchdog 0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
    // Wait for the Watchdog 0 module to be ready.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WDOG0));
    // Check to see if the registers are locked, and if so, unlock them.
    if(WatchdogLockState(WATCHDOG0_BASE) == true)
        WatchdogUnlock(WATCHDOG0_BASE);
    // Initialize the watchdog timer.
    WatchdogReloadSet(WATCHDOG0_BASE, 0xFEEFEE);
    // Enable the reset.
    WatchdogResetEnable(WATCHDOG0_BASE);
    // Enable the watchdog timer.
    WatchdogEnable(WATCHDOG0_BASE);
}

void watchdog_kick() {
    WatchdogReloadSet(WATCHDOG0_BASE, 0xFEEFEE);
}