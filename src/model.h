/**
 * @file model.h
 * @author Silas Bartel (silas.a.bartel@gmail.com)
 * @brief Model of the current sensor application.
 * @version 0.1
 * @date 2019-07-14
 * 
 * If you search for the documentation,
 * have a look at the source file.
 *
 */

#pragma once

#include "lsm303dlhc.h"

typedef enum {
    MODEL_CALIBRATION_NONE,
    MODEL_CALIBRATION_WAITING,
    MODEL_CALIBRATION_DONE
} ModelCalibration_t;

typedef struct {
    ModelCalibration_t calibrated;
    Lsm303dlhcMagData_t mag_offset;
    float current;
} Model_t;

void model_init(Model_t* model);
void model_update_mag(Model_t* model, Lsm303dlhcMagData_t data);
void model_update_acc(Model_t* model, Lsm303dlhcAccData_t data);
void model_calibrate(Model_t* model);
uint8_t model_get_status(Model_t* model);
float model_get_current(Model_t* model);