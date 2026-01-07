/*
 * hal_encoder.h
 *
 * Created: 12/21/2025 3:29:54 PM
 *  Author: conti
 */ 


#ifndef HAL_ENCODER_H_
#define HAL_ENCODER_H_

#include "mcal_gpio.h"

/** * @brief Direction and Button states */
typedef enum {
    ENCODER_IDLE = 0U,
    ENCODER_CW,        /* Clockwise */
    ENCODER_CCW,       /* Counter-Clockwise */
    ENCODER_BTN_PRESSED
} EncoderEvent_t;

/** * @brief HW-040 Configuration structure */
typedef struct {
    const Mcal_Gpio_Config_t* pstPinA;  /**< CLK Pin */
    const Mcal_Gpio_Config_t* pstPinB;  /**< DT Pin  */
    const Mcal_Gpio_Config_t* pstPinSW; /**< Switch Pin */
} Hal_Encoder_Config_t;

/* --- Function Prototypes --- */

/**
 * @brief Initializes the CLK, DT, and SW pins.
 * @param pstConfig Pointer to the HW-040 configuration.
 */
void Hal_Encoder_Init(const Hal_Encoder_Config_t* pstConfig);

/**
 * @brief Polls the encoder for rotation or button press.
 * @param pstConfig Pointer to the configuration.
 * @return tenEncoderEvent The detected event.
 */
EncoderEvent_t Hal_Encoder_GetEvent(const Hal_Encoder_Config_t* pstConfig);


#endif /* HAL_ENCODER_H_ */