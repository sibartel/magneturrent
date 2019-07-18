/**
 * @file i2c.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @author Martin Bauer
 * @author Moritz Amann
 * @brief Small library for the internal i2c.
 * @version 0.1
 * @date 2019-07-17
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

/**
 * @brief Initializes the i2c peripheral
 * 
 */
void i2c_init(i2cDevice_t* device) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C5);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C5);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C5) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOG));
 
    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PG6_I2C5SCL);
    GPIOPinConfigure(GPIO_PG7_I2C5SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTG_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTG_BASE, GPIO_PIN_7);
 
    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C5_BASE, SysCtlClockGet(), false);

    //clear I2C FIFOs
    HWREG(I2C5_BASE + I2C_O_FIFOCTL) = 80008000;

    device->base = I2C5_BASE;
}

/**
 * @brief Writes given amount of data to specific register of i2c device.
 *
 * @param device handler of the i2c device
 * @param addr i2c addres of device
 * @param reg target register of device
 * @param data pointer to data to be written
 * @param size amount of data to be written
 */
void i2c_write_reg(i2cDevice_t* device, uint8_t addr, uint8_t reg, uint8_t* data, uint8_t size) {
    I2CMasterSlaveAddrSet(device->base, addr, false);
    
    // Select reg we want to write to
    I2CMasterDataPut(device->base, reg);
    I2CMasterControl(device->base, I2C_MASTER_CMD_BURST_SEND_START);
    i2c_busy_blocking(device);
    
    uint8_t index = 0;
    while(index < size - 1) {
        // Send data elements
        I2CMasterDataPut(device->base, data[index++]);
        I2CMasterControl(device->base, I2C_MASTER_CMD_BURST_SEND_CONT);
        i2c_busy_blocking(device);
    }

    // Send last data element
    I2CMasterDataPut(device->base, data[index]);
    I2CMasterControl(device->base, I2C_MASTER_CMD_BURST_SEND_FINISH);
    i2c_busy_blocking(device);
}

/**
 * @brief Reads given amount of data from specific register of i2c device.
 *
 * @param device handler of the i2c device
 * @param addr i2c addres of device
 * @param reg target register of device
 * @param data pointer to memory
 * @param size amount of data to be read
 */
void i2c_read_reg(i2cDevice_t* device, uint8_t addr, uint8_t reg, uint8_t* data, uint8_t size) {
    // Set mode to "write"
    I2CMasterSlaveAddrSet(device->base, addr, false);

    // Select register we want to read from
    I2CMasterDataPut(device->base, reg);
    I2CMasterControl(device->base, I2C_MASTER_CMD_BURST_SEND_START);
    i2c_busy_blocking(device);

    // Set mode to read
    I2CMasterSlaveAddrSet(device->base, addr, true);

    if(size == 1) {
        I2CMasterControl(device->base, I2C_MASTER_CMD_SINGLE_RECEIVE);
        i2c_busy_blocking(device);
        data[0] = (uint8_t) I2CMasterDataGet(device->base);
    } else {
        uint8_t index = 0;
        while(index < size - 1) {
            // Read data elements
            if(index == 0)
                I2CMasterControl(device->base, I2C_MASTER_CMD_BURST_RECEIVE_START);
            else
                I2CMasterControl(device->base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
            i2c_busy_blocking(device);
            data[index++] = (uint8_t) I2CMasterDataGet(device->base);
        }
        
        I2CMasterControl(device->base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        i2c_busy_blocking(device);
        data[index] = (uint8_t) I2CMasterDataGet(device->base);
    }
}

/**
 * @brief Blocks until device is ready.
 *
 * @param device handler of the i2c device
 */
void i2c_busy_blocking(i2cDevice_t* device) {
    while(!I2CMasterBusy(device->base));
    while(I2CMasterBusy(device->base));
    i2c_check_error(device);
}

/**
 * @brief Checks if i2c device is in an error state. If so block.
 *
 * @param device handler of the i2c device
 */
void i2c_check_error(i2cDevice_t* device) {
    if(I2CMasterErr(device->base) != I2C_MASTER_ERR_NONE) {
        GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIO_PIN_2);
        while(1);
    }
}