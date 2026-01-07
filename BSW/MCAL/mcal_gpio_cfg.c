/*
 * mcal_gpio_cfg.c
 *
 * Created: 12/22/2025 3:57:55 PM
 *  Author: conti
 */ 

#include "mcal_gpio.h"

/* --- Relay Configurations --- */

/** @brief Relay 1 - Heater control */
const Mcal_Gpio_Config_t Mcal_RelayHeater = {
	.pu8Port  = &PORTD,
	.pu8Ddr   = &DDRD,
	.pu8Pin   = &PIND,
	.u8PinNum = 3U
};

/** @brief Relay 2 - Cooling or Fan control */
const Mcal_Gpio_Config_t Mcal_RelayCooler = {
	.pu8Port  = &PORTD,
	.pu8Ddr   = &DDRD,
	.pu8Pin   = &PIND,
	.u8PinNum = 4U
};

/* --- Sensor Data Pins (One-Wire) --- */

/** @brief Data line for the 3 DS18B20 sensors */
const Mcal_Gpio_Config_t Mcal_Ds18b20Bus = {
	.pu8Port  = &PORTD,
	.pu8Ddr   = &DDRD,
	.pu8Pin   = &PIND,
	.u8PinNum = 2U
};

/* --- User Interface --- */

/** @brief Rotary Encoder Clk pin */
const Mcal_Gpio_Config_t Mcal_EncoderClk = {
	.pu8Port  = &PORTB,
	.pu8Ddr   = &DDRB,
	.pu8Pin   = &PINB,
	.u8PinNum = 0U
};

/** @brief Rotary Encoder Dt pin */
const Mcal_Gpio_Config_t Mcal_EncoderDt = {
	.pu8Port  = &PORTB,
	.pu8Ddr   = &DDRB,
	.pu8Pin   = &PINB,
	.u8PinNum = 1U    /* Analog Pin A0 used as Digital */
};

/** @brief Rotary Encoder Switch (Button) */
const Mcal_Gpio_Config_t Mcal_EncoderSw = {
	.pu8Port  = &PORTB,
	.pu8Ddr   = &DDRB,
	.pu8Pin   = &PINB,
	.u8PinNum = 2U    /* Analog Pin A0 used as Digital */
};

/** @brief Onboard LED for Arduino Nano - for testing */
const Mcal_Gpio_Config_t Mcal_OnboardLed = {
	.pu8Port  = &PORTB,
	.pu8Ddr   = &DDRB,
	.pu8Pin   = &PINB,
	.u8PinNum = 5U    /* PB5 is Bit 5 */
};