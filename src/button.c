#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "button.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

static void (*button_handler)() = NULL;
void button_interrupt_handler();

void button_init() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));

    GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTM_BASE , GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntRegister(GPIO_PORTM_BASE, button_interrupt_handler);
    GPIOIntEnable(GPIO_PORTM_BASE, GPIO_INT_PIN_0);
}

void button_register_handler(void (*handler)()) {
    button_handler = handler;
}

void button_interrupt_handler() {
    GPIOIntClear(GPIO_PORTM_BASE, GPIO_INT_PIN_0);
    if(button_handler)
        button_handler();
}