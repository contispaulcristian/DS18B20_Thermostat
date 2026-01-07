/*
 * mcal_gpio.c
 *
 * Created: 12/21/2025 2:43:29 PM
 *  Author: conti
 */ 

#include "mcal_gpio.h"

/**
 * @brief Configures a GPIO pin as either an input or an output.
 */
Std_ReturnType_t Mcal_Gpio_Init(const Mcal_Gpio_Config_t* pstPin, Mcal_Gpio_Direction_t enDir) {
    Std_ReturnType_t enRetVal = E_NOT_OK;

    /* Security check: Ensure we aren't dereferencing a null pointer */
    if (NULL_PTR != pstPin) {
        if (GPIO_OUTPUT == enDir) {
            /* Set the bit in the Data Direction Register */
            *(pstPin->pu8Ddr) |= (1U << pstPin->u8PinNum);
        } else {
            /* Clear the bit in the Data Direction Register */
            *(pstPin->pu8Ddr) &= ~(1U << pstPin->u8PinNum);
        }
        enRetVal = E_OK;
    }
    return enRetVal;
}

/**
 * @brief Sets the logical state of an output pin.
 */
Std_ReturnType_t Mcal_Gpio_Write(const Mcal_Gpio_Config_t* pstPin, Mcal_Gpio_Level_t enLevel) {
    Std_ReturnType_t enRetVal = E_NOT_OK;

    if (NULL_PTR != pstPin) {
        if (GPIO_HIGH == enLevel) {
            /* Set bit in the Port register */
            *(pstPin->pu8Port) |= (1U << pstPin->u8PinNum);
        } else {
            /* Clear bit in the Port register */
            *(pstPin->pu8Port) &= ~(1U << pstPin->u8PinNum);
        }
        enRetVal = E_OK;
    }
    return enRetVal;
}

/**
 * @brief Reads the current physical state of a GPIO pin.
 */
Mcal_Gpio_Level_t Mcal_Gpio_Read(const Mcal_Gpio_Config_t* pstPin) {
    Mcal_Gpio_Level_t enRetLevel = GPIO_LOW;

    if (NULL_PTR != pstPin) {
        /* Check the bit in the Input (PIN) register */
        if (*(pstPin->pu8Pin) & (1U << pstPin->u8PinNum)) {
            enRetLevel = GPIO_HIGH;
        } else {
            enRetLevel = GPIO_LOW;
        }
    }
    return enRetLevel;
}

/**
 * @brief Inverts the current state of an output pin.
 */
void Mcal_Gpio_Toggle(const Mcal_Gpio_Config_t* pstPin) {
    if (NULL_PTR != pstPin) {
        /* XOR the bit to flip it */
        *(pstPin->pu8Port) ^= (1U << pstPin->u8PinNum);
    }
}