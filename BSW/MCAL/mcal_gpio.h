/*
 * mcal_gpio.h
 *
 * Created: 12/21/2025 2:41:19 PM
 *  Author: conti
 */ 


#ifndef MCAL_GPIO_H_
#define MCAL_GPIO_H_

#include "mcal_std_types.h"

/**
 * @brief Logical levels for GPIO pins.
 */
typedef enum {
    GPIO_LOW = 0U,
    GPIO_HIGH = 1U
} enMcal_Gpio_Level_t;

/**
 * @brief Configuration modes for pin direction.
 */
typedef enum {
    GPIO_INPUT = 0U,
    GPIO_OUTPUT = 1U
} enMcal_Gpio_Direction_t;

/**
 * @brief Representation of a physical GPIO pin and its associated hardware registers.
 * Includes pointers to Port, Direction, and Input registers for memory-mapped I/O.
 */
typedef struct {
    volatile uint8_t* pu8Port;  /**< Address of the Data Register (e.g., &PORTB) */
    volatile uint8_t* pu8Ddr;   /**< Address of the Data Direction Register (e.g., &DDRB) */
    volatile uint8_t* pu8Pin;   /**< Address of the Input Pins Address Register (e.g., &PINB) */
    uint8_t           u8PinNum; /**< The specific bit/pin number (0-7) on the port */
} stMcal_Gpio_Config_t;

/* --- Function Prototypes --- */

/**
 * @brief Configures a GPIO pin as either an input or an output.
 * @param pstPin Pointer to the pin configuration structure.
 * @param enDir  The desired direction (GPIO_INPUT or GPIO_OUTPUT).
 * @return E_OK if configuration was successful, E_NOT_OK if pointer is NULL.
 */
enStd_ReturnType enMcal_Gpio_Init(const stMcal_Gpio_Config_t* pstPin, enMcal_Gpio_Direction_t enDir);

/**
 * @brief Sets the logical state of an output pin.
 * @param pstPin Pointer to the pin configuration structure.
 * @param enLevel The level to write (GPIO_LOW or GPIO_HIGH).
 * @return E_OK if write was successful, E_NOT_OK if pointer is NULL.
 */
enStd_ReturnType enMcal_Gpio_Write(const stMcal_Gpio_Config_t* pstPin, enMcal_Gpio_Level_t enLevel);

/**
 * @brief Reads the current physical state of a GPIO pin.
 * @param pstPin Pointer to the pin configuration structure.
 * @return The current pin level (GPIO_HIGH or GPIO_LOW). Returns GPIO_LOW if pointer is NULL.
 */
enMcal_Gpio_Level_t enMcal_Gpio_Read(const stMcal_Gpio_Config_t* pstPin);

/**
 * @brief Inverts the current state of an output pin.
 * @param pstPin Pointer to the pin configuration structure.
 */
void vMcal_Gpio_Toggle(const stMcal_Gpio_Config_t* pstPin);


#endif /* MCAL_GPIO_H_ */