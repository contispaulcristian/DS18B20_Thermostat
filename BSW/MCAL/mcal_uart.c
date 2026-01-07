/*
 * mcal_uart.c
 *
 * Created: 12/21/2025 3:20:27 PM
 *  Author: conti
 */ 

#include "mcal_uart.h"

/**
 * @brief Initializes UART Peripheral using the hardware formula:
 * UBRR = (F_CPU / (16 * Baud)) - 1
 */
UartStatus_t Mcal_Uart_Init(const Mcal_Uart_Config_t* pstConfig) {
    UartStatus_t enRetVal = UART_ERROR;
    uint16_t u16UbrrValue;

    if (NULL_PTR != pstConfig) {
        /* Calculate UBRR using descriptive constants instead of magic numbers */
        u16UbrrValue = (uint16_t)((MCAL_GET_CPU_FREQ() / 
                       (UART_SAMPLING_DIVISOR * pstConfig->u32BaudRate_bps)) - UART_UBRR_OFFSET);

        /* Rest of the initialization... */
        UBRR0H = (uint8_t)(u16UbrrValue >> 8);
        UBRR0L = (uint8_t)(u16UbrrValue);     
        
        enRetVal = UART_OK;
    }
    return enRetVal;
}

/**
 * @brief Transmits a single byte (Blocking)
 */
void Mcal_Uart_Write(uint8_t u8Data_in) {
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)));
    
    /* Put data into buffer, sends the data */
    UDR0 = u8Data_in;
}

/**
 * @brief Receives a single byte (Blocking)
 */
uint8_t Mcal_Uart_Read(void) {
    /* Wait for data to be received */
    while (!(UCSR0A & (1 << RXC0)));
    
    /* Get and return received data from buffer */
    return UDR0;
}