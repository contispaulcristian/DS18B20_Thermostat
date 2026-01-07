/*
 * hal_encoder_cfg.c
 *
 * Created: 12/22/2025 4:07:48 PM
 *  Author: conti
 */ 

#include "hal_encoder_cfg.h"
#include "mcal_gpio_cfg.h"

/**
 * @brief Logical grouping of the HW-040 Encoder pins.
 * This assembly links the physical MCAL pins to the HAL driver.
 */
const Hal_Encoder_Config_t Hal_Encoder_DefaultCfg = {
    .pstPinA  = &Mcal_EncoderClk, /* Physical Pin PB0 */
    .pstPinB  = &Mcal_EncoderDt,  /* Physical Pin PB1 */
    .pstPinSW = &Mcal_EncoderSw   /* Physical Pin PB2 */
};