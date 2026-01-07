/*
 * mcal_uart.h
 *
 * Created: 12/21/2025 3:20:37 PM
 *  Author: conti
 */ 


#ifndef MCAL_UART_H_
#define MCAL_UART_H_

#include "mcal_std_types.h"
#include "mcal_clock.h"

/* --- UART Hardware Constants --- */
/** @brief Divisor for Normal Speed Mode (U2X0 = 0) */
#define UART_SAMPLING_DIVISOR        16UL

/** @brief UBRR register is zero-indexed */
#define UART_UBRR_OFFSET             1U

/* UART Status Definitions */
typedef enum {
    UART_OK = 0U,
    UART_ERROR,
    UART_BUSY
} UartStatus_t;

/** * @brief Configuration structure for UART Initialization
 */
typedef struct {
    uint32_t u32BaudRate_bps;   /**< Baudrate (e.g., 9600, 115200) */
    uint8_t  u8DataBits;        /**< Number of data bits (usually 8) */
    uint8_t  u8StopBits;        /**< 1 or 2 stop bits */
} Mcal_Uart_Config_t;

/* --- Function Prototypes --- */

/**
 * @brief Initializes the UART peripheral based on the config structure.
 * @param pstConfig Pointer to the configuration struct.
 * @return tenUartStatus UART_OK if successful.
 */
UartStatus_t Mcal_Uart_Init(const Mcal_Uart_Config_t* pstConfig);

/**
 * @brief Transmits a single byte over UART.
 */
void Mcal_Uart_Write(uint8_t u8Data_in);

/**
 * @brief Receives a single byte from UART (Blocking).
 */
uint8_t Mcal_Uart_Read(void);

/**
 * @brief 
 */
void Mcal_Uart_WriteString(const char* pszStr);

#endif /* MCAL_UART_H_ */