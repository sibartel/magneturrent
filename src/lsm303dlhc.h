/**
 * @file lsm303dlhc.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @author Martin Bauer
 * @author Moritz Amann
 * @brief Library for the lsm303dlhc sensor.
 * @version 0.1
 * @date 2019-05-17
 * 
 * If you search for the documentation,
 * have a look at the source file.
 *
 */

#pragma once

#include <stdint.h>
#include "i2c.h"

/* 7bit i2c addresses of the acceleration and magnetic subsystem */
#define LSM303DLHC_ACC_ADDR 0b0011001
#define LSM303DLHC_MAG_ADDR 0b0011110

/**
 * @brief The registers of the acceleration subsystem.
 */
typedef enum {
    LSM303DLHC_ACC_REGISTER_CTRL_REG1_A         = 0x20,
    LSM303DLHC_ACC_REGISTER_CTRL_REG2_A         = 0x21,
    LSM303DLHC_ACC_REGISTER_CTRL_REG3_A         = 0x22,
    LSM303DLHC_ACC_REGISTER_CTRL_REG4_A         = 0x23,
    LSM303DLHC_ACC_REGISTER_CTRL_REG5_A         = 0x24,
    LSM303DLHC_ACC_REGISTER_CTRL_REG6_A         = 0x25,
    LSM303DLHC_ACC_REGISTER_REFERENCE_A         = 0x26,
    LSM303DLHC_ACC_REGISTER_STATUS_REG_A        = 0x27,
    LSM303DLHC_ACC_REGISTER_OUT_X_L_A           = 0x28,
    LSM303DLHC_ACC_REGISTER_OUT_X_H_A           = 0x29,
    LSM303DLHC_ACC_REGISTER_OUT_Y_L_A           = 0x2A,
    LSM303DLHC_ACC_REGISTER_OUT_Y_H_A           = 0x2B,
    LSM303DLHC_ACC_REGISTER_OUT_Z_L_A           = 0x2C,
    LSM303DLHC_ACC_REGISTER_OUT_Z_H_A           = 0x2D,
    LSM303DLHC_ACC_REGISTER_FIFO_CTRL_REG_A     = 0x2E,
    LSM303DLHC_ACC_REGISTER_FIFO_SRC_REG_A      = 0x2F,
    LSM303DLHC_ACC_REGISTER_INT1_CFG_A          = 0x30,
    LSM303DLHC_ACC_REGISTER_INT1_SOURCE_A       = 0x31,
    LSM303DLHC_ACC_REGISTER_INT1_THS_A          = 0x32,
    LSM303DLHC_ACC_REGISTER_INT1_DURATION_A     = 0x33,
    LSM303DLHC_ACC_REGISTER_INT2_CFG_A          = 0x34,
    LSM303DLHC_ACC_REGISTER_INT2_SOURCE_A       = 0x35,
    LSM303DLHC_ACC_REGISTER_INT2_THS_A          = 0x36,
    LSM303DLHC_ACC_REGISTER_INT2_DURATION_A     = 0x37,
    LSM303DLHC_ACC_REGISTER_CLICK_CFG_A         = 0x38,
    LSM303DLHC_ACC_REGISTER_CLICK_SRC_A         = 0x39,
    LSM303DLHC_ACC_REGISTER_CLICK_THS_A         = 0x3A,
    LSM303DLHC_ACC_REGISTER_TIME_LIMIT_A        = 0x3B,
    LSM303DLHC_ACC_REGISTER_TIME_LATENCY_A      = 0x3C,
    LSM303DLHC_ACC_REGISTER_TIME_WINDOW_A       = 0x3D
} Lsm303dlhcAccRegisters_t;

/**
 * @brief The registers of the magnetic subsystem.
 */
typedef enum {
    LSM303DLHC_MAG_REGISTER_CRA_REG_M    = 0x00,
    LSM303DLHC_MAG_REGISTER_CRB_REG_M    = 0x01,
    LSM303DLHC_MAG_REGISTER_MR_REG_M     = 0x02,
    LSM303DLHC_MAG_REGISTER_OUT_X_H_M    = 0x03,
    LSM303DLHC_MAG_REGISTER_OUT_X_L_M    = 0x04,
    LSM303DLHC_MAG_REGISTER_OUT_Z_H_M    = 0x05,
    LSM303DLHC_MAG_REGISTER_OUT_Z_L_M    = 0x06,
    LSM303DLHC_MAG_REGISTER_OUT_Y_H_M    = 0x07,
    LSM303DLHC_MAG_REGISTER_OUT_Y_L_M    = 0x08,
    LSM303DLHC_MAG_REGISTER_SR_REG_Mg    = 0x09,
    LSM303DLHC_MAG_REGISTER_IRA_REG_M    = 0x0A,
    LSM303DLHC_MAG_REGISTER_IRB_REG_M    = 0x0B,
    LSM303DLHC_MAG_REGISTER_IRC_REG_M    = 0x0C,
    LSM303DLHC_MAG_REGISTER_TEMP_OUT_H_M = 0x31,
    LSM303DLHC_MAG_REGISTER_TEMP_OUT_L_M = 0x32
} Lsm303dlhcMagRegisters_t;

/**
 * @brief Supported data rates for the magnetic subsystem.
 */
typedef enum {
    LSM303DLHC_MAG_DATA_RATE_220         = 0b00011100,
    LSM303DLHC_MAG_DATA_RATE_75          = 0b00011000,
    LSM303DLHC_MAG_DATA_RATE_30          = 0b00010100,
    LSM303DLHC_MAG_DATA_RATE_15          = 0b00010000,
    LSM303DLHC_MAG_DATA_RATE_7_5         = 0b00001100,
    LSM303DLHC_MAG_DATA_RATE_3_0         = 0b00001000,
    LSM303DLHC_MAG_DATA_RATE_1_5         = 0b00000100,
    LSM303DLHC_MAG_DATA_RATE_0_75        = 0b00000000
} Lsm303dlhcMagDataRate_t;

/**
 * @brief Supported gain settings for the magnetic subsystem.
 */
typedef enum {
    LSM303DLHC_MAG_GAIN_1100_980         = 0b00100000,
    LSM303DLHC_MAG_GAIN_855_760          = 0b01000000,
    LSM303DLHC_MAG_GAIN_670_600          = 0b01100000,
    LSM303DLHC_MAG_GAIN_450_400          = 0b10000000,
    LSM303DLHC_MAG_GAIN_400_355          = 0b10100000,
    LSM303DLHC_MAG_GAIN_300_295          = 0b11000000,
    LSM303DLHC_MAG_GAIN_230_205          = 0b11100000
} Lsm303dlhcMagGain_t;

// in 10nG
typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} Lsm303dlhcMagData_t;

// in ?
typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} Lsm303dlhcAccData_t;

typedef struct {
    i2cDevice_t* device;
    Lsm303dlhcMagDataRate_t mag_data_rate;
    Lsm303dlhcMagGain_t mag_gain;
} lsm303dlhcSensor_t;

void lsm303dlhc_init(lsm303dlhcSensor_t* sensor, i2cDevice_t* device);
void lsm303dlhc_mag_enable(lsm303dlhcSensor_t* sensor);
void lsm303dlhc_acc_enable(lsm303dlhcSensor_t* sensor);


void lsm303dlhc_mag_set_data_rate(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagDataRate_t rate);
Lsm303dlhcMagDataRate_t lsm303dlhc_mag_get_data_rate(lsm303dlhcSensor_t* sensor);
void lsm303dlhc_mag_set_gain(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagGain_t gain);
Lsm303dlhcMagGain_t lsm303dlhc_mag_get_gain(lsm303dlhcSensor_t* sensor);

Lsm303dlhcMagData_t lsm303dlhc_mag_get(lsm303dlhcSensor_t* sensor);
Lsm303dlhcAccData_t lsm303dlhc_acc_get(lsm303dlhcSensor_t* sensor);
