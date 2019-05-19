#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"

#include "i2c.h"
#include "lsm303dlhc.h"
#include "watchdog.h"
#include "uart.h"
#include "heartbeat.h"

// Error handler
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void main() {
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    FPULazyStackingEnable();

    // Clock settings
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    // Enable Interrupts
    IntMasterEnable();

    heartbeat_init();
    uart_init();
    watchdog_init();

    i2cDevice_t i2c2;
    i2c_init(&i2c2);

    lsm303dlhcSensor_t sensor;
    lsm303dlhc_init(&sensor, &i2c2);
    lsm303dlhc_mag_enable(&sensor);

    while(1) {
        Lsm303dlhcMagData_t data = lsm303dlhc_mag_get(&sensor);
        uart_send((uint8_t*) &(data.x), 4 * 3);
        
        watchdog_kick();
        heartbeat_process();
    }
}
