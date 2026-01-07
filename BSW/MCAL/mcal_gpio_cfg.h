/*
 * mcal_gpio_cfg.h
 *
 * Created: 12/21/2025 3:58:14 PM
 *  Author: conti
 */ 


#ifndef MCAL_GPIO_CFG_H_
#define MCAL_GPIO_CFG_H_

#include "mcal_gpio.h"

/* Extern declarations for Relays */
extern const Mcal_Gpio_Config_t Mcal_RelayHeater;
extern const Mcal_Gpio_Config_t Mcal_RelayCooler;

/* Extern declarations for Sensors */
extern const Mcal_Gpio_Config_t Mcal_Ds18b20Bus;

/* Extern declarations for Encoder */
extern const Mcal_Gpio_Config_t Mcal_EncoderClk;
extern const Mcal_Gpio_Config_t Mcal_EncoderDt;
extern const Mcal_Gpio_Config_t Mcal_EncoderSw;

extern const Mcal_Gpio_Config_t Mcal_OnboardLed;

#endif /* MCAL_GPIO_CFG_H_ */