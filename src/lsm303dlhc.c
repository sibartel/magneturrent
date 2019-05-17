/**
 * @file lsm303dlhc.c
 * @author Silas Bartel (arvius@web.de)
 * @brief Library for the lsm303dlhc sensor.
 * @version 0.1
 * @date 2019-05-17
 * 
 */

#include "lsm303dlhc.h"
#include "i2c.h"

uint32_t Lsm303dlhcMagGain[] = {
    0,
    90909,
    116959,
    149254,
    222222,
    250000,
    333333,
    434782
};

/* Function declaration for private functions. */

void lsm303dlhc_mag_write(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint8_t data);
uint32_t lsm303dlhc_mag_get_gain_abs(lsm303dlhcSensor_t* sensor);


/* Public function definitions */

/**
 * @brief Inizializes a lsm303dlhc sensor connected to the given i2c device.
 * 
 * @param sensor handler for the sensor, needed to read and write to this sensor
 * @param device i2c device the sensor is connected to
 */
void lsm303dlhc_init(lsm303dlhcSensor_t* sensor, i2cDevice_t* device) {
    sensor->device = device;
    lsm303dlhc_mag_set_data_rate(sensor, LSM303DLHC_MAG_DATA_RATE_220);
    lsm303dlhc_mag_set_gain(sensor, LSM303DLHC_MAG_GAIN_1100_980);
}

/**
 * @brief Enables the magnetic subsystem for a given lsm303dlhc sensor.
 * 
 * @param sensor handler to the sensor
 */
void lsm303dlhc_mag_enable(lsm303dlhcSensor_t* sensor) {
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_MR_REG_M, 0x00);
}

/**
 * @brief Sets the data rate for the magnetic subsystem of a given lsm303dlhc sensor.
 * 
 * @param sensor handler to the sensor
 * @param rate data rate which should be configured
 */
void lsm303dlhc_mag_set_data_rate(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagDataRate_t rate) {
    sensor->mag_data_rate = rate;
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_CRA_REG_M, rate);
}

/**
 * @brief Returns the data rate for the magnetic subsystem of a given lsm303dlhc sensor.
 * 
 * @param sensor handler to the sensor
 * @return Lsm303dlhcMagDataRate_t the current data rate
 */
Lsm303dlhcMagDataRate_t lsm303dlhc_mag_get_data_rate(lsm303dlhcSensor_t* sensor) {
    return sensor->mag_data_rate;
}

/**
 * @brief Sets the gain for the magnetic subsystem of a given lsm303dlhc sensor.
 * 
 * @param sensor handler to the sensor
 * @param gain gain which should be configured
 */
void lsm303dlhc_mag_set_gain(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagGain_t gain) {
    sensor->mag_gain = gain;
    lsm303dlhc_mag_write(sensor, LSM303DLHC_MAG_REGISTER_CRB_REG_M, gain);
}

/**
 * @brief Returns the gain for the magnetic subsystem of a given lsm303dlhc sensor.
 * 
 * @param sensor handler to the sensor
 * @return Lsm303dlhcMagGain_t the current gain
 */
Lsm303dlhcMagGain_t lsm303dlhc_mag_get_gain(lsm303dlhcSensor_t* sensor) {
    return sensor->mag_gain;
}

/**
 * @brief Returns the magnetic field data of a given lsm303dlhc sensor.
 * 
 * @param sensor handler to the sensor
 * @return Lsm303dlhcMagData_t struct containing the vectors of the magnetic field in 10mG
 */
Lsm303dlhcMagData_t lsm303dlhc_mag_get(lsm303dlhcSensor_t* sensor) {
    uint8_t data_raw[3*2];
    i2c_read_reg(sensor->device, LSM303DLHC_MAG_ADDR, LSM303DLHC_MAG_REGISTER_OUT_X_H_M, (uint8_t*) &data_raw, 3*2);
    int16_t x_raw = (data_raw[0] << 8) | data_raw[1];
    int16_t y_raw = (data_raw[4] << 8) | data_raw[5];
    int16_t z_raw = (data_raw[2] << 8) | data_raw[3];
    int32_t x = x_raw * lsm303dlhc_mag_get_gain_abs(sensor);
    int32_t y = y_raw * lsm303dlhc_mag_get_gain_abs(sensor);
    int32_t z = z_raw * lsm303dlhc_mag_get_gain_abs(sensor);
    Lsm303dlhcMagData_t data = {
        .x = x,
        .y = y,
        .z = z
    };
    return data;
}


/* Private Functions */

/**
 * @brief Returns the absolute gain of the magnetic subsystem of a given lsm303dlhc sensor.
 *
 * Use this gain to convert the signed 12bit raw value of the sensor into 10mG int32_t value.
 * 
 * @param sensor handler to the sensor
 * @return uint32_t the absolute gain
 */
uint32_t lsm303dlhc_mag_get_gain_abs(lsm303dlhcSensor_t* sensor) {
    return Lsm303dlhcMagGain[lsm303dlhc_mag_get_gain(sensor) >> 5];
}

/**
 * @brief Writes data into register of the magnetic subsystem of a given lsm303dlhc sensor.
 * 
 * @param sensor handler to the sensor
 * @param reg register the data is written to
 * @param data data
 */
void lsm303dlhc_mag_write(lsm303dlhcSensor_t* sensor, Lsm303dlhcMagRegisters_t reg, uint8_t data) {
    i2c_write_reg(sensor->device, LSM303DLHC_MAG_ADDR, reg, &data, 1);
}
