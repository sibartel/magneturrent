#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "extint.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

static void (*extint_handler)() = NULL;

void extint_init() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
}

void extint_register_handler(void (*handler)()) {
    extint_handler = handler;
}

void gpio_interrupt_handler() {
    if(extint_handler)
        extint_handler();
}