/*
 * hal_relay.h
 *
 * Created: 12/21/2025 3:28:50 PM
 *  Author: conti
 */ 


#ifndef HAL_RELAY_H_
#define HAL_RELAY_H_

#include "mcal_gpio.h"

/* --- Function Prototypes --- */
/**
 * @brief Initializes the relay pin as an output and sets it to the off state.
 * @param pstRelay Pointer to the GPIO configuration structure for the relay.
 */
void Hal_Relay_Init(const Mcal_Gpio_Config_t* pstRelay);

/**
 * @brief Turns the specified relay ON.
 * @param pstRelay Pointer to the relay configuration.
 */
void Hal_Relay_TurnOn(const Mcal_Gpio_Config_t* pstRelay);

/**
 * @brief Turns the specified relay OFF.
 * @param pstRelay Pointer to the relay configuration.
 */
void Hal_Relay_TurnOff(const Mcal_Gpio_Config_t* pstRelay);


#endif /* HAL_RELAY_H_ */