/*
 * mcal_i2c_cfg.c
 *
 * Created: 12/22/2025 4:36:42 PM
 *  Author: conti
 */ 

#include "mcal_i2c_cfg.h"

/** @brief I2c cfg Standard Mode: 100kHz */
const Mcal_I2c_Config_t Mcal_I2c_DefaultCfg = {
	.u32BaudRate_bps = 100000UL,
	.u8Prescaler     = I2C_PRESCALER_1
};