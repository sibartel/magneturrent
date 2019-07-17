/**
 * @file model.c
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @brief Model of the current sensor application.
 * @version 0.1
 * @date 2019-07-14
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "model.h"

/**
 * @brief Used to calculate the current.
 *        Depending on the distance of the cable to the sensor.
 */
#define MODEL_MAG_CURRENT_FACTOR -12100000

/**
 * @brief Threshold causing invalidation of calibration.
 */
#define MODEL_ACC_THRESHOLD 1000

int32_t abs(int32_t val) {
    if(val < 0)
        val *= -1;
    return val;
}

/**
 * @brief Initializes a instants of the application model.
 * 
 * @param model handler for the model
 */
void model_init(Model_t* model) {
    model->calibrated = MODEL_CALIBRATION_NONE;
    model->mag_offset.x = 0;
    model->mag_offset.y = 0;
    model->mag_offset.z = 0;
    model->current = 0;
}

/**
 * @brief Update the model with new magnetic data.
 * 
 * @param model handler for the model
 * @param data the new magnetic data
 */
void model_update_mag(Model_t* model, Lsm303dlhcMagData_t data) {
    if(model->calibrated == MODEL_CALIBRATION_WAITING) {
        model->mag_offset = data;
        model->calibrated = MODEL_CALIBRATION_DONE;
    } else {
        data.x -= model->mag_offset.x;
        data.y -= model->mag_offset.y;
        data.z -= model->mag_offset.z;

        model->current = ((float) data.z) / MODEL_MAG_CURRENT_FACTOR;
    }
}

/**
 * @brief Update the model with new acceleration data.
 * 
 * @param model handler for the model
 * @param data the new acceleration data
 */
void model_update_acc(Model_t* model, Lsm303dlhcAccData_t data) {
    if(abs(data.x) > MODEL_ACC_THRESHOLD || abs(data.y) > MODEL_ACC_THRESHOLD)
        model->calibrated = false;
}

/**
 * @brief Use the current data as zero calibration point.
 * 
 * @param model handler for the model
 */
void model_calibrate(Model_t* model) {
    model->calibrated = MODEL_CALIBRATION_WAITING;
}

/**
 * @brief Get the status byte of the model.
 * 
 * @param model handler for the model
 */
uint8_t model_get_status(Model_t* model) {
    return model->calibrated;
}

/**
 * @brief Get the actual current calculated by the model.
 * 
 * @param model handler for the model
 */
float model_get_current(Model_t* model) {
    return model->current;
}