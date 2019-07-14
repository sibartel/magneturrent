/**
 * @file uart.c
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @brief Small library for the internal uart0.
 * @version 0.1
 * @date 2019-05-19
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "uart.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"

/**
 * @brief Initializes the uart0 peripheral.
 *
 * Settings: 115200; 8bit 1 Stop; No Parity
 * 
 */
void uart_init() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}

/**
 * @brief Writes bytes referenced by data on uart0. 
 * 
 * @param data pointer to the bytes which should be written
 * @param size amount of bytes to be written
 */
void uart_send(const uint8_t* data, uint32_t size) {
    while(size--)
        UARTCharPut(UART0_BASE, *data++);
}

/**
 * @brief Uart0 interrupt handler.
 *
 * Cleares interrupt flags and reads the data.
 * 
 */
void uart_interrupt_handler() {
    uint32_t ui32Status = UARTIntStatus(UART0_BASE, true);

    UARTIntClear(UART0_BASE, ui32Status);

//    while(UARTCharsAvail(UART0_BASE))
//    {
//        //
//        // Read the next character from the UART and write it back to the UART.
//        //
//        UARTCharPutNonBlocking(UART0_BASE,
//                                   UARTCharGetNonBlocking(UART0_BASE));
//    }
}