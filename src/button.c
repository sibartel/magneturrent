/**
 * @file button.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @brief Small library for button of eval board.
 * @version 0.1
 * @date 2019-07-18
 * 
 * If you search for the documentation,
 * have a look at the source file.
 *
 */
 
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "button.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

/* Callbach handle is stored here */
static void (*button_handler)() = NULL;
void button_interrupt_handler();

/**
 * @brief Initializes the pin setup.
 *
 * Pin: PM0 Button: SW1
 * 
 */
void button_init() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));

    GPIOPinTypeGPIOInput(GPIO_PORTM_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTM_BASE , GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntRegister(GPIO_PORTM_BASE, button_interrupt_handler);
    GPIOIntEnable(GPIO_PORTM_BASE, GPIO_INT_PIN_0);
}

/**
 * @brief Sets the callback handler which will be called on trigger event.
 *
 * @param handler the handler to be registered
 */
void button_register_handler(void (*handler)()) {
    button_handler = handler;
}

/**
 * @brief Internal used callback handler.
 * 
 */
void button_interrupt_handler() {
    GPIOIntClear(GPIO_PORTM_BASE, GPIO_INT_PIN_0);
    if(button_handler)
        button_handler();
}