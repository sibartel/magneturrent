/**
Library for the lsm303dlhc sensor.
**/

#include "lsm303dlhc.h"
#include "i2c.h"

uint32_t Lsm303dlhcMagGainXY[] = {
    0,
    90909,
    116959,
    149254,
    222222,
    250000,
    333333,
    434782
};
uint32_t Lsm303dlhcMagGainZ[]  = {
    0,
    102041,
    131579,
    166666,
    250000,
    281690,
    338983,
    487805
};

void lsm303dlhc_init(lsm303dlhcSensor_t* sensor, i2cDevice_t* device) {
    sensor->device = device;
    lsm303dlhc_mag_set_data_rate(sensor, LSM303DLHC_MAG_DATA_RATE_220);
    lsm303dlhc_mag_set_gain(sensor, LSM303DLHC_MAG_GAIN_1100_980);
}

void lsm303dlhc_mag_enable(lsm303dlhcSensor_t* sensor) {
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_MR_REG_M, 0x00);
}

void lsm303dlhc_mag_set_data_rate(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagDataRate_t rate) {
    sensor->mag_data_rate = rate;
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_CRA_REG_M, rate);
}

Lsm303dlhcMagDataRate_t lsm303dlhc_mag_get_data_rate(lsm303dlhcSensor_t* sensor) {
    return sensor->mag_data_rate;
}

void lsm303dlhc_mag_set_gain(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagGain_t gain) {
    sensor->mag_gain = gain;
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_CRB_REG_M, gain);
}

Lsm303dlhcMagGain_t lsm303dlhc_mag_get_gain(lsm303dlhcSensor_t* sensor) {
    return sensor->mag_gain;
}

uint32_t lsm303dlhc_mag_get_gain_xy(lsm303dlhcSensor_t* sensor) {
    return Lsm303dlhcMagGainXY[lsm303dlhc_mag_get_gain(sensor) >> 5];
}

uint32_t lsm303dlhc_mag_get_gain_z(lsm303dlhcSensor_t* sensor) {
    return Lsm303dlhcMagGainZ[lsm303dlhc_mag_get_gain(sensor) >> 5];
}

Lsm303dlhcMagData_t lsm303dlhc_mag_get(lsm303dlhcSensor_t* sensor) {
    uint8_t data_raw[3*2];
    i2c_read_reg(sensor->device, LSM303DLHC_MAG_ADDR, LSM303DLHC_MAG_REGISTER_OUT_X_H_M, (uint8_t*) &data_raw, 3*2);
    int16_t x_raw = (data_raw[0] << 8) | data_raw[1];
    int16_t y_raw = (data_raw[4] << 8) | data_raw[5];
    int16_t z_raw = (data_raw[2] << 8) | data_raw[3];
    int32_t x = x_raw * lsm303dlhc_mag_get_gain_xy(sensor);
    int32_t y = y_raw * lsm303dlhc_mag_get_gain_xy(sensor);
    int32_t z = ((int32_t) z_raw) * lsm303dlhc_mag_get_gain_xy(sensor);
    Lsm303dlhcMagData_t data = {
        .x = x,
        .y = y,
        .z = z
    };
    return data;
}

void lsm303dlhc_mag_read(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint8_t* data) {
    i2c_read_reg(sensor->device, LSM303DLHC_MAG_ADDR, reg, data, 1);
}

void lsm303dlhc_mag_read16(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint16_t* data) {
    uint16_t data_raw;
    i2c_read_reg(sensor->device, LSM303DLHC_MAG_ADDR, reg, (uint8_t*) &data_raw, 2);
    *data = (uint16_t) ((data_raw << 8) | (data_raw >> 8));
}

void lsm303dlhc_mag_write(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint8_t data) {
    i2c_write_reg(sensor->device, LSM303DLHC_MAG_ADDR, reg, &data, 1);
}
