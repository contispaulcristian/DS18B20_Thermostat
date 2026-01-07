/*
 * hal_relay.c
 *
 * Created: 12/21/2025 3:30:43 PM
 *  Author: conti
 */ 


#include "hal_relay.h"

void Hal_Relay_Init(const Mcal_Gpio_Config_t* pstRelay) {
    if (NULL_PTR != pstRelay) {
        /* Set the pin as an output */
        (void)Mcal_Gpio_Init(pstRelay, GPIO_OUTPUT);
        
        /* Ensure relay starts in the OFF state (assuming Active High) */
        (void)Mcal_Gpio_Write(pstRelay, GPIO_HIGH);
    }
}

void Hal_Relay_TurnOn(const Mcal_Gpio_Config_t* pstRelay) {
    /* Logic to drive the relay high */
    (void)Mcal_Gpio_Write(pstRelay, GPIO_LOW);
}

void Hal_Relay_TurnOff(const Mcal_Gpio_Config_t* pstRelay) {
    /* Logic to drive the relay low */
    (void)Mcal_Gpio_Write(pstRelay, GPIO_HIGH);
}
