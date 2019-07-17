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
#include "extint.h"
#include "heartbeat.h"
#include "interface.h"
#include "model.h"

// Error handler
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

lsm303dlhcSensor_t sensor;
Model_t model;

void read_data() {
    Lsm303dlhcMagData_t data = lsm303dlhc_mag_get(&sensor);
    model_update_mag(&model, data);
    interface_send(model_get_status(&model), model_get_current(&model));
}

void main() {
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    FPUEnable();
    FPULazyStackingEnable();

    // Clock settings
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    // Enable Interrupts
    IntMasterEnable();

    heartbeat_init();
    uart_init();
    watchdog_init();

    model_init(&model);

    i2cDevice_t i2c2;
    i2c_init(&i2c2);

    lsm303dlhc_init(&sensor, &i2c2);
    lsm303dlhc_mag_enable(&sensor);

    extint_init();
    extint_register_handler(read_data);

    model_calibrate(&model);

    while(1) {
        watchdog_kick();
        heartbeat_process();
    }
}
