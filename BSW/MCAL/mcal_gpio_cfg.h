/*
 * mcal_gpio_cfg.h
 *
 * Created: 12/21/2025 3:58:14 PM
 *  Author: conti
 */ 


#ifndef MCAL_GPIO_CFG_H_
#define MCAL_GPIO_CFG_H_

#include "mcal_gpio.h"

/* --- Relay Configurations --- */

/** @brief Relay 1 - Heater control */
const stMcal_Gpio_Config_t stMcal_stRelayHeater = {
	.pu8Port  = &PORTB,
	.pu8Ddr   = &DDRB,
	.pu8Pin   = &PINB,
	.u8PinNum = 0U    /* Digital Pin 8 */
};

/** @brief Relay 2 - Cooling or Fan control */
const stMcal_Gpio_Config_t stMcal_stRelayCooler = {
	.pu8Port  = &PORTB,
	.pu8Ddr   = &DDRB,
	.pu8Pin   = &PINB,
	.u8PinNum = 1U    /* Digital Pin 9 */
};

/* --- Sensor Data Pins (One-Wire) --- */

/** @brief Data line for the 3 DS18B20 sensors */
const stMcal_Gpio_Config_t stMcal_stDs18b20Bus = {
	.pu8Port  = &PORTD,
	.pu8Ddr   = &DDRD,
	.pu8Pin   = &PIND,
	.u8PinNum = 2U    /* Digital Pin 2 */
};

/* --- User Interface --- */

/** @brief Rotary Encoder Switch (Button) */
const stMcal_Gpio_Config_t stMcal_stEncoderSw = {
	.pu8Port  = &PORTC,
	.pu8Ddr   = &DDRC,
	.pu8Pin   = &PINC,
	.u8PinNum = 0U    /* Analog Pin A0 used as Digital */
};

/** @brief Onboard LED for Arduino Nano (PB5 / D13) */
const stMcal_Gpio_Config_t stMcal_stOnboardLed = {
	.pu8Port  = &PORTB,
	.pu8Ddr   = &DDRB,
	.pu8Pin   = &PINB,
	.u8PinNum = 5U    /* PB5 is Bit 5 */
};

#endif /* MCAL_GPIO_CFG_H_ */