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

//void Hal_Encoder_Init(const Hal_Encoder_Config_t* pstConfig) {
   // if (NULL_PTR != pstConfig) {
        /* Initialize CLK and DT as inputs */
  //      (void)Mcal_Gpio_Init(pstConfig->pstPinA, GPIO_INPUT);
   //     (void)Mcal_Gpio_Init(pstConfig->pstPinB, GPIO_INPUT);
        
        /* Initialize SW as input with pull-up */
    //    (void)Mcal_Gpio_Init(pstConfig->pstPinSW, GPIO_INPUT);
        
        /* Capture startup state */
   //     u8LastStateA = Mcal_Gpio_Read(pstConfig->pstPinA);
  //  }
//}
void Hal_Encoder_Init(const Hal_Encoder_Config_t* pstConfig) {
	if (NULL_PTR != pstConfig) {
		/* Initialize CLK (Pin A) as Input + Pull-Up */
		(void)Mcal_Gpio_Init(pstConfig->pstPinA, GPIO_INPUT);
		(void)Mcal_Gpio_Write(pstConfig->pstPinA, GPIO_HIGH); /* Enable Pull-up */

		/* Initialize DT (Pin B) as Input + Pull-Up */
		(void)Mcal_Gpio_Init(pstConfig->pstPinB, GPIO_INPUT);
		(void)Mcal_Gpio_Write(pstConfig->pstPinB, GPIO_HIGH); /* Enable Pull-up */
		
		/* Initialize SW (Button) as Input + Pull-Up */
		(void)Mcal_Gpio_Init(pstConfig->pstPinSW, GPIO_INPUT);
		(void)Mcal_Gpio_Write(pstConfig->pstPinSW, GPIO_HIGH); /* Enable Pull-up */
		
		/* Capture startup state */
		u8LastStateA = Mcal_Gpio_Read(pstConfig->pstPinA);
	}
}


/* Global variable to remember previous pin states (A and B) */
/* 0 = Low, 1 = High. Combined (A << 1) | B */
static uint8_t u8OldState = 3; /* Start assuming both High (3) */

EncoderEvent_t Hal_Encoder_GetEvent(const Hal_Encoder_Config_t* pstConfig) {
    EncoderEvent_t enEvent = ENCODER_IDLE;
    uint8_t u8NewState;
    uint8_t u8PinA, u8PinB;

    if (NULL_PTR != pstConfig) {
        /* --- 1. Handle Button (Simple Debounce) --- */
        if (GPIO_LOW == Mcal_Gpio_Read(pstConfig->pstPinSW)) {
             /* Check again after small wait to confirm */
             _delay_us(100); 
             if (GPIO_LOW == Mcal_Gpio_Read(pstConfig->pstPinSW)) {
                 enEvent = ENCODER_BTN_PRESSED;
                 /* Wait for release to prevent repeated toggles */
                 while(GPIO_LOW == Mcal_Gpio_Read(pstConfig->pstPinSW));
                 return enEvent; /* Return immediately */
             }
        }

        /* --- 2. Handle Rotation (State Machine) --- */
        u8PinA = Mcal_Gpio_Read(pstConfig->pstPinA);
        u8PinB = Mcal_Gpio_Read(pstConfig->pstPinB);
        
        /* Combine pins into a 2-bit number: (A, B) */
        /* Example: A=1, B=0 -> 10 binary -> 2 decimal */
        u8NewState = (u8PinA << 1) | u8PinB;

        /* If state hasn't changed, do nothing */
        if (u8NewState == u8OldState) {
            return ENCODER_IDLE;
        }

        /* State Table Logic:
         * We look at transitions. 
         * Standard quadrature moves: 3 -> 1 -> 0 -> 2 -> 3 (CW)
         * Or reverse: 3 -> 2 -> 0 -> 1 -> 3 (CCW)
         */
        
        /* Check specific transitions that represent a "Click" */
        if ((u8OldState == 3 && u8NewState == 1) || 
            (u8OldState == 0 && u8NewState == 2)) {
            enEvent = ENCODER_CW;  /* Clockwise */
        }
        else if ((u8OldState == 3 && u8NewState == 2) || 
                 (u8OldState == 0 && u8NewState == 1)) {
            enEvent = ENCODER_CCW; /* Counter-Clockwise */
        }

        /* Save new state for next time */
        u8OldState = u8NewState;
    }

    return enEvent;
}