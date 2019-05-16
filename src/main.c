//*****************************************************************************
//
// uart_echo.c - Example for reading data from and writing data to the UART in
//               an interrupt driven fashion.
//
// Copyright (c) 2011-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the DK-TM4C123G Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "i2c.h"
#include "lsm303dlhc.h"

#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>UART Echo (uart_echo)</h1>
//!
//! This example application utilizes the UART to echo text.  The first UART
//! (connected to the USB debug virtual serial port on the evaluation board)
//! will be configured in 115,200 baud, 8-n-1 mode.  All characters received on
//! the UART are transmitted back to the UART.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void
UARTIntHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrrupt status.
    //
    ui32Status = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
//    while(UARTCharsAvail(UART0_BASE))
//    {
//        //
//        // Read the next character from the UART and write it back to the UART.
//        //
//        UARTCharPutNonBlocking(UART0_BASE,
//                                   UARTCharGetNonBlocking(UART0_BASE));
//    }
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        UARTCharPut(UART0_BASE, *pui8Buffer++);
    }
}

void PrintNInt(int16_t val, char *pointer, uint8_t n) {
  uint16_t printedChars = 0;
  uint8_t negative = 0;
  if(val < 0) {
    negative = 1;
    val = -val;
  }
  while (printedChars < n) {
    if (val == 0) {
      if(negative) {
        pointer[n - ++printedChars] = '-';
        negative = 0;
      } else {
        pointer[n - ++printedChars] = ' ';
      }
    } else {
      int lastDigit = val % 10;
      pointer[n - ++printedChars] = (char) (lastDigit + '0');
      val /= 10;
    }
  }
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int
main(void)
{
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //
    // Prompt for text to be entered.
    //
    UARTSend((uint8_t *)"\n\rHi from your tivaboard.", 24);

    i2cDevice_t i2c2;
    i2c_init(&i2c2);

    lsm303dlhcSensor_t sensor;
    lsm303dlhc_init(&sensor, &i2c2);
    lsm303dlhc_mag_enable(&sensor);
    lsm303dlhc_mag_set_data_rate(&sensor, LSM303DLHC_MAG_DATA_RATE_220);
    lsm303dlhc_mag_set_gain(&sensor, LSM303DLHC_MAG_GAIN_670_600);
    uint8_t msg[] = "\n\rZ MAG:                  ";
    uint16_t x = 42;

    volatile int32_t ui32Loop;
    while(1) {
        GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_PIN_2);
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++);

        lsm303dlhc_mag_read16(&sensor, LSM303DLHC_MAG_REGISTER_OUT_X_H_M, &x);
        lsm303dlhc_mag_read16(&sensor, LSM303DLHC_MAG_REGISTER_OUT_Y_H_M, &x);
        lsm303dlhc_mag_read16(&sensor, LSM303DLHC_MAG_REGISTER_OUT_Z_H_M, &x);
        PrintNInt(x, (char *) (msg + 14), 5);
        UARTSend(msg, 27);

        GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0);
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++);
    }
}
