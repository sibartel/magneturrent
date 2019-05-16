/**
Library for the lsm303dlhc sensor.
**/

#include "lsm303dlhc.h"
#include "i2c.h"

void lsm303dlhc_init(lsm303dlhcSensor_t* sensor, i2cDevice_t* device) {
    sensor->device = device;
}

void lsm303dlhc_mag_enable(lsm303dlhcSensor_t* sensor) {
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_MR_REG_M, 0x00);
}

void lsm303dlhc_mag_set_data_rate(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagDataRate_t rate) {
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_CRA_REG_M, rate);
}

void lsm303dlhc_mag_set_gain(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagGain_t gain) {
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_CRB_REG_M, gain);
}

void lsm303dlhc_mag_read(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint8_t* data) {
    i2c_read_reg(sensor->device, LSM303DLHC_MAG_ADDR, reg, data, 1);
}

void lsm303dlhc_mag_read16(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint16_t* data) {
    uint16_t data_raw;
    i2c_read_reg(sensor->device, LSM303DLHC_MAG_ADDR, reg, &data_raw, 2);
    *data = (uint16_t) ((data_raw << 8) | (data_raw >> 8));
}

void lsm303dlhc_mag_write(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint8_t data) {
    i2c_write_reg(sensor->device, LSM303DLHC_MAG_ADDR, reg, &data, 1);
}
