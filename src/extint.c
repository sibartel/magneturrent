/**
 * @file extint.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @brief Small library for external interrupt on data ready pin.
 * @version 0.1
 * @date 2019-07-18
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "extint.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

/* Callback handler is stored here */
static void (*extint_handler)() = NULL;

/**
 * @brief Initializes the pin setup.
 *
 * Pin: PF4 Trigger: FallingEdge
 * 
 */
void extint_init() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, gpio_interrupt_handler);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
}

/**
 * @brief Sets the callback handler which will be called on trigger event.
 *
 * @param handler the handler to be registered
 */
void extint_register_handler(void (*handler)()) {
    extint_handler = handler;
}

/**
 * @brief Internal used callback handler.
 * 
 */
void gpio_interrupt_handler() {
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    if(extint_handler)
        extint_handler();
}