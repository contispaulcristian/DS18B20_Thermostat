/*
 * hal_ds18b20.h
 *
 * Created: 12/21/2025 3:29:28 PM
 *  Author: conti
 */ 


#ifndef HAL_DS18B20_H_
#define HAL_DS18B20_H_

#include "mcal_gpio.h"

/* --- DS18B20 Commands --- */
#define DS18B20_CMD_SKIP_ROM         0xCCU
#define DS18B20_CMD_CONVERT_T        0x44U
#define DS18B20_CMD_READ_SCRATCHPAD  0xBEU

/* --- Function Prototypes --- */

/**
 * @brief Initializes the 1-Wire bus for the sensor.
 * @param pstPin Pointer to the GPIO configuration.
 * @return Std_ReturnType_t E_OK if sensor is present.
 */
Std_ReturnType_t Hal_Ds18b20_Init(const Mcal_Gpio_Config_t* pstPin);

/**
 * @brief Starts temperature conversion and reads the result.
 * @param pstPin Pointer to the GPIO configuration.
 * @param pfTemp_out Pointer to store the result in Celsius.
 * @return Std_ReturnType_t E_OK if successful.
 */
//Std_ReturnType_t Hal_Ds18b20_ReadTemp(const Mcal_Gpio_Config_t* pstPin, float* pfTemp_out);




/* Step 1: Tell sensor to start measuring (Returns immediately) */
Std_ReturnType_t Hal_Ds18b20_StartConversion(const Mcal_Gpio_Config_t* pstPin);

/* Step 2: Read the result (Call this 750ms after Step 1) */
Std_ReturnType_t Hal_Ds18b20_ReadResult(const Mcal_Gpio_Config_t* pstPin, float* pfTemp_out);


#endif /* HAL_DS18B20_H_ */