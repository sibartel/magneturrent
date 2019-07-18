/**
 * @file i2c.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @brief Small library for the internal i2c.
 * @version 0.1
 * @date 2019-07-17
 * 
 * If you search for the documentation,
 * have a look at the source file.
 *
 */

#pragma once

typedef struct {
    uint32_t base;
} i2cDevice_t;

void i2c_init(i2cDevice_t* device);
void i2c_write_reg(i2cDevice_t* device, uint8_t addr, uint8_t reg, uint8_t* data, uint8_t size);
void i2c_read_reg(i2cDevice_t* device, uint8_t addr, uint8_t reg, uint8_t* data, uint8_t size);
void i2c_busy_blocking(i2cDevice_t* device);
void i2c_check_error(i2cDevice_t* device);
