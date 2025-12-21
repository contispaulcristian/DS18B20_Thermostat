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
enStd_ReturnType enMcal_Gpio_Init(const stMcal_Gpio_Config_t* pstPin, enMcal_Gpio_Direction_t enDir) {
    enStd_ReturnType enRet = E_NOT_OK;

    /* Security check: Ensure we aren't dereferencing a null pointer */
    if (NULL_PTR != pstPin) {
        if (GPIO_OUTPUT == enDir) {
            /* Set the bit in the Data Direction Register */
            *(pstPin->pu8Ddr) |= (1U << pstPin->u8PinNum);
        } else {
            /* Clear the bit in the Data Direction Register */
            *(pstPin->pu8Ddr) &= ~(1U << pstPin->u8PinNum);
        }
        enRet = E_OK;
    }
    return enRet;
}

/**
 * @brief Sets the logical state of an output pin.
 */
enStd_ReturnType enMcal_Gpio_Write(const stMcal_Gpio_Config_t* pstPin, enMcal_Gpio_Level_t enLevel) {
    enStd_ReturnType enRet = E_NOT_OK;

    if (NULL_PTR != pstPin) {
        if (GPIO_HIGH == enLevel) {
            /* Set bit in the Port register */
            *(pstPin->pu8Port) |= (1U << pstPin->u8PinNum);
        } else {
            /* Clear bit in the Port register */
            *(pstPin->pu8Port) &= ~(1U << pstPin->u8PinNum);
        }
        enRet = E_OK;
    }
    return enRet;
}

/**
 * @brief Reads the current physical state of a GPIO pin.
 */
enMcal_Gpio_Level_t enMcal_Gpio_Read(const stMcal_Gpio_Config_t* pstPin) {
    enMcal_Gpio_Level_t enRetLevel = GPIO_LOW;

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
void vMcal_Gpio_Toggle(const stMcal_Gpio_Config_t* pstPin) {
    if (NULL_PTR != pstPin) {
        /* XOR the bit to flip it */
        *(pstPin->pu8Port) ^= (1U << pstPin->u8PinNum);
    }
}