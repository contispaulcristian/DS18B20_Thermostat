/*
 * hal_encoder.c
 *
 * Created: 12/21/2025 3:30:06 PM
 *  Author: conti
 */ 

#include "mcal_clock.h"
#include "hal_encoder.h"
#include <util/delay.h>

/* Static variable to store the last state of CLK (Pin A) */
static uint8_t u8LastStateA = GPIO_HIGH;

void Hal_Encoder_Init(const Hal_Encoder_Config_t* pstConfig) {
    if (NULL_PTR != pstConfig) {
        /* Initialize CLK and DT as inputs */
        (void)Mcal_Gpio_Init(pstConfig->pstPinA, GPIO_INPUT);
        (void)Mcal_Gpio_Init(pstConfig->pstPinB, GPIO_INPUT);
        
        /* Initialize SW as input with pull-up */
        (void)Mcal_Gpio_Init(pstConfig->pstPinSW, GPIO_INPUT);
        
        /* Capture startup state */
        u8LastStateA = Mcal_Gpio_Read(pstConfig->pstPinA);
    }
}

EncoderEvent_t Hal_Encoder_GetEvent(const Hal_Encoder_Config_t* pstConfig) {
    EncoderEvent_t enEvent = ENCODER_IDLE;
    uint8_t u8CurrentStateA;

    if (NULL_PTR != pstConfig) {
        /* Check Button (Active Low) */
        if (GPIO_LOW == Mcal_Gpio_Read(pstConfig->pstPinSW)) {
            /* Simple blocking debounce */
            _delay_ms(20); 
            if (GPIO_LOW == Mcal_Gpio_Read(pstConfig->pstPinSW)) {
                enEvent = ENCODER_BTN_PRESSED;
                /* Wait for release to avoid multiple triggers */
                while(GPIO_LOW == Mcal_Gpio_Read(pstConfig->pstPinSW));
            }
        }

        /* Check Rotation (if no button press detected) */
        if (ENCODER_IDLE == enEvent) {
            u8CurrentStateA = Mcal_Gpio_Read(pstConfig->pstPinA);

            if (u8CurrentStateA != u8LastStateA) {
                /* If CLK changed, DT tells us the direction */
                if (Mcal_Gpio_Read(pstConfig->pstPinB) != u8CurrentStateA) {
                    enEvent = ENCODER_CW;
                } else {
                    enEvent = ENCODER_CCW;
                }
                u8LastStateA = u8CurrentStateA;
            }
        }
    }

    return enEvent;
}