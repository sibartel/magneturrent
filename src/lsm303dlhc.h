#pragma once

#include <stdint.h>
#include "i2c.h"

#define LSM303DLHC_ACC_ADDR 0b0011001
#define LSM303DLHC_MAG_ADDR 0b0011110

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

typedef enum {
    LSM303DLHC_MAG_GAIN_1100_980         = 0b00100000,
    LSM303DLHC_MAG_GAIN_855_760          = 0b01000000,
    LSM303DLHC_MAG_GAIN_670_600          = 0b01100000,
    LSM303DLHC_MAG_GAIN_450_400          = 0b10000000,
    LSM303DLHC_MAG_GAIN_400_355          = 0b10100000,
    LSM303DLHC_MAG_GAIN_300_295          = 0b11000000,
    LSM303DLHC_MAG_GAIN_230_205          = 0b11100000,
} Lsm303dlhcMagGain_t;

typedef struct {
    i2cDevice_t* device;
    Lsm303dlhcMagDataRate_t mag_data_rate;
    Lsm303dlhcMagGain_t mag_gain;
} lsm303dlhcSensor_t;

void lsm303dlhc_init(lsm303dlhcSensor_t* sensor, i2cDevice_t* device);
void lsm303dlhc_mag_enable(lsm303dlhcSensor_t* sensor);

void lsm303dlhc_mag_set_data_rate(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagDataRate_t rate);
void lsm303dlhc_mag_set_gain(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagGain_t gain);

void lsm303dlhc_mag_read(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint8_t* data);
void lsm303dlhc_mag_read16(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint16_t* data);
void lsm303dlhc_mag_write(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint8_t data);
